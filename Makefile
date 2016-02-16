all:
	mkdir -p build && cd build && cmake ../ && make

x32:
	mkdir -p build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../x32.cmake_toolchain ../ && make

clean:
	cd build && make clean && cd .. && rm -rf build

