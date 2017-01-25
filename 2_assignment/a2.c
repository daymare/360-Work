
#include <stdio.h>
#include <stdlib.h>

#include <string.h>


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

// test all of the print functions
void testPrints()
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

	// test printf everything
	myprintf("more testing! %c, %s, %u, %d, %o, %x\n", 'c', "test string!", 123456, -1234, 842799, 842799);
}


///////////////////////////////////////// problem 3


typedef struct node
{
	struct node *next;
	char name[64];
	int priority;
}Node;

Node *readyQueue = NULL;

// enqueue a node in priority then FIFO order
int enqueue(Node **queue, Node *p)
{
	// case for empty list
	if (*queue == NULL)
	{
		*queue = p;
		p->next = NULL;
		return 0;
	}

	// case for first element in list
	if (p->priority > (*queue)->priority)
	{
		p->next = *queue;
		*queue = p;
		return 0;
	}

	// insert in nonempty list
	Node* current = *queue;
	Node* next = current->next;
	while (next != NULL)
	{
		if (p->priority > next->priority)
		{
			current->next = p;
			p->next = next;
			return 0;
		}
		current = next;
		next = next->next;
	}

	// reached end of list, just insert at end
	p->next = NULL;
	current->next = p;
}


// dequeue a node from the priority queue.
Node *dequeue(Node **queue)
{
	// take out the first node in the list.
	Node *result = *queue;
	*queue = result->next;
	result->next = NULL;

	return result;
}

void printMainArgs(int argc, char* argv[], char* env[])
{
	// print argv strings
	char *currentString = NULL;
	int i = 0;
	for (i = 0; i < argc; i++)
	{
		currentString = argv[i];
		myprintf("%s,   ", currentString);
	}

	myprintf("\n\n");

	i = 0;
	currentString = env[i];

	while (currentString != NULL)
	{
		myprintf("%s,   ", currentString);
		i++;
		currentString = env[i];
	}

	myprintf("\n\n");
}

Node * mallocNode()
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->priority = 0;
	return newNode;
}

void fillNode(Node * node, int i)
{
	sprintf(node->name, "node%d", i);
	node->priority = rand() % 10;
}


void printNode(Node * node)
{
	myprintf("%s, p: %d -->", node->name, node->priority);
}

void printQueue(Node * head)
{
	while(head != NULL)
	{
		printNode(head);
		head = head->next;
	}

	putchar('\n');
}



int main(int argc, char* argv[], char* env[])
{
	testPrints();

	printMainArgs(argc, argv, env);

	// infinite enqueue loop
	int i = 0;
	Node *currentNode = NULL;
	Node *queue = NULL;
	while(1)
	{
		// add a new node to queue and print
		currentNode = mallocNode();
		fillNode(currentNode, i);
		enqueue(&queue, currentNode);
		printQueue(queue);

		i++;
		
		// wait for the user to press any key
		fflush(stdin);
		getchar();
	}
}


