#ifndef BYTECODE_CONVERTER_H
#define BYTECODE_CONVERTER_H


/*
files start with 0x53 0x75

instruction separator 0x1
strings are ended by 0x0

for{
    0x2 var_loop_name start end
}
endfor{
    0x3 8 bytes for the for index
}
while{
    0x4 condition
}
endwhile{
    0x5 8 bytes for the while index
}


*/

typedef unsigned char Byte_su_t;


typedef struct Bytecode_t{
    Byte_su_t* bytes;
    int len;

}Bytecode_t;

#include "../include/bytecode_converter.h"
#include "../include/instruction.h"
#include <stdarg.h>


Bytecode_t *bytecode_append_char(Bytecode_t *code, int n, ...);

Bytecode_t *bytecode_append_llint(Bytecode_t *code, int n, ...);

Bytecode_t Ast_to_bytecode(Ast x);

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n);

void free_bytecode(Bytecode_t x);

Bytecode_t Constants_to_bytecode(Object *argv, int argc);

#endif