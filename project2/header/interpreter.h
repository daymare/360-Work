#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

#include "dataStructures.h"

#include "commands/cd.h"
#include "commands/ls.h"
#include "commands/pwd.h"
#include "commands/unlink.h"
#include "commands/mkdir.h"
#include "commands/link.h"
#include "commands/open.h"
#include "commands/close.h"
#include "commands/rmdir.h"
#include "commands/pfd.h"
#include "commands/simlink.h"

void runCommands(Pipe *commands);


#endif
