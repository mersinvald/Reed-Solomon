#ifndef RS_HPP
#define RS_HPP
#include <stdlib.h>
#include <iostream>
#include "poly.hpp"
#include "gf.hpp"

enum RESULT {
    RESULT_SUCCESS,
    RESULT_ERROR
};

namespace RS {

typedef uint8_t  uint8;
typedef unsigned uint;

#define MSG_CNT 3   // необходимое количество полиномов длиной в сообщение
#define POLY_CNT 14 // необходимое количество полиномов длиной в ecc_length * 2

template <const uint8 msg_length,  // Длина сообщения без кода коррекции
          const uint8 ecc_length>  // Длина кода коррекции

class ReedSolomon {
public:
    ReedSolomon() {
        const uint8 enc_len  = msg_length + ecc_length;
        const uint8 poly_len = ecc_length * 2;
        uint  offset = 0;

        /* Заполняем первые 6 вручную, так как расположение зависит от шаблона:
         * Кодеру не требуются ID_MSG_E и далее, а он длиной в целое сообщение,
         * тогда как T_POLY имеют длину nsy,*2 */

        polynoms[0].Init(ID_MSG_IN, offset, enc_len, &memory);
        offset += enc_len;

        polynoms[1].Init(ID_MSG_OUT, offset, enc_len, &memory);
        offset += enc_len;

        for(uint i = ID_GENERATOR; i < ID_MSG_E; i++) {
            polynoms[i].Init(i, offset, poly_len, &memory);
            offset += poly_len;
        }

        polynoms[5].Init(ID_MSG_E, offset, enc_len, &memory);
        offset += enc_len;

        for(uint i = ID_TPOLY3; i < ID_ERR_EVAL+1; i++) {
            polynoms[i].Init(i, offset, poly_len, &memory);
            offset += poly_len;
        }
    }

    ~ReedSolomon() {
        // Декструктор-пустышка, без него компилятор пытается освободить память на стеке через delete
        memory = NULL;
    }

    /* @brief Кодирование сообщения
     * @param *src - указатель на исходное сообщение             (размером msg_lenth)
     * @param *dst - буффер для записи закодированного сообщения (размером >= msg_length + ecc_length */
    void Encode(void* src, void* dst) {
        #ifdef DEBUG
        assert(msg_length + ecc_length < 256);
        #endif

        /* Статический массив для кэширования генератора */
        static uint8 generator[ecc_length+1] = {0};
        static bool generator_cached = false;

        /* Выделяем на стеке паямять для полиномов */
        uint8 stack_memory[MSG_CNT * msg_length + POLY_CNT * ecc_length * 2];
        this->memory = stack_memory;

        uint8 *src_ptr = (uint8*) src;
        uint8 *dst_ptr = (uint8*) dst;

        Poly &msg_in  = polynoms[ID_MSG_IN];
        Poly &msg_out = polynoms[ID_MSG_OUT];
        Poly &gen     = polynoms[ID_GENERATOR];

        // Зануляем полиномы (зачем занулять msg_in я мозгом так и не допёр, но без этого не работает)
        // Одним словом, weird shit
        msg_in.Reset();
        msg_out.Reset();

        // Используем кэшированный генератор или генерируем новый
        if(generator_cached) {
            gen.Set(generator, sizeof(generator));
        } else {
            GeneratorPoly();
            memcpy(generator, gen.ptr(), gen.length);
            generator_cached = true;
        }

        // Копируем сообщение в полиномы
        msg_in.Set(src_ptr, msg_length);
        msg_out = msg_in;
        msg_out.length = msg_in.length + ecc_length;

        // Тут происходит магия кодирования
        uint8 coef; // кэш
        for(uint i = 0; i < msg_length; i++){
            coef = msg_out[i];
            if(coef != 0){
                for(uint j = 1; j < gen.length; j++){
                    msg_out[i+j] ^= gf::mul(gen[j], coef);
                }
            }
        }

        // Копируем сообщение
        memcpy(dst_ptr, src_ptr, msg_length * sizeof(uint8));

        // Копируем ECC
        memcpy(dst_ptr+msg_length, msg_out.ptr()+msg_length, ecc_length * sizeof(uint8));
    }

