#include <stdio.h>
#include <string.h>

//reserved space for them digits
#define	CAP	100*1000
//signum
#define POSITIVE		1
#define NEGATIVE		-1

//def base for calculations
int base;

//created structure
typedef struct {
	short digit[CAP]; //the number
	int signbit;	  // + or -
	int lastdigit;   //most significant bit
} BigNum;

BigNum t1, t2, t3;
BigNum n1, n2, n3;

//read and write functions
get_from_file(BigNum *n, char * file);
write_to_file(BigNum *n, char* file);

//math happens
make_bignum(int s, BigNum *n)
{

	int i;
	int t;

	if (s >= 0) n->signbit = POSITIVE;
	else n->signbit = NEGATIVE;

	for (i = 0; i<CAP; i++) n->digit[i] = (unsigned char)0;

	n->lastdigit = -1;

	t = abs(s);

	while (t > 0) {
		n->lastdigit++;
		n->digit[n->lastdigit] = (t % base);
		t = t / base;
	}

	if (s == 0) n->lastdigit = 0;
}

int return_max(int a, int b)
{
	if (a > b) return(a); else return(b);
}

//number n gets multiplied by 10^d
shift(BigNum *n, int d)
{
	int i;

	if ((n->lastdigit == 0) && (n->digit[0] == 0)) return;

	for (i = n->lastdigit; i >= 0; i--)
		n->digit[i + d] = n->digit[i];

	for (i = 0; i<d; i++) n->digit[i] = 0;

	n->lastdigit = n->lastdigit + d;
}

//remove zeros
clear(BigNum *n)
{
	while ((n->lastdigit > 0) && (n->digit[n->lastdigit] == 0))
		n->lastdigit--;
	//clearing "-0"
	if ((n->lastdigit == 0) && (n->digit[0] == 0))
		n->signbit = POSITIVE;
}

add_nums(BigNum *a, BigNum *b, BigNum *c)
{
	int carry;
	int i;

	make_bignum(0, c);

	if (a->signbit == b->signbit) c->signbit = a->signbit;
	else {
		if (a->signbit == NEGATIVE) {
			a->signbit = POSITIVE;
			sub_nums(b, a, c);
			a->signbit = NEGATIVE;
		}
		else {
			b->signbit = POSITIVE;
			sub_nums(a, b, c);
			b->signbit = NEGATIVE;
		}
		return;
	}

	c->lastdigit = return_max(a->lastdigit, b->lastdigit) + 1;
	carry = 0;

	for (i = 0; i <= (c->lastdigit); i++) {
		c->digit[i] = (unsigned char)(carry + a->digit[i] + b->digit[i]) % base;
		carry = (carry + a->digit[i] + b->digit[i]) / base;
	}

	clear(c);
}


sub_nums(BigNum *a, BigNum *b, BigNum *c)
{
	//borrowing purpose
	int borrow;
	int v;
	int i;

	make_bignum(0, c);

	if ((a->signbit == NEGATIVE) || (b->signbit == NEGATIVE)) {
		b->signbit = -1 * b->signbit;
		add_nums(a, b, c);
		b->signbit = -1 * b->signbit;
		return;
	}

	if (comp(a, b) == POSITIVE) {
		sub_nums(b, a, c);
		c->signbit = NEGATIVE;
		return;
	}

	c->lastdigit = return_max(a->lastdigit, b->lastdigit);
	borrow = 0;

	for (i = 0; i <= (c->lastdigit); i++) {
		v = (a->digit[i] - borrow - b->digit[i]);
		if (a->digit[i] > 0)
			borrow = 0;
		if (v < 0) {
			v = v + base;
			borrow = 1;
		}

		c->digit[i] = (unsigned char)v % base;
	}

	clear(c);
}

comp(BigNum *a, BigNum *b)
{
	int i;

	if ((a->signbit == NEGATIVE) && (b->signbit == POSITIVE)) return(POSITIVE);
	if ((a->signbit == POSITIVE) && (b->signbit == NEGATIVE)) return(NEGATIVE);

	if (b->lastdigit > a->lastdigit) return (POSITIVE * a->signbit);
	if (a->lastdigit > b->lastdigit) return (NEGATIVE * a->signbit);

	for (i = a->lastdigit; i >= 0; i--) {
		if (a->digit[i] > b->digit[i]) return(NEGATIVE * a->signbit);
		if (b->digit[i] > a->digit[i]) return(POSITIVE * a->signbit);
	}

	return(0);
}

mul(BigNum *a, BigNum *b, BigNum *c)
{
	int i, j;

	make_bignum(0, c);
	make_bignum(0, &t1);
	make_bignum(0, &t2);

	t1 = *a;

	for (i = 0; i <= b->lastdigit; i++) {
		for (j = 1; j <= b->digit[i]; j++) {
			add_nums(c, &t1, &t2);
			*c = t2;
		}
		shift(&t1, 1);
	}

	c->signbit = a->signbit * b->signbit;

	clear(c);
}


