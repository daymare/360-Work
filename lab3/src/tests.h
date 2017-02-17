#ifndef TESTS
#define TESTS

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "globals.h"
#include "parser.h"



void test_loadEnv()
{
    char* testEnv[] = {"HOME=/home/daylan/","PATH=/opt/ros/indigo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:./",  NULL};

    loadEnv(testEnv);

    if (strcmp(HOME, "/home/daylan/") != 0)
    {
        printf("loadenv: $HOME not extracted properly!");
    }

    if (strcmp(PATH[0], "/opt/ros/indigo/bin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[1], "/usr/local/sbin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[2], "/usr/local/bin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[3], "/usr/sbin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[4], "/usr/bin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[5], "/sbin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[6], "/bin") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[7], "/usr/games") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[8], "/usr/local/games") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[9], "./") != 0) printf("loadenv: $PATH not extracted properly!");
    if (strcmp(PATH[10], "") != 0) printf("loadenv: $PATH not terminated properly");

}

void test_processSingleCommand()
{
    Command result;
    char command[] = "ls /hello/testing > testing.txt";

    processSingleCommand(command, &result);

    printf("check stuff here!");
}




void runTests()
{
    printf("running tests!\n");

    test_loadEnv();
    test_processSingleCommand();


    printf("testing complete!\n");
}



#endif


