#ifndef INOUT_H
#define INOUTH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"

char* read_prompt();


Object print_prompt(Object*obj,int n_arg);

Object println_prompt(Object*obj,int n_arg);

void init_inout(void) __attribute__((constructor));


#endif