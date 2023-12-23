#ifndef BYTECODE_BYTECODE_H
#define BYTECODE_BYTECODE_H

#include "../instruction.h"

typedef unsigned char Byte_su_t;

typedef struct Bytecode_t{
    Byte_su_t* bytes;
    int len;
}Bytecode_t;

Bytecode_t *bytecode_append_char(Bytecode_t *code, int n, ...);

Bytecode_t *bytecode_append_llint(Bytecode_t *code, S_sulfur_int value);

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n);

void free_bytecode(Bytecode_t x);

Bytecode_t new_bytecode();

void bytecode_cat(Bytecode_t *dest, Bytecode_t source);

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n);

#endif