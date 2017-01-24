
#include <stdio.h>


// print char (given as putc)
// print string
void printString(char *string)
{
	while(*string != '\0')
	{
		putchar(*string);
		string++;
	}
}


// print uint
char *digitTable = "0123456789ABCDEF";
const int U_INT_BASE = 10;

void generalPrintIntHelper(unsigned int num, const int base)
{
	if (num > 0)
	{
		char c;
		c = digitTable[num % base];
		generalPrintIntHelper(num / base, base);
		putchar(c);
	}
}

// not used
// prints an unsigned integer to the console
void printUInt_Helper(unsigned int num)
{
	if (num > 0)
	{
		char c;
		c = digitTable[num % U_INT_BASE];
		printUInt_Helper(num / U_INT_BASE);
		putchar(c);
	}
}

void printUInt(unsigned int num)
{
	if (num == 0)
	{
		putchar('0');
	}else
	{
		generalPrintIntHelper(num, 10);
	}
}


// print int
void printInt(int num)
{
	if (num < 0)
	{
		putchar('-');
		num = 0 - num; // take the abs of num.
	}

	printUInt(num);
}

// print uoct
void printUOct(unsigned int num)
{
	printString("0o");
	if (num == 0)
	{
		putchar('0');
	}
	else
	{
		generalPrintIntHelper(num, 8);
	}
}


// print uhex
void printUHex(unsigned int num)
{
	printString("0x");
	if (num == 0)
	{
		putchar('0');
	}
	else
	{
		generalPrintIntHelper(num, 16);
	}
}


// printf
void myprintf(char *format, ...)
{
	char *currentChar = format; // first character in the format string
	int *currentVar = &format + 1; // first pointer to variable

	while(*currentChar != '\0')
	{
		// if normal character just print it out
		if (*currentChar != '%')
		{
			if (*currentChar == '\n')
			{
				putchar('\r');
			}

			putchar(*currentChar);
		}
		else
		{
			// handle % for variables
			currentChar++;
			switch(*currentChar)
			{
				case 'c':
					putchar(*currentVar);
					break;
				case 's':
					printString(*currentVar);
					break;
				case 'u':
					printUInt(*currentVar);
					break;
				case 'd':
					printInt(*currentVar);
					break;
				case 'o':
					printUOct(*currentVar);
					break;
				case 'x':
					printUHex(*currentVar);
					break;
			}

			currentVar++;
		}

		currentChar++;
	}
}


///////////////////////////////////////// problem 3


typedef struct node
{
	struct node *next;
	char name[64];
	int priority;
}Node;

Node *readyQueue = NULL;

int enqueue(Node **





int main(int argc, char* argv, char* env)
{
	// test print uint
	printUInt(891784);
	putchar('\n');

	// test print string
	printString("testing! Hello world!");
	putchar('\n');

	// test print integer
	printInt(-93839);
	putchar('\n');

	// test print positive integer
	printInt(64823);
	putchar('\n');
	putchar('\n');

	// test print octal
	printUOct(842799);
	putchar('\n');
	printString("correct octal: 0o3156057\n");

	// test print hex
	printUHex(842799);
	putchar('\n');
	printString("correct hex: 0xCDC2F\n");

	// test printf basic
	myprintf("testing! %d %d \n", 12345, 12);

	// test printf EVERYTHING
	myprintf("more testing! %c, %s, %u, %d, %o, %x\n", 'c', "test string!", 123456, -1234, 842799, 842799);

}


