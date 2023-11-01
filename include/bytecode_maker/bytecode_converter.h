#ifndef BYTECODE_MAKER_BYTECODE_CONVERTER_H
#define BYTECODE_MAKER_BYTECODE_CONVERTER_H

typedef unsigned char Byte_su_t;


typedef struct Bytecode_t{
    Byte_su_t* bytes;
    int len;

}Bytecode_t;

#include "../instruction.h"

extern Object *constants;
extern S_sulfur_int constants_len;

extern Bytecode_t *asts;
extern S_sulfur_int asts_len;

extern Bytecode_t *insts_bc;
extern S_sulfur_int insts_bc_len;



Bytecode_t *bytecode_append_char(Bytecode_t *code, int n, ...);

Bytecode_t *bytecode_append_llint(Bytecode_t *code, S_sulfur_int value);

Bytecode_t Ast_to_bytecode(Ast x);

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n);

void free_bytecode(Bytecode_t x);

Bytecode_t new_bytecode();

Bytecode_t ast_to_bytecode(Ast x, S_sulfur_int *ast_len);

Bytecode_t Constants_to_bytecode(Object *argv, int argc);

Bytecode_t make_bytecode_inst(Instruction inst);

void bytecode_cat(Bytecode_t *dest, Bytecode_t source);

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n);

Bytecode_t make_bytecode(Instruction *inst, int len);

#endif