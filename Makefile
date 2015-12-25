all: libRS RStest example

libRS:
	cd src && make

RStest:
	cd tests && make

example:
	cd examples && make

clean:
	cd src && make clean
	cd examples && make clean
	cd tests && make clean