div(BigNum *a, BigNum *b, BigNum *c, int mod)
{
	//local signs
	int asign, bsign;
	int i, j;

	make_bignum(0, c);

	c->signbit = a->signbit * b->signbit;

	asign = a->signbit;
	bsign = b->signbit;

	a->signbit = POSITIVE;
	b->signbit = POSITIVE;

	make_bignum(0, &t1);
	make_bignum(0, &t2);

	c->lastdigit = a->lastdigit;

	for (i = a->lastdigit; i >= 0; i--) {
		shift(&t1, 1);
		t1.digit[0] = a->digit[i];
		c->digit[i] = 0;
		while (comp(&t1, b) != POSITIVE) {
			c->digit[i] ++;
			sub_nums(&t1, b, &t2);
			t1 = t2;
		}
	}

	clear(c);

	a->signbit = asign;
	b->signbit = bsign;

	if (mod == 1)
	{
		make_bignum(0, &t3);
		mul(b, c, &t3);
		make_bignum(0, c);
		sub_nums(a, &t3, c);
	}
}

void copy(BigNum &c, BigNum &a)
{
    long int i;
}

//still buggy
pow(BigNum *a, BigNum *b, BigNum *c)
{
	make_bignum(0, c);
	//make_bignum(0, &t1);
	make_bignum(0, &t2);

	make_bignum(1, &t1);
	add_nums(&t2, a, &t3);
	for (; b->lastdigit != 0 && b->digit != 0; sub_nums(b, &t1, &t2))
	{
		*b = t2;
		make_bignum(0, &t2);
		mul(a, &t3, &t2);
		copy(&t2, c);
	}
}

void main(int argc, char* argv[])
{
	char file1[32], file2[32], oper, rfile[32], modFile[32];
	//base 10 for txt files
	base = 10;
	int mod = 0;

	if (argc < 5)
	{
		printf("Not enough arguments on input!");
		return 0;
	}

	strcpy(file1, argv[1]);
	oper = argv[2][0];
	strcpy(file2, argv[3]);
	strcpy(rfile, argv[4]);
	if (argc > 5)
	{
		if (argv[5][0] == '-' && argv[5][1] == 'b')
		{
		    //binary base
			base = 256;
			if (argc > 6)
			{
				strcpy(modFile, argv[6]);
				mod = 1;
			}
		}
		else
		{
			strcpy(modFile, argv[5]);
			mod = 1;
		}
	}
	//reading functions
	get_from_file(&n1, file1);
	get_from_file(&n2, file2);

	if (oper == '+') 		add_nums(&n1, &n2, &n3);
	else if (oper == '-') 	sub_nums(&n1, &n2, &n3);
	else if (oper == '.') 	mul(&n1, &n2, &n3);
	else if (oper == '/') 	div(&n1, &n2, &n3, 0);
	else if (oper == '%')	pow(&n1, &n2, &n3);
	else{
			printf("Operator unidentified!");
			exit(0);
		}

    //if there was module
	if (mod)
	{
		make_bignum(0, &n1);;
		make_bignum(0, &n2);
		get_from_file(&n1, modFile);
		div(&n3, &n1, &n2, 1);
		write_to_file(&n2, rfile);
	}
	else
	{
		write_to_file(&n3, rfile);
	}

	return 0;
}

get_from_file(BigNum *n, char * file)
{
	int i;
	FILE * f = NULL;
	char mode[4];
	//defining modes for reading
	if (base == 10)
	{
		strcpy(mode, "r");
	}
	else
	{
		strcpy(mode, "rb");
	}
	f = fopen(file, mode);
	//if failed to open
	if (f == NULL)
	{
		printf("Error occurred during file opening!\n");
		exit(0);
	}
	for (i = 0; !feof(f); i++)
	{
		//if actual number
		if (base == 10)
		{
			n->digit[i] = fgetc(f) - 48;
			if (i == 0 && n->digit[i] == '-')
			{
				n->signbit = NEGATIVE;
			}
		}
		//if binary
		else
		{
			n->digit[i] = (unsigned char)fgetc(f);
		}
	}
	n->lastdigit = i - 2;
	n->digit[i - 1] = 0;
	for (i = n->lastdigit; i > n->lastdigit / 2; i--)
	{
		n->digit[i] ^= n->digit[n->lastdigit - i] ^= n->digit[i] ^= n->digit[n->lastdigit - i];
	}
	fclose(f);
}

write_to_file(BigNum *n, char* file)
{
	//define mode for writing
	char mode[4];
	FILE * f = NULL;;
	if (base == 10)
	{
		strcpy(mode, "w");
	}
	else
	{
		strcpy(mode, "wb+\0");
	}
	f = fopen(file, mode);
	//error in writing
	if (f == NULL)
	{
		printf("Error occurred while writing!\n");
		exit(0);
	}
	for (int i = n->lastdigit; i >= 0; i--)
	{
		if (base == 10)
		{
			fputc(n->digit[i] + '0', f);
		}
		else
		{
			fputc(n->digit[i], f);
		}
	}
	fclose(f);
}
