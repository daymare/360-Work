#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

#include <stdbool.h>


typedef struct ioRedirect
{
    int haveRedirect;
    int isAppend;
    char filename[128];
} IORedirect;

typedef struct command
{
    char tokenizedCommand[10][64];
    IORedirect inRedirect;
    IORedirect outRedirect;
} Command;

typedef struct pipe Pipe;

typedef struct pipe
{
    Command command;
    Pipe* leftPipe;
} Pipe;

Pipe* allocPipe()
{
    Pipe* result = malloc(sizeof(Pipe));
    memset(&(result->command), 0, sizeof(Command));
    result->leftPipe = NULL;
}
void freePipe(Pipe* pipe)
{
    if (pipe->leftPipe != NULL)
    {
        freePipe(pipe->leftPipe);
        pipe->leftPipe = NULL;
    }

    free(pipe);
}


#endif



