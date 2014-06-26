#include "BigInt.h"

int max(int a, int b){
	return a > b ? a : b;}

BigInt::BigInt() {
	digit_mass = NULL;
	Last = 0;
	signum = POSITIVE;
	Basis = 10;
}

BigInt::BigInt(const BigInt &Source) {
	digit_mass = new unsigned short[Source.Last + 1];
	Last = Source.Last;
	Basis = Source.Basis;
	signum = Source.signum;
	for (int i = 0; i <= Last; i++) digit_mass[i] = Source[i];
}

BigInt::BigInt(char* str, int b)
{
	Basis = b;
	int n = strlen(str);
	signum = POSITIVE;
	if (Basis == 10){
		if (str[0] == '-')	{
			signum = NEGATIVE;
			Last = n - 2;
			digit_mass = new unsigned short[n - 1];
			for (int i = 1; i < n; i++) digit_mass[n - i - 1] = (unsigned char)str[i] - '0';}
		else	{
			Last = n - 1;
			digit_mass = new unsigned short[n];
			for (int i = 0; i < n; i++) digit_mass[n - i] = (unsigned char)str[i] - '0';	}
	}
	else	{
		Last = n - 1;
		digit_mass = new unsigned short[n+1];
		for (int i = 0; i < Last; i++) digit_mass[n - i] = (unsigned char)str[i];
	}
}

BigInt::BigInt(int Source, int b)
{
	Basis = b;
	if (Source >= 0) signum = POSITIVE;
	else signum = NEGATIVE;

	long long temp = Basis;
	int p = 0;
	unsigned char a[11];

	if (Basis == 10) {
		if (Source < 0) {
			signum = NEGATIVE;
			Source *= -1;}
		for (int i = 0; i < 9; i++)	{
			int x = Source % temp;
			x = x / (temp / Basis);
			a[i] = x;
			if (x != 0)
			{
				p = i;
			}
			temp *= Basis;}
		digit_mass = new unsigned short[p + 1];
		Last = p;
		for (int i = 0; i <= p; i++)	digit_mass[i] = a[i];
	}
	else	{
		for (int i = 0; i < 4; i++)	{
			int x = Source % temp;
			x = x / (temp / Basis);
			a[i] = x;
			if (x != 0)	 p = i;
			temp *= Basis;}
		digit_mass = new unsigned short[p + 1];
		Last = p;
		for (int i = 0; i <= p; i++) digit_mass[i] = a[i]; }
	if (Source == 0) Last = 0;
}

BigInt::~BigInt()
{
	if (digit_mass != NULL) delete[] digit_mass;
	digit_mass = NULL;
	Last = NULL;
}

void BigInt::setbase(int b) {
	Basis = b;
}

void BigInt::scan(char* file)
{
	FILE * f = NULL;
	char mode[3];
	if (Basis == 10) strcpy(mode, "r");
	else strcpy(mode, "rb");
	f = fopen(file, mode);
	if (f == NULL) exit(0);

	signum = POSITIVE;
	string buf;
	int i;
	for (i = 0; !feof(f); i++){
		if (Basis == 10){
			char temp = fgetc(f);
			if (i == 0 && temp == '-'){
				signum = NEGATIVE;
				i--;
				continue;
			}
			buf.push_back(temp - 48);	}
		else	buf.push_back ((unsigned char)fgetc(f));}
	Last = i - 2;
	if (digit_mass != NULL){
		delete[] digit_mass;
		digit_mass = NULL;}
	digit_mass = new unsigned short[Last + 1];
	for (i = 0; i <= Last; i++) digit_mass[Last - i] = (unsigned char)buf[i];
	fclose(f);
}

