#include <stdio.h>
#include <string.h>


int *FP; // global pointer for the frame pointer


// print an address content pair with a label
void printMem(int* pointer)
{
	int address = (int)pointer;
	int content = (int)*pointer;

	printf("%x      %x\n", address, content);
}

void printNewline()
{
	printf("\n");
}


// print the stack frame linked list
void doProblem1()
{
	int* BFP = FP;
	int* AFP = (int*)*BFP;
	int* mainFP = (int*)*AFP;


	printf("\nStack Frame Linked List:\n");

	printMem("B frame pointer", BFP);
	printMem("A frame pointer", AFP);
	printMem("main frame pointer", mainFP);

	printNewline();
}


// credit: Brandon Kallaher
// better? Certainly simpler. hmm.
void doProblem1BrandonStyle()
{
	int* currentFP = FP;

	while(currentFP != 0)
	{
		printf("%x -> ", (int)currentFP);
		currentFP = (int*)*currentFP;
	}

	printf("0\n");
}

struct VariableList
{
	char **list;
	int size;
};

// print stack frame for a function
void printStackFrame(int* framePointer, char* functionName, VariableList params, VariableList locals)
{
	int* rPC = framePointer+1;
	int* local1 = framePointer-3;
	int* param1 = framePointer+2;

	printMem("RP", rPC);
	printMem("local", local1);
	printMem("param", param1);

	printNewline();
}

// print the hex contents of the whole stack
void doProblem2and3()
{
	char **Bvars = {"u", "v", "w"};
	char **Avars = {"d", "e", "f"};
	char **mainvars = {"a", "b", "c"};

	char **functParams = {"x", "y"};
	char **mainParams = {"argc", "argv", "env"};

	VariableList BVarsL;
	BVarsL.list = Bvars;
	BVarsL.size = 3;

	VariableList AllParamsL;
	AllParamsL.list = functParams;
	AllParamsL.size = 3;



	printStackFrame(FP, "B");
}

int B(int x, int y)
{
	int u,v,w;
	printf("enter B\n");
	u = 7; v = 8; w = 9;
	asm("movl %ebp, FP"); // set FP to CPUs ebp register

	doProblem1();

	doProblem2and3();

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