    /* @brief Декодирование сообщения
     * @param *msg_in      - указатель на закодированное сообщение       (размером msg_length + ecc_length)
     * @param *msg_out     - буффер для записи декодированного сообщения (размером >= msg_length)
     * @param *erase_pos   - позиции байтов, содержащих ошибки           (если известны)
     * @param erase_count  - количество байтов, содержащих ошибки
     * @return RESULT_SUCCESS при успешном завершении, иначе код ошибки */
     int Decode(void* src, void* dst, uint8* erase_pos = nullptr, size_t erase_count = 0) {
        #ifdef DEBUG
        assert(msg_length + ecc_length < 256);
        #endif

        uint8 *src_ptr = (uint8*) src;
        uint8 *dst_ptr = (uint8*) dst;

        const uint src_len = msg_length + ecc_length;
        const uint dst_len = msg_length;

        /* Выделяем на стеке паямять для полиномов */
        uint8 stack_memory[MSG_CNT * msg_length + POLY_CNT * ecc_length * 2];
        this->memory = stack_memory;

        Poly &msg_in  = polynoms[ID_MSG_IN];
        Poly &msg_out = polynoms[ID_MSG_OUT];
        Poly &epos    = polynoms[ID_ERASURES];

        // Копируем сообщение в полиномы
        msg_in.Set(src_ptr, src_len);
        msg_out = msg_in;

        // Записываем известные ошибки в полином, если они были переданы
        if(erase_pos == NULL) {
            epos.length = 0;
        } else {
            epos.Set(erase_pos, erase_count);
            for(uint i = 0; i < epos.length; i++){
                msg_in[epos[i]] = 0;
            }
        }

        // Известных ошибок больше, чем может быть исправлено
        if(epos.length > ecc_length) return RESULT_ERROR;

        Poly &synd   = polynoms[ID_SYNDROMES];
        Poly &eloc   = polynoms[ID_ERRORS_LOC];
        Poly &reloc  = polynoms[ID_TPOLY1];
        Poly &err    = polynoms[ID_ERRORS];
        Poly &forney = polynoms[ID_FORNEY];

        // Вычисляем синдромы полинома
        CalcSyndromes(msg_in);

        // Проверяем, есть ли ошибки
        bool has_errors = false;
        for(uint i = 0; i < synd.length; i++) {
            if(synd[i] != 0) {
                has_errors = true;
                break;
            }
        }

        // Записываем сообщение и выходим, если нет ошибок
        if(!has_errors) goto return_corrected_msg;

        CalcForneySyndromes(synd, epos, src_len);
        FindErrorLocator(forney, NULL, epos.length);

        // Разворачиваем синдром
        // TODO оптимизировать разворот через инверсию индексов
        reloc.length = eloc.length;
        for(int i = eloc.length-1, j = 0; i >= 0; i--, j++){
            reloc[j] = eloc[i];
        }

        // Вычисляем позиции ошибок
        FindErrors(reloc, src_len);

        // Произошла ошибка при поиске ошибок (so helpfull :D)
        if(err.length == 0) return RESULT_ERROR;

        /* Складываем найденные ошибки с уже известными */
        for(uint i = 0; i < err.length; i++) {
            epos.Append(err[i]);
        }

        // Исправляем
        CorrectErrata(synd, epos, msg_in);

    return_corrected_msg:
        msg_out.length = dst_len;
        memcpy(dst_ptr, msg_out.ptr(), msg_out.length * sizeof(uint8));
        return RESULT_SUCCESS;
    }

#ifndef DEBUG
private:
#endif

    enum POLY_ID {
        ID_MSG_IN = 0,
        ID_MSG_OUT,
        ID_GENERATOR,   // 3
        ID_TPOLY1,      // T for Temporary
        ID_TPOLY2,

        ID_MSG_E,       // 5

        ID_TPOLY3,     // 6
        ID_TPOLY4,

        ID_SYNDROMES,
        ID_FORNEY,

        ID_ERASURES_LOC,
        ID_ERRORS_LOC,

        ID_ERASURES,
        ID_ERRORS,

        ID_COEF_POS,
        ID_ERR_EVAL,
    };

    // Указатель на массив на стеке вызванного метода
    uint8* memory;
    Poly polynoms[MSG_CNT + POLY_CNT];

    #ifdef DEBUG
    const uint8 msg_len = msg_length;
    const uint8 ecc_len = ecc_length;
    #endif

