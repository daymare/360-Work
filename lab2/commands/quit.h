#ifndef QUIT
#define QUIT


#include <stdlib.h>

#include "save.h"

int quit(char* path)
{
	// save to default directory
	save("default.fs");
	exit(0);
}


#endif
