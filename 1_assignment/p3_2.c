#include <stdio.h>
#include <string.h>


int *FP; // global poitner to the frame pointer for B

void doProblem1()
{
	int* currentFP = FP;

	printf("\n\nproblem 1:\n");

	while(currentFP != 0)
	{
		printf("%x -> ", (int)currentFP);
		currentFP = (int*)*currentFP;
	}

	printf("0\n");
}

void doProblem2()
{
	int i = 0;
	int *currentMem = NULL;

	printf("\n\nproblem 2:\n");

	for (i = -20; i < 60; i++)
	{
		currentMem = FP+i;
		printf("%x     %x\n", currentMem, *currentMem);
	}

	printf("\n\n");
}

int B(int x, int y)
{
	int u,v,w;
	printf("enter B\n");
	u = 7; v = 8; w = 9;
	asm("movl %ebp, FP"); // set FP to CPUs ebp register

	doProblem1();
	doProblem2();

	printf("exit B\n");
}

int A(int x, int y)
{
	int d,e,f;
	printf("enter A\n");
	d = 4; e = 5; f = 6;
	B(d,e);
	printf("exit A\n");
}

int main(int argc, char *argv[], char *env[])
{
	int a,b,c;
	printf("enter main\n");
	a = 1; b = 2; c = 3;

	A(a,b);
	printf("exit main\n");
}





