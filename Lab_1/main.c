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
	short digit_mass[CAP]; //the number
	int Signum;	  // + or -
	int Last;   //index of most significant bit, most left-positioned
} BigNum;

BigNum temp1, temp2, temp3, temp4, temp5;
BigNum n1, n2, n3;

//read and write functions
get_from_file(BigNum *n, char * file);
write_to_file(BigNum *n, char* file);

//math happens
make_bignum(int s, BigNum *n)
{

	int i;
	int temp;

	if (s >= 0) n->Signum = POSITIVE;
	else n->Signum = NEGATIVE;

	for (i = 0; i<CAP; i++) n->digit_mass[i] = (unsigned char)0;

	n->Last = -1;

	temp = abs(s);

	while (temp > 0) {
		n->Last++;
		n->digit_mass[n->Last] = (temp % base);
		temp = temp / base;
	}

	if (s == 0) n->Last = 0;
}

int return_max(int a, int b)
{
	if (a > b) return(a); else return(b);
}

//number n gets multiplied by 10^d
shift(BigNum *n, int d)
{
	int i;

	if ((n->Last == 0) && (n->digit_mass[0] == 0)) return;

	for (i = n->Last; i >= 0; i--)
		n->digit_mass[i + d] = n->digit_mass[i];

	for (i = 0; i<d; i++) n->digit_mass[i] = 0;

	n->Last = n->Last + d;
}

//remove zeros
clear(BigNum *n)
{
	while ((n->Last > 0) && (n->digit_mass[n->Last] == 0))
		n->Last--;
	//clearing "-0"
	if ((n->Last == 0) && (n->digit_mass[0] == 0))
		n->Signum = POSITIVE;
}

