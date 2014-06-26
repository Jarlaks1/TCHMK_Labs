all:
	swig -c++ -python BigInt.i
	g++ -fPIC -c BigInt.cpp
	g++ -fPIC -c BigInt_wrap.cxx -I/usr/include/python2.7
	g++ -shared BigInt.o BigInt_wrap.o -o _BigInt.so