void BigInt::print(char* file){
	char mode[4];
	FILE * f = NULL;
	if (digit_mass == NULL){
		printf("Nothing to write!");
		exit(0);}
	if (Basis == 10){
		strcpy(mode, "w");}
	else{
		strcpy(mode, "wb+\0");}
	f = fopen(file, mode);
	if (f == NULL){
		printf("Error!\n");
		exit(0);}
	if (signum == NEGATIVE){
		fputc('-', f);}
	for (int i = Last; i >= 0; i--){
		if (Basis == 10) fputc(digit_mass[i] + '0', f);
		else fputc(digit_mass[i], f);}
	fclose(f);
}

int BigInt::comp(const BigInt &a) const{
	if ((a.signum == NEGATIVE) && (this->signum == POSITIVE)) return(POSITIVE);
	if ((a.signum == POSITIVE) && (this->signum == NEGATIVE)) return(NEGATIVE);

	if (this->Last > a.Last) return (POSITIVE * a.signum);
	if (a.Last > this->Last) return (NEGATIVE * a.signum);

	for (int i = a.Last; i >= 0; i--) {
		if (a.digit_mass[i] > this->digit_mass[i]) return(NEGATIVE * a.signum);
		if (this->digit_mass[i] > a.digit_mass[i]) return(POSITIVE * a.signum);
	}
	return(0);
}

int BigInt::compmod(const BigInt &a) const{
	if (this->Last > a.Last) return (POSITIVE);
	if (a.Last > this->Last) return (NEGATIVE);

	for (int i = a.Last; i >= 0; i--) {
		if (a.digit_mass[i] > this->digit_mass[i]) return(NEGATIVE);
		if (this->digit_mass[i] > a.digit_mass[i]) return(POSITIVE);
	}
	return(0);
}

unsigned short & BigInt::operator[] (int i){
	if (i > Last)	{
		unsigned short a = 0;
		return a;	}
	return this->digit_mass[i];
}

unsigned short BigInt::operator[] (int i) const{
	if (i > Last) return 0;
	return this->digit_mass[i];
}

bool BigInt::operator== (const BigInt &Source) const{
	return this->comp(Source) == 0;
}

bool BigInt::operator<= (const BigInt &Source) const{
	return this->comp(Source) <= 0;
}

bool BigInt::operator>= (const BigInt &Source) const{
	return this->comp(Source) >= 0;
}

bool BigInt::operator!= (const BigInt &Source) const{
	return this->comp(Source) != 0;
}

bool BigInt::operator< (const BigInt &Source) const{
	return this->comp(Source) < 0;
}

bool BigInt::operator> (const BigInt &Source) const{
	return this->comp(Source) > 0;
}

BigInt BigInt::operator=(BigInt Source){
	if (digit_mass != NULL) delete[] digit_mass;
	digit_mass = new unsigned short[Source.Last + 1];
	Last = Source.Last;
	signum = Source.signum;
	Basis = Source.Basis;
	for (int i = 0; i <= Last; i++) digit_mass[i] = Source[i];
	return *this;
}

void BigInt::clear() {
	while ((Last > 0) && (digit_mass[Last] == 0))	Last--;
	if ((Last == 0) && (digit_mass[0] == 0))	{
		signum = POSITIVE;
		delete[] digit_mass;
		digit_mass = new unsigned short[1];
		digit_mass[0] = 0;
	}
	else	{
		unsigned short* temp = new unsigned short[Last + 1];
		for (int i = 0; i <= Last; i++) temp[i] = digit_mass[i];
		delete[] digit_mass;
		digit_mass = temp;	}
}

void BigInt::shift(int d){
	int i;

	if ((Last == 0) && (digit_mass[0] == 0)) return;

	unsigned short* temp = new unsigned short[Last + d + 1];

	for (i = Last; i >= 0; i--)	temp[i + d] = digit_mass[i];
	for (i = 0; i<d; i++) temp[i] = 0;
	Last = Last + d;
	delete[] digit_mass;
	digit_mass = temp;
}

