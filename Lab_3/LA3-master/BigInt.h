#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define POSITIVE 1
#define NEGATIVE -1

class BigInt
{
private:
	unsigned short * digit_mass;
	int Last, Basis;

	int comp(const BigInt &a) const;
	int compmod(const BigInt &a) const;
	void clear();
	void shift(int d);
	BigInt _addsub(const BigInt &a, const BigInt &b) const;
	BigInt _mul(const BigInt a, const BigInt b) const;
	BigInt _div(const BigInt a, const BigInt b, bool mod) const;
	BigInt _pow(const BigInt a, const BigInt b) const;

public:
	int Signum;
	BigInt();
	BigInt(const BigInt &Source);
	BigInt(char* str, int b);
	BigInt(int Source, int b);
	~BigInt();

	void setbase(int b);
	void scan(char* file);
	void print(char* file);

	unsigned short & operator[] (int i);
	unsigned short operator[] (int i) const;

	bool operator== (const BigInt &Source) const;
	bool operator<= (const BigInt &Source) const;
	bool operator>= (const BigInt &Source) const;
	bool operator!= (const BigInt &Source) const;
	bool operator< (const BigInt &Source) const;
	bool operator> (const BigInt &Source) const;

	BigInt operator= (BigInt Source);

	BigInt operator+ (const BigInt &Source) const;
	BigInt operator- (const BigInt &Source) const;
	BigInt operator* (const BigInt &Source) const;
	BigInt operator/ (const BigInt &Source) const;
	BigInt operator^ (const BigInt &Source) const;
	BigInt operator% (const BigInt &Source) const;
};
