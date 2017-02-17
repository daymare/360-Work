

#include <stdio.h>

int main(int argc, char** argv, char** env)
{
    char** currentV = env;

    while (*currentV != NULL)
    {
        printf("%s\n", *currentV);
        currentV++;
    }
}
