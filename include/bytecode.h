#ifndef BYTECODE_H
#define BYTECODE_H

#include "../include/instruction.h"
#include "../include/utilities.h"
#include "../include/sulfur.h"
#include "../include/bytecode.h"



extern char instruction_sizes[61];
extern char instruction_opcode[61];


#define BASE_INSTRUCTION	inst_S_op_t



int get_instruction_size(int type);

char get_opcode(int type);
/*
jmp +1
INST

-> jmp + 9
	INST

because jmp is of size 9
*/
void make_jumps_relative_to_size(Instruction *code, int len);

void bytes_append_str(uti_Bytes *bytes, char *str, char add_zero);

void bytes_append_u8(uti_Bytes *bytes, char val);

void bytes_append_u64(uti_Bytes *bytes, int val);

void bytes_set_u64_at(uti_Bytes *bytes, int val, int idx);
int get_str_arr_len(char **arr);

void relink_push_str(Instruction *code, int len, int *strs);

#define BYTECODE_MAGIC_NUM ".SU."
#define BYTECODE_FTYPE_EXE 0


uti_Bytes make_bytecode_file(Instruction *code, int len);
void bytes_append_code(uti_Bytes *res, Instruction *code, int len);

#endif