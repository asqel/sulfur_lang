#ifndef BYTECODE_MAKER_MAKE_CONSTANTS_H
#define BYTECODE_MAKER_MAKE_CONSTANTS_H
#include "../memlib.h"

extern Object *constants;
extern S_sulfur_int constants_len;

Object *new_constants(S_sulfur_int *len);

int add_constants(Object x);

int add_constant_str(char *s);

int add_constant_int(S_ount_t value);

int add_constant_float(S_floap_t value);

#endif