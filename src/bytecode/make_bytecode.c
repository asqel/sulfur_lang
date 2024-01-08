#include "../../include/bytecode/bytecode.h"
#include "../../include/bytecode/op_codes.h"
#include "../../include/bytecode/constants.h"


Bytecode_t make_bytecode(Instruction *inst, int len) {
	//reset_constants();
	//inst_to_constants(inst, len);
//
	//Bytecode_t res = new_bytecode();
	//bytecode_add_char_arr(&res, MAGICAL_NUMBER_AS_STR, 4);
	//bytecode_append_char(&res, 8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
//
	//constants_to_bytecode(&res);
	//S_sulfur_int first_inst_addr = res.len;
	//memcpy(&(res.bytes[4]), &first_inst_addr, 8);
//
	//insts_to_bytecode(&res, inst, len);
	//bytecode_append_char(&res, 1, OPC_END_CODE);
//
	//reset_constants();
	//return res;
}

void insts_to_bytecode(Bytecode_t *code, Instruction *inst, int len) {

	//if (instructions_addr)
	//	free(instructions_addr);
	//instructions_addr = NULL;
	//instructions_addr_len = 0;
//
	//instructions_addr = malloc(sizeof(S_sulfur_int) * len);
	//instructions_addr_len = len;
	//for(int i = 0; i < len; i++) {
	//	inst_to_bytecode(code, inst, len, &i);
	//}

}

S_sulfur_int *instructions_addr = NULL;
int instructions_addr_len = 0;

void inst_to_bytecode(Bytecode_t *code, Instruction *inst, int len, int *p) {
	//if (inst[*p].type == inst_expr_t) {
	//	instructions_addr[*p] = code->len;
	//	ast_to_bytecode(code, inst[*p].value.expr);
	//	bytecode_append_char(code, 1, OPC_POP);
	//	return;
	//}
	//if (inst[*p].type == inst_return_t) {
	//	instructions_addr[*p] = code->len;
	//	ast_to_bytecode(code, inst[*p].value.expr);
	//	bytecode_append_char(code, 1, OPC_RET_FUNC);
	//	return ;
	//}
	//if (inst[*p].type == inst_section_t) {
	//	instructions_addr[*p] = code->len;
	//	bytecode_append_char(code, 1, OPC_PASS);
	//	return;
	//}
	//if (inst[*p].type == inst_goto_t) {
	//	char *jump_to = inst[*p].value.goto_sec;
	//	int n = -1;
    //	//search down
    //	for(int i = *p + 1; i < len; i++){
    //	    if(inst[i].type == inst_section_t){
    //	        if(!strcmp(jump_to, inst[i].value.section)){
    //	            n = i;
    //	            break;
    //	        }
    //	    }
    //	}
    //	if(n == -1){
    //	    //search up
    //	    for(int i = *p - 1; i >= 0; i--){
    //	        if(inst[i].type == inst_section_t){
    //	            if(!strcmp(jump_to, inst[i].value.section)){
    //	                n = i;
    //	                break;
    //	            }
    //	        }
    //	    }
    //	}
//
	//	if (n == -1) {
	//		printf("ERROR while generating byetcode cannot goto section '%s' because it does not exists\n");
	//		exit(1);
	//	}
	//	int goto_idx = *p;
	//	bytecode_append_char(code, 1, OPC_JMP);
	//	bytecode_append_llint(code, 0xFFFFFFFFFFFFFFFF);
	//	(*p)++;
	//	while (*p <= n) {
	//		inst_to_bytecode(code, inst, len, p);
	//		(*p)++;
	//	}
	//	S_sulfur_int goto_addr = instructions_addr[goto_idx];
	//	memcpy(&code->bytes[goto_addr + 1], &instructions_addr[n], 8);
	//	return ;
//
	//}
	//if (inst[*p].type == inst_while_t) {
	//	/*	0 | push condition
	//		1 | jump_ifn 5
	//		2 | ...
	//		3 | push condition
	//		4 | jump_if 2
	//		5 |
	//	*/ 
	//	ast_to_bytecode(code, inst[*p].value.wh->condition);
	//	instructions_addr[*p] = code->len;
	//	bytecode_append_char(code, OPC_JMP_IFN);
	//	bytecode_append_llint(code, 0xFFFFFFFFFFFFFFFF);
	//	return ;
	//}
	//if (inst[*p].type == inst_while_t) {
	//	S_sulfur_int while_addr = instructions_addr[inst[*p].value.endwhile];
	//	ast_to_bytecode(code, inst[while_addr].value.wh->condition);
	//	instructions_addr[*p] = code->len;
	//	bytecode_append_char(code, OPC_JMP_IF);
	//	bytecode_append_llint(code, &while_addr + 9);
	//	memcpy(&code->bytes[while_addr + 1], &code->len, 8);
	//	return ;
	//}
	//if (inst[*p].type == inst_for_t) {
	//	// 0  |  push a
	//	// 1  |  dup
	//	// 2  |  push var_name
	//	// 3  |  assign
	//	// 4  |  pop ;pop the old value of the iterator
	//	instructions_addr[*p] =code->len;
	//}
}