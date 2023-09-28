#ifndef STRING_SU_H
#define STRING_SU_H
#include "../../include/memlib.h"

extern Module string_module;
extern char * string_methods;

Object init_string(memory *MEMORY,char*path);

#endif