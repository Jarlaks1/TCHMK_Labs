#include "BigInt.h"

int main(int argc, char* argv[])
{
	BigInt a, b, c;
	int Basis = 10;
	char file1[32], file2[32], oper, result_file[32], modFile[32];
	int mod = 0;

	if (argc < 5)	{
		printf("Not enough arguments");
		return 0;}

	strcpy(file1, argv[1]);
	oper = argv[2][0];
	strcpy(file2, argv[3]);
	strcpy(result_file, argv[4]);
	if (argc > 5)	{
		if (argv[5][0] == '-' && argv[5][1] == 'b'){
			Basis = 256;
			if (argc > 6)	{
				strcpy(modFile, argv[6]);
				mod = 1;
			}
		}
		else	{
			strcpy(modFile, argv[5]);
			mod = 1;
		}
	}

	a.setbase(Basis);
	b.setbase(Basis);
	a.scan(file1);
	b.scan(file2);

	switch (oper)
	{
	case '+':
	{
				c = a + b;
				break;
	}
	case '-':
	{
				c = a - b;
				break;
	}
	case '.':
	{
				c = a * b;
				break;
	}
	case '/':
	{
				c = a / b;
				break;
	}
	case '^':
	{
				c = a ^ b;
				break;
	}
	case '%':
	{
				if (b.signum == NEGATIVE)
				{
					printf("Wrong b signfor pow operation");
					return 0;
				}
				c = a % b;
				break;
	}
	default:
	{
			   printf("Wrong operator");
			   exit(0);
			   break;
	}
	}

	if (mod)
	{
		BigInt m;
		m.setbase(Basis);
		m.scan(modFile);
		c = c % m;
	}
	c.print(result_file);
	return 0;
}