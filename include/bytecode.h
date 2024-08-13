#ifndef BYTECODE_H
#define BYTECODE_H

#include "../include/instruction.h"
#include "../include/utilities.h"
#include "../include/sulfur.h"


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

enum OPCODES {
  	OPC_op = 0xFF,
    OPC_push_str = 0,
    OPC_push_var = 7,
    OPC_push_global_var = 8,
    OPC_push_1b = 2,
    OPC_push_0b = 1,
    OPC_push_nil = 3,
    OPC_push_i = 4,
    OPC_push_ount = 5,
    OPC_push_floap = 6,
    OPC_pop = 9,
    OPC_op_add = 40,
    OPC_op_sub = 41,
    OPC_op_mul = 42,
    OPC_op_div = 43,
    OPC_op_pow = 44,
    OPC_op_fldiv = 45,
    OPC_op_mod = 46,
    OPC_op_eq = 47,
    OPC_op_ge = 48,
    OPC_op_geq = 49,
    OPC_op_le = 50,
    OPC_op_leq = 51,
    OPC_op_lshift = 52,
    OPC_op_rshift = 53,
    OPC_op_noteq = 54,
    OPC_op_negate = 55,
    OPC_op_not = 56,
    OPC_op_unpack = 57,
    OPC_rjmp = 10,
    OPC_rjmpif = 11,
    OPC_rjmpifn = 12,
    OPC_dot_get = 15,
    OPC_dot_set = 13,
    OPC_col_get = 16,
    OPC_col_set = 14,
    OPC_var_set = 16,
    OPC_global_var_set = 17,
    OPC_prepare_fcall = 18,
    OPC_fcall = 19,
    OPC_fret = 20,
    OPC_ret = 21,
    OPC_call = 22,
    OPC_var_inc = 23,
    OPC_var_dec = 25,
    OPC_global_var_inc = 24,
    OPC_global_var_dec = 26,
    OPC_dot_inc = 27,
    OPC_dot_dec = 29,
    OPC_col_inc = 28,
    OPC_col_dec = 30,
    OPC_nop = 0b10101010,
    OPC_dup = 31,
    OPC_for = 32,
    OPC_fstart_def = 33,
    OPC_fset_mod_fix = 34,
    OPC_fset_mod_pack = 35,
    OPC_fset_param_len = 36,
    OPC_fadd_param = 37,
    OPC_fset_name = 38,
    OPC_fend_de = 39
};

typedef struct {
	unsigned char *data; // address of the data
	int data_len; // len of the data in Bytes
}extra_data_t;

typedef struct {
	int key_len; // number of key/values
	char **keys; // array of the key addresses
	extra_data_t *values;
} bytecode_extra_data;



typedef struct {
	uti_Bytes byte_code;
	unsigned char *strings; // adress of strings just after the length
	unsigned char *var_left; // same
	unsigned char *var_right; // same
	int strings_len;
	int var_left_len;
	int var_right_len;
	unsigned char *code; // addres of the code to execute
	char file_type;
	char *date_time; // address of the time as strs (can be FAKE_TIME)
	bytecode_extra_data extra_data;

} bytecode_info;


#endif