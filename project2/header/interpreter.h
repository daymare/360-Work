#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

#include "dataStructures.h"

#include "commands/cd.h"
#include "commands/ls.h"
#include "commands/pwd.h"


void runCommands(Pipe *commands);


#endif
