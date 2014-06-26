all:
	g++ -fPIC -c BigInt.cpp -o BigInt.o
	g++ -shared -o BigInt.so BigInt.o
	g++ main.cpp BigInt.so -I. -Wl,-rpath,. -L.
