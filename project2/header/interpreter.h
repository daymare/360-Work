#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

#include "dataStructures.h"

#include "commands/cd.h"
#include "commands/ls.h"
#include "commands/pwd.h"
#include "commands/unlink.h"
#include "commands/mkdir.h"


void runCommands(Pipe *commands);


#endif
