lib: uintx_math.cpp uintx_math.h uintx_t.cpp uintx_t.h
	g++ -std=c++11 -O3 -c uintx_math.cpp uintx_t.cpp
	ar rs uintx_t.a uintx_math.o uintx_t.o 2> /dev/null
	mv uintx_t.a ../../lib

cleanup:
	@rm uintx_math.o uintx_t.o
	@rm -f .makefile.* 

