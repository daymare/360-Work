
#include <string.h>

#include <stdio.h>

int main()
{
    char testString[] = "something arg | command arg";

    char* t1 = strtok(testString, "|");
    char* t2 = strtok(0, "|");

    printf("%s", t1);
    printf("%s", t2);

}