add_nums(BigNum *a, BigNum *b, BigNum *c)
{
	int carry;
	int i;

	make_bignum(0, c);

	if (a->Signum == b->Signum) c->Signum = a->Signum;
	else {
		if (a->Signum == NEGATIVE) {
			a->Signum = POSITIVE;
			sub_nums(b, a, c);
			a->Signum = NEGATIVE;
		}
		else {
			b->Signum = POSITIVE;
			sub_nums(a, b, c);
			b->Signum = NEGATIVE;
		}
		return;
	}

	c->Last = return_max(a->Last, b->Last) + 1;
	carry = 0;

	for (i = 0; i <= (c->Last); i++) {
		c->digit_mass[i] = (unsigned char)(carry + a->digit_mass[i] + b->digit_mass[i]) % base;
		carry = (carry + a->digit_mass[i] + b->digit_mass[i]) / base;
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

	if ((a->Signum == NEGATIVE) || (b->Signum == NEGATIVE)) {
		b->Signum = -1 * b->Signum;
		add_nums(a, b, c);
		b->Signum = -1 * b->Signum;
		return;
	}

	if (comp(a, b) == POSITIVE) {
		sub_nums(b, a, c);
		c->Signum = NEGATIVE;
		return;
	}

	c->Last = return_max(a->Last, b->Last);
	borrow = 0;

	for (i = 0; i <= (c->Last); i++) {
		v = (a->digit_mass[i] - borrow - b->digit_mass[i]);
		if (a->digit_mass[i] > 0)
			borrow = 0;
		if (v < 0) {
			v = v + base;
			borrow = 1;
		}

		c->digit_mass[i] = (unsigned char)v % base;
	}

	clear(c);
}

comp(BigNum *a, BigNum *b)
{
	int i;

	if ((a->Signum == NEGATIVE) && (b->Signum == POSITIVE)) return(POSITIVE);
	if ((a->Signum == POSITIVE) && (b->Signum == NEGATIVE)) return(NEGATIVE);

	if (b->Last > a->Last) return (POSITIVE * a->Signum);
	if (a->Last > b->Last) return (NEGATIVE * a->Signum);

	for (i = a->Last; i >= 0; i--) {
		if (a->digit_mass[i] > b->digit_mass[i]) return(NEGATIVE * a->Signum);
		if (b->digit_mass[i] > a->digit_mass[i]) return(POSITIVE * a->Signum);
	}

	return(0);
}

mul(BigNum *a, BigNum *b, BigNum *c)
{
	int i, j;

	make_bignum(0, c);
	make_bignum(0, &temp1);
	make_bignum(0, &temp2);

	temp1 = *a;

	for (i = 0; i <= b->Last; i++) {
		for (j = 1; j <= b->digit_mass[i]; j++) {
			add_nums(c, &temp1, &temp2);
			*c = temp2;
		}
		shift(&temp1, 1);
	}

	c->Signum = a->Signum * b->Signum;

	clear(c);
}


div(BigNum *a, BigNum *b, BigNum *c, int mod)
{
	//local signs
	int asign, bsign;
	int i, j;

	make_bignum(0, c);

	c->Signum = a->Signum * b->Signum;

	asign = a->Signum;
	bsign = b->Signum;

	a->Signum = POSITIVE;
	b->Signum = POSITIVE;

	make_bignum(0, &temp1);
	make_bignum(0, &temp2);

	c->Last = a->Last;

	for (i = a->Last; i >= 0; i--) {
		shift(&temp1, 1);
		temp1.digit_mass[0] = a->digit_mass[i];
		c->digit_mass[i] = 0;
		while (comp(&temp1, b) != POSITIVE) {
			c->digit_mass[i] ++;
			sub_nums(&temp1, b, &temp2);
			temp1 = temp2;
		}
	}

	clear(c);

	a->Signum = asign;
	b->Signum = bsign;

	if (mod == 1)
	{
		make_bignum(0, &temp3);
		mul(b, c, &temp3);
		make_bignum(0, c);
		sub_nums(a, &temp3, c);
	}
}

copy(BigNum *a, BigNum *b)
{
  int i;
  make_bignum(0, a);
  a->Last = b->Last;
  a->Signum = b->Signum;
  for (i = 0; i <= b->Last; i++)
  {
    a->digit_mass[i] = b->digit_mass[i];
  }
  clear(a);
}

pow(BigNum *a, BigNum *b, BigNum *c)
{
	make_bignum(0, c);
	make_bignum(0, &temp2);
    make_bignum(0, &temp4);
    make_bignum(0, &temp3);
	make_bignum(1, &temp1);
	//if (b->digit_mass[b->Last] % 2 == 0)
	//c->Signum = POSITIVE;
	//add_nums(&temp4, a, &temp3);
	copy(&temp3, a);
    sub_nums(b, &temp1, &temp2);
    copy(b, &temp2);
	while ((b->Last != 0) || (b->digit_mass[0] != 0))
	{
	    sub_nums(b, &temp1, &temp2);
		copy(b, &temp2);
		make_bignum(0, &temp2);
		mul(a, &temp3, &temp4);
		copy(&temp3, &temp4);
		make_bignum(0, &temp2);
        make_bignum(1, &temp1);
	}
	copy(c, &temp3);
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
	printf(file);
	printf(mode);
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
			n->digit_mass[i] = fgetc(f) - 48;
			if (i == 0 && n->digit_mass[i] == '-')
			{
				n->Signum = NEGATIVE;
			}
		}
		//if binary
		else
		{
			n->digit_mass[i] = (unsigned char)fgetc(f);
		}
	}
	n->Last = i - 2;
	n->digit_mass[i - 1] = 0;
	for (i = n->Last; i > n->Last / 2; i--)
	{
		n->digit_mass[i] ^= n->digit_mass[n->Last - i] ^= n->digit_mass[i] ^= n->digit_mass[n->Last - i];
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
		strcpy(mode, "wb+");
	}
	f = fopen(file, mode);
	//error in writing
	if (f == NULL)
	{
		printf("Error occurred while writing!\n");
		exit(0);
	}
	for (int i = n->Last; i >= 0; i--)
	{
		if (base == 10)
		{
			fputc(n->digit_mass[i] + '0', f);
		}
		else
		{
			fputc(n->digit_mass[i], f);
		}
	}
	fclose(f);
}