    void GeneratorPoly() {
        Poly &gen = polynoms[ID_GENERATOR];
        gen[0] = 1;
        gen.length = 1;

        Poly &mulp = polynoms[ID_TPOLY1];
        Poly &temp = polynoms[ID_TPOLY2];
        mulp.length = 2;

        for(int i = 0; i < ecc_length; i++){
            mulp[0] = 1;
            mulp[1] = gf::pow(2, i);

            gf::poly_mul(gen, mulp, temp);

            gen = temp;
        }
    }

    void CalcSyndromes(const Poly &msg) {
        Poly &synd = polynoms[ID_SYNDROMES];
        synd.length = ecc_length+1;
        synd[0] = 0;
        for(uint i = 1; i < ecc_length+1; i++){
            synd[i] = gf::poly_eval(msg, gf::pow(2, i-1));
        }
    }

    void FindErrataLocator(const Poly &epos) {
        Poly &errata_loc = polynoms[ID_ERASURES_LOC];
        Poly &mulp = polynoms[ID_TPOLY1];
        Poly &addp = polynoms[ID_TPOLY2];
        Poly &apol = polynoms[ID_TPOLY3];
        Poly &temp = polynoms[ID_TPOLY4];

        errata_loc.length = 1;
        errata_loc[0] = 1;

        mulp.length = 1;
        addp.length = 2;

        for(uint i = 0; i < epos.length; i++){
            mulp[0] = 1;
            addp[0] = gf::pow(2, epos[i]);
            addp[1] = 0;

            gf::poly_add(mulp, addp, apol);
            gf::poly_mul(errata_loc, apol, temp);

            errata_loc = temp;
        }
    }

    void FindErrorEvaluator(const Poly &synd, const Poly &errata_loc, Poly &dst, uint8 ecclen) {
        Poly &mulp = polynoms[ID_TPOLY1];
        gf::poly_mul(synd, errata_loc, mulp);

        Poly &divisor = polynoms[ID_TPOLY2];
        divisor.length = ecclen+2;

        divisor.Reset();
        divisor[0] = 1;

        gf::poly_div(mulp, divisor, dst);
    }

    void CorrectErrata(const Poly &synd, const Poly &err_pos, const Poly &msg_in) {
        Poly &c_pos     = polynoms[ID_COEF_POS];
        Poly &corrected = polynoms[ID_MSG_OUT];
        c_pos.length = err_pos.length;

        for(uint i = 0; i < err_pos.length; i++)
            c_pos[i] = msg_in.length - 1 - err_pos[i];

        /* использует t_poly 1, 2, 3, 4 */
        FindErrataLocator(c_pos);
        Poly &errata_loc = polynoms[ID_ERASURES_LOC];

        /* разворачиваем синдромы */
        Poly &rsynd = polynoms[ID_TPOLY3];
        rsynd.length = synd.length;

        for(int i = synd.length-1, j = 0; i >= 0; i--, j++) {
            rsynd[j] = synd[i];
        }

        /* getting reversed error evaluator polynomial */
        Poly &re_eval = polynoms[ID_TPOLY4];

        /* uses T_POLY 1, 2 */
        FindErrorEvaluator(rsynd, errata_loc, re_eval, errata_loc.length-1);

        /* reversing it back */
        Poly &e_eval = polynoms[ID_ERR_EVAL];
        e_eval.length = re_eval.length;
        for(int i = re_eval.length-1, j = 0; i >= 0; i--, j++)
            e_eval[j] = re_eval[i];


        Poly &X = polynoms[ID_TPOLY1]; /* this will store errors positions */
        X.length = 0;

        int16_t l;
        for(uint i = 0; i < c_pos.length; i++){
            l = 255 - c_pos[i];
            X.Append(gf::pow(2, -l));
        }

        /* Magnitude polynomial */
        Poly &E = polynoms[ID_MSG_E];
        E.Reset();
        E.length = msg_in.length;

        uint8 Xi_inv;

        Poly &err_loc_prime_temp = polynoms[ID_TPOLY2];

        uint8 err_loc_prime;
        uint8 y;

        for(uint i = 0; i < X.length; i++){
            Xi_inv = gf::inverse(X[i]);

            err_loc_prime_temp.length = 0;
            for(uint j = 0; j < X.length; j++){
                if(j != i){
                    err_loc_prime_temp.Append(gf::sub(1, gf::mul(Xi_inv, X[j])));
                }
            }

            err_loc_prime = 1;
            for(uint j = 0; j < err_loc_prime_temp.length; j++){
                err_loc_prime = gf::mul(err_loc_prime, err_loc_prime_temp[j]);
            }

            y = gf::poly_eval(re_eval, Xi_inv);
            y = gf::mul(gf::pow(X[i], 1), y);

            E[err_pos[i]] = gf::div(y, err_loc_prime);
        }

        gf::poly_add(msg_in, E, corrected);
    }

