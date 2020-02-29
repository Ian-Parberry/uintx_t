check: main.cpp expressiontree.cpp expressiontree.h lexical.cpp lexical.h parser.cpp parser.h
	g++ -std=c++11 -O3 -o check.exe -I ../../uintx_t/Src Main.cpp expressiontree.cpp lexical.cpp parser.cpp ../../lib/uintx_t.a
	mv check.exe ../..

cleanup:
	@rm -f .makefile.* 