BigInt BigInt::_addsub(const BigInt &a, const BigInt &b) const{
	int carry;				

	BigInt c;
	c.setbase(a.Basis);

	if (a.signum == b.signum)
	{
		c.signum = a.signum;

		c.Last = max(a.Last, b.Last) + 1;
		c.digit_mass = new unsigned short[c.Last + 1];
		carry = 0;

		for (int i = 0; i <= (c.Last); i++) {
			int x, y;
			if (i > a.Last) x = 0;
			else x = a[i];
			if (i > b.Last) y = 0;
			else y = b[i];
			c.digit_mass[i] = (unsigned char)(carry + x + y) % Basis;
			carry = (carry + x + y) / Basis;	}

		c.clear();
		return c;
	}
	else	{
		int v;				
		int i;
		int borrow;

		if (a.compmod(b) == NEGATIVE) {
			c = b + a;
			return c;}
		
		c.signum = a.signum;
		c.Last = max(a.Last, b.Last);
		c.digit_mass = new unsigned short[c.Last + 1];
		borrow = 0;

		for (i = 0; i <= c.Last; i++) {
			v = (a[i] - borrow - b[i]);
			if (a[i] > 0)
				borrow = 0;
			if (v < 0) {
				v = v + Basis;
				borrow = 1;}

			c.digit_mass[i] = (unsigned char)v % Basis;
		}

		c.clear();
		return c;
	}
}

BigInt BigInt::_mul(const BigInt a, const BigInt b) const{
	BigInt temp1, temp2, c(0, a.Basis);
	temp1 = a;
	temp2.setbase(a.Basis);

	for (int i = 0; i <= b.Last; i++) {
		for (int j = 1; j <= b.digit_mass[i]; j++) {
			temp2 = c + temp1;
			c = temp2;
		}
		temp1.shift(1);
	}
	c.signum = a.signum * b.signum;
	c.clear();
	return c;
}

BigInt BigInt::_div(const BigInt a, const BigInt b, bool mod) const{
	int i, j;

	BigInt temp1(0, a.Basis), temp2(0, a.Basis), temp3(0, a.Basis), c;

	c.signum = a.signum * b.signum;
	c.setbase(a.Basis);
	temp3 = b;
	temp3.signum = POSITIVE;

	c.Last = a.Last;
	c.digit_mass = new unsigned short[c.Last + 1];

	for (int i = a.Last; i >= 0; i--) {
		temp1.shift(1);
		temp1.clear();
		temp1[0] = a[i];
		c[i] = 0;
		while (temp1 >= temp3) {
			c[i]++;
			temp2 = temp1 - temp3;
			temp2.clear();
			temp1 = temp2;}}

	c.clear();

	if (mod == 1){
		BigInt temp4;
		temp4 = b * c;
		if (a.signum == NEGATIVE)	 temp4.signum = POSITIVE;
		c = a - temp4;
		c.signum = POSITIVE;	}
	return c;
}

BigInt BigInt::_pow(const BigInt a, const BigInt b) const {
	BigInt c(1, a.Basis), temp1(1, a.Basis), temp2(0, a.Basis), temp3;
	temp3 = b;
	for (; b.Last != 0 && b.digit_mass[0] != 0; temp2 = b - temp1){
		temp3 = temp2;
		temp2 = c * b;
		c = temp2;}
	return c;
}

BigInt BigInt::operator+ (const BigInt &Source) const{
	return _addsub(*this, Source);
}

BigInt BigInt::operator- (const BigInt &Source) const{
	BigInt temp;
	temp = Source;
	temp.signum *= -1;
	return _addsub(*this, temp);
}

BigInt BigInt::operator* (const BigInt &Source) const{
	return _mul(*this, Source);}

BigInt BigInt::operator/ (const BigInt &Source) const{
	return _div(*this, Source, 0);}

BigInt BigInt::operator^ (const BigInt &Source) const{
	return _pow(*this, Source);}

BigInt BigInt::operator% (const BigInt &Source) const{
	return _div(*this, Source, 1);}