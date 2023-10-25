#ifndef MAKE_CONSTANTS_H
#define MAKE_CONSTANTS_H
#include "memlib.h"

extern Object *constants;
extern int constants_len; 


void init_constants();

int add_constants(Object x);

int add_constant_string(char *s);

#endif