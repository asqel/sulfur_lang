#ifndef BYTECODE_CONSTANTS_H
#define BYTECODE_CONSTANTS_H
#include "../memlib.h"

#include "../../include/instruction.h"
#include "../../include/Ast.h"
#include "../../include/bytecode/bytecode.h"
extern Object *constants;
extern S_sulfur_int constants_len;

void reset_constants();

S_sulfur_int add_constants(Object x);

S_sulfur_int add_constant_str(char *s);

S_sulfur_int add_constant_int(S_ount_t value);

S_sulfur_int add_constant_float(S_floap_t value);


S_sulfur_int get_constants(Object x);

S_sulfur_int get_constant_str(char *s);

S_sulfur_int get_constant_int(S_ount_t value);

S_sulfur_int get_constant_float(S_floap_t value);

void ast_to_constants(Ast x);

void inst_to_constants(Instruction *inst, int len);

void constants_to_bytecode(Bytecode_t *code);

#endif