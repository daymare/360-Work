#ifndef INTERPRETER
#define INTERPRETER


#include <unistd.h>
#include <fcntl.h>

#include "dataStructures.h"


//TODO handle more complicated cases
int runCommands(Pipe* commands)
{
    if (strcmp(commands->command.tokenizedCommand[0], "exit") == 0)
    {
        exit(0);
    }

    int pid = fork();

    // fork to save shell program
    if (pid)
    {
       int status;
       pid = wait(&status); 
    }
    else
    {
        // execute the pipe

        // check if the pipe has only one element
        if (commands->leftPipe == NULL)
        {
            // simply run the given command
            runCommand(commands->command);
        }
        else
        {
            // execute multiple items in the pipe
            rRunCommands(commands);
        }
    }
}

// last 2 items in the pipe need to be forked together

// run commands in the pipe recursively
int rRunCommands(Pipe* commands)
{
    int pd[2];
    int pid;

    // pipe must have at least 2 commands.
    if (commands->leftPipe->leftPipe == NULL)
    {
        // there are exactly 2 commands in the pipe.
        // fork both and stop
        pipe(pd);

        pid = fork();

        if (pid)
        {
            // parent as pipe reader
            close(pd[1]);
            close(0);
            dup(pd[0]);

            // execute right command
            runCommand(commands->command);
        }
        else
        {
            // child as pipe writer
            close(pd[0]);

            close(1);
            dup(pd[1]);
            
            // execute left command
            runCommand(commands->leftPipe->command);
        }
    }
    else
    {
        // there are 3 or more commands in the pipe
        pipe(pd);

        pid = fork();

        if (pid)
        {
            // parent as pipe reader
            close(pd[1]);
            close(0);
            dup(pd[0]);

            // execute rightmost command
            runCommand(commands->command);
        }
        else
        {
            // child as pipe writer
            close(pd[0]);
            close(1);
            dup(pd[1]);
            
            // execute remaining left commands in pipe
            rRunCommands(commands->leftPipe);
        }
    }
}


int runCommand(Command command)
{
    char* argv[20];
    int returnCode = 0;
    int i;

    // set up argv
    for (i = 0; i < 20; i++)
    {
        if (strcmp(command.tokenizedCommand[i], "") == 0)
        {
            argv[i] = NULL;
        }
        else
        {
            argv[i] = command.tokenizedCommand[i];
        }
    }

    // set up IO redirects
    if (command.inRedirect.haveRedirect == 1)
    {
        close(0);
        open(command.inRedirect.filename, O_RDONLY);
    }

    if (command.outRedirect.haveRedirect ==1)
    {
        close(1);
        if (command.outRedirect.isAppend == 1)
        {
            open(command.outRedirect.filename, O_WRONLY|O_APPEND);
        }
        else
        {
            open(command.outRedirect.filename, O_WRONLY|O_CREAT);
        }
    }

    // check for trivial command
    if (isTrivialCommand(command))
    {
        runTrivialCommand(command);
    }
    else
    {
        // run non-trivial command
        returnCode = execvp(command.tokenizedCommand[0], argv);
        printf("%d", returnCode);
    }
}


#endif 
