

#include "trivialCommands.h"
#include "globals.h"
#include "dataStructures.h"
#include "support.h"
#include "color.h"
#include "tests.h"
#include "parser.h"
#include "interpreter.h"


char input[128];
Pipe* commands;


int main(int argc, char** argv, char** env)
{
    // run tests
    runTests();

    // load relevant env into globals
    loadEnv(env);

    while (1)
    {
        // TODO print out filepath based command prompt.
        // get command input
        printf(ANSI_COLOR_CYAN "Dash $ " ANSI_COLOR_RESET);
        memset(input, 0, 128);
        fgets(input, 128, stdin);
        input[strlen(input)-1] = '\0';

        // parse input
        parseCommands(input, &commands);

        // run commands
        runCommands(commands);

        // free commands before next input
        freePipe(commands);
    }

    printf("\n");// so when we exit our shell the actual shell comes up in the right place.

    return 0;
}