    bool FindErrorLocator(const Poly &synd, Poly *erase_loc = NULL, size_t erase_count = 0) {
        Poly &error_loc = polynoms[ID_ERRORS_LOC];
        Poly &err_loc = polynoms[ID_TPOLY1];
        Poly &old_loc = polynoms[ID_TPOLY2];
        // По какой-то причине эта ссылка выкидывается коипилятором, так что копируем структуру.
        Poly temp    = polynoms[ID_TPOLY3];
        Poly &temp2   = polynoms[ID_TPOLY4];

        if(erase_loc != NULL) {
            err_loc = *erase_loc;
            old_loc = *erase_loc;
        } else {
            err_loc.length = 1;
            old_loc.length = 1;
            err_loc[0] = 1;
            old_loc[0] = 1;
        }

        uint synd_shift = 0;
        if(synd.length > ecc_length)
            synd_shift = synd.length - ecc_length;

        uint8 K = 0;
        uint8 delta = 0;
        uint8 index;

        for(uint i = 0; i < ecc_length - erase_count; i++){
            if(erase_loc != NULL)
                K = erase_count + i + synd_shift;
            else
                K = i + synd_shift;

            delta = synd[K];
            for(uint j = 1; j < err_loc.length; j++) {
                index = err_loc.length - j - 1;
                delta ^= gf::mul(err_loc[index], synd[K-j]);
            }

            old_loc.Append(0);

            if(delta != 0) {
                if(old_loc.length > err_loc.length) {
                    gf::poly_scale(old_loc, temp, delta);
                    gf::poly_scale(err_loc, old_loc, gf::inverse(delta));
                    err_loc = temp;
                }
                gf::poly_scale(old_loc, temp, delta);
                gf::poly_add(err_loc, temp, temp2);
                err_loc = temp2;
            }
        }

        uint shift = 0;
        while(err_loc.length && err_loc[shift] == 0) shift++;

        uint errs = err_loc.length - shift - 1;
        if(((errs - erase_count) * 2 + erase_count) > ecc_length){
            return false; /* количество ошибок больше, чем может быть исправлено! */
        }

        memcpy(error_loc.ptr(), err_loc.ptr() + shift, (err_loc.length - shift) * sizeof(uint8));
        error_loc.length = (err_loc.length - shift);
        return true;
    }

    bool FindErrors(const Poly& error_loc, size_t msg_in_size) {
        Poly &err = polynoms[ID_ERRORS];

        uint8 errs = error_loc.length - 1;
        err.length = 0;

        for(uint i = 0; i < msg_in_size; i++) {
            if(gf::poly_eval(error_loc, gf::pow(2, i)) == 0) {
                err.Append(msg_in_size - 1 - i);
            }
        }

        /* Sanity check:
         * the number of err/errata positions found
         * should be exactly the same as the length of the errata locator polynomial */
        if(err.length != errs)
            /* couldn't find error locations */
            return false;
        return true;
    }



    void CalcForneySyndromes(const Poly &synd, const Poly &erasures_pos, size_t msg_in_size) {
        Poly &erase_pos_reversed = polynoms[ID_TPOLY1];
        Poly &forney_synd = polynoms[ID_FORNEY];
        erase_pos_reversed.length = 0;

        for(uint i = 0; i < erasures_pos.length; i++){
            erase_pos_reversed.Append(msg_in_size - 1 - erasures_pos[i]);
        }

        forney_synd.Reset();
        forney_synd.Set(synd.ptr()+1, synd.length-1);

        uint8 x;
        for(uint i = 0; i < erasures_pos.length; i++) {
            x = gf::pow(2, erase_pos_reversed[i]);
            for(int j = 0; j < forney_synd.length - 1; j++){
                forney_synd[j] = gf::mul(forney_synd[j], x) ^ forney_synd[j+1];
            }
        }
    }
};

}

#endif // RS_HPP

