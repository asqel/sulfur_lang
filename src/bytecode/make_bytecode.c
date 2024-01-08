#include "../../include/bytecode/bytecode.h"
#include "../../include/bytecode/op_codes.h"
#include "../../include/bytecode/constants.h"
extern Bytecode_t *ast_to_bytecode(Bytecode_t *code, Ast x);

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

S_sulfur_int *instructions_addr = NULL;
int instructions_addr_len = 0;

int bc_get_const_addr_str(char * str);
int bc_get_const_addr(Object *x);

void inst_to_bytecode(Bytecode_t *code, Instruction *inst, int len, int *p) {
	if (inst[*p].type == inst_expr_t) {
		instructions_addr[*p] = code->len;
		ast_to_bytecode(code, *inst[*p].value.expr);
		bytecode_append_char(code, 1, OPC_POP);
		return;
	}
	if (inst[*p].type == inst_pass_t || inst[*p].type == inst_section_t || inst[*p].type == inst_else_t) {
		instructions_addr[*p] = code->len;
		return;
	}
	if (inst[*p].type == return_t) {
		instructions_addr[*p] = code->len;
		ast_to_bytecode(code, *inst[*p].value.ret);
		bytecode_append_char(code, 1, OPC_END_EXEC);
		return;
	}
	if (inst[*p].type == inst_if_t) {
		ast_to_bytecode(code, *inst[*p].value.i->condition);
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP_IFN);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst[*p].type == inst_elif_t) {
		ast_to_bytecode(code, *inst[*p].value.el->condition);
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP_IFN);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst[*p].type == inst_else_t) {
		instructions_addr[*p] = code->len;
		return;
	}
	if (inst[*p].type == inst_endif) {
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP_IF);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst[*p].type == inst_endifelse) {
		instructions_addr[*p] = code->len;
		return ;
	}
	if (inst[*p].type == inst_for_t) {
		ast_to_bytecode(code, *inst[*p].value.fo->start);
		bytecode_append_char(code, 1, OPC_DUP);
		bytecode_append_char(code, 1, OPC_PUSH);
		bytecode_append_llint(code, bc_get_const_addr_str(inst[*p].value.fo->var_name));
		bytecode_append_char(code, 1, OPC_PUSH_OUNT);
		bytecode_append_llint(code, hash_str(inst[*p].value.fo->var_name));
		bytecode_append_char(code, 1, OPC_ASSIGN);
		bytecode_append_char(code, 1, OPC_POP);
		ast_to_bytecode(code, *inst[*p].value.fo->end);
		bytecode_append_char(code, 1, OPC_EQ);
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP_IF);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst[*p].type == inst_endfor_t) {
		ast_to_bytecode(code, *inst[*p].value.fo->end);
		ast_to_bytecode(code, *inst[*p].value.fo->start);
		bytecode_append_char(code, 1, OPC_PUSH);

		bytecode_append_llint(code, bc_get_const_addr_str(inst[*p].value.fo->var_name));
		bytecode_append_char(code, 1, OPC_PUSH_OUNT);
		bytecode_append_llint(code, hash_str(inst[*p].value.fo->var_name));
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_FOR_JMP);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst_while_t == inst[*p].type) {
		ast_to_bytecode(code, *inst[*p].value.wh->condition);
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP_IFN);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst_endwhile_t == inst[*p].type) {
		ast_to_bytecode(code, *inst[inst[*p].value.endwhile].value.wh->condition);
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP_IF);
		bytecode_append_llint(code, 0);
		return;
	}
	if (inst[*p].type == inst_jmp_t) {
		instructions_addr[*p] = code->len;
		bytecode_append_char(code, 1, OPC_JMP);
		bytecode_append_llint(code, 0);
		return;
	}
	else {
		printf("ERROR cant convert instruction type %d to bytecode \n", inst[*p].type);
		exit(1);
	}
}

void bytecode_link_jmps(Bytecode_t *code, Instruction *inst, int len, S_sulfur_int *addr, int addr_len) {
	for(int i = 0; i < len; i++) {
		if (inst[i].type == inst_jmp_t) {
			if (inst[i].value.jmp < len) {
				int jump_to = addr[inst[i].value.jmp];
				memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
			}
			else {
				int jump_to = code->len;
				memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
			}
		}
		else if (inst[i].type == inst_if_t) {
			int jump_to = addr[inst[i].value.i->endif_p] + OPC_SIZE_JMP;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_elif_t) {
			int jump_to = addr[inst[i].value.el->endif_p] + OPC_SIZE_JMP;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_endif) {
			int jump_to = addr[inst[i].value.endif];
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_for_t) {
			int jump_to = addr[inst[i].value.fo->endfor] + OPC_SIZE_FOR_JMP;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_endfor_t) {
			int jump_to = addr[inst[i].value.endfor] + OPC_SIZE_JMP_IF;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_while_t) {
			int jump_to = addr[inst[i].value.wh->endwhile] + OPC_SIZE_JMP_IF;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_endwhile_t) {
			int jump_to = addr[inst[i].value.endwhile] + OPC_SIZE_JMP_IFN;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
	}
}

void insts_to_bytecode(Bytecode_t *code, Instruction *inst, int len) {

	instructions_addr = calloc(len, sizeof(S_sulfur_int));
	instructions_addr_len = len;
	for(int i = 0; i < len; i++) {
		inst_to_bytecode(code, inst, len, &i);
	}
	bytecode_link_jmps(code, inst, len, instructions_addr, instructions_addr_len);
	free(instructions_addr);
	instructions_addr = NULL;
	instructions_addr_len = 0;

}


void bytecode_from_ano_func(Bytecode_t *code, anonym_func *ano_func) {
	S_sulfur_int *instructions_addr = NULL;
	int instructions_addr_len = 0;
	Instruction *inst = ano_func->code;
	instructions_addr = malloc(sizeof(S_sulfur_int) * ano_func->code_len);
	instructions_addr_len = ano_func->code_len;
	for(int p = 0; p < ano_func->code_len; p++) {
		if (inst[p].type == inst_expr_t) {
			instructions_addr[p] = code->len;
			ast_to_bytecode(code, *inst[p].value.expr);
			bytecode_append_char(code, 1, OPC_POP);
			return;
		}
		if (inst[p].type == inst_pass_t || inst[p].type == inst_section_t || inst[p].type == inst_else_t) {
			instructions_addr[p] = code->len;
			return;
		}
		if (inst[p].type == return_t) {
			instructions_addr[p] = code->len;
			ast_to_bytecode(code, *inst[p].value.ret);
			bytecode_append_char(code, 1, OPC_RET);
			return;
		}
		if (inst[p].type == inst_if_t) {
			ast_to_bytecode(code, *inst[p].value.i->condition);
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP_IFN);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst[p].type == inst_elif_t) {
			ast_to_bytecode(code, *inst[p].value.el->condition);
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP_IFN);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst[p].type == inst_else_t) {
			instructions_addr[p] = code->len;
			return;
		}
		if (inst[p].type == inst_endif) {
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP_IF);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst[p].type == inst_endifelse) {
			instructions_addr[p] = code->len;
			return ;
		}
		if (inst[p].type == inst_for_t) {
			ast_to_bytecode(code, *inst[p].value.fo->start);
			bytecode_append_char(code, 1, OPC_DUP);
			bytecode_append_char(code, 1, OPC_PUSH);
			bytecode_append_llint(code, bc_get_const_addr_str(inst[p].value.fo->var_name));
			bytecode_append_char(code, 1, OPC_PUSH_OUNT);
			bytecode_append_llint(code, hash_str(inst[p].value.fo->var_name));
			bytecode_append_char(code, 1, OPC_ASSIGN);
			bytecode_append_char(code, 1, OPC_POP);
			ast_to_bytecode(code, *inst[p].value.fo->end);
			bytecode_append_char(code, 1, OPC_EQ);
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP_IF);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst[p].type == inst_endfor_t) {
			ast_to_bytecode(code, *inst[p].value.fo->end);
			ast_to_bytecode(code, *inst[p].value.fo->start);
			bytecode_append_char(code, 1, OPC_PUSH);

			bytecode_append_llint(code, bc_get_const_addr_str(inst[p].value.fo->var_name));
			bytecode_append_char(code, 1, OPC_PUSH_OUNT);
			bytecode_append_llint(code, hash_str(inst[p].value.fo->var_name));
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_FOR_JMP);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst_while_t == inst[p].type) {
			ast_to_bytecode(code, *inst[p].value.wh->condition);
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP_IFN);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst_endwhile_t == inst[p].type) {
			ast_to_bytecode(code, *inst[inst[p].value.endwhile].value.wh->condition);
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP_IF);
			bytecode_append_llint(code, 0);
			return;
		}
		if (inst[p].type == inst_jmp_t) {
			instructions_addr[p] = code->len;
			bytecode_append_char(code, 1, OPC_JMP);
			bytecode_append_llint(code, 0);
			return;
		}
		else {
			printf("ERROR cant convert instruction type %d to bytecode \n", inst[p].type);
			exit(1);
		}
	}
	int len = ano_func->code_len;
	int *addr = instructions_addr;
	for(int i = 0; i < len; i++) {
		if (inst[i].type == inst_jmp_t) {
			if (inst[i].value.jmp < len) {
				int jump_to = addr[inst[i].value.jmp];
				memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
			}
			else {
				int jump_to = code->len;
				memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
			}
		}
		else if (inst[i].type == inst_if_t) {
			int jump_to = addr[inst[i].value.i->endif_p] + OPC_SIZE_JMP;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_elif_t) {
			int jump_to = addr[inst[i].value.el->endif_p] + OPC_SIZE_JMP;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_endif) {
			int jump_to = addr[inst[i].value.endif];
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_for_t) {
			int jump_to = addr[inst[i].value.fo->endfor] + OPC_SIZE_FOR_JMP;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_endfor_t) {
			int jump_to = addr[inst[i].value.endfor] + OPC_SIZE_JMP_IF;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_while_t) {
			int jump_to = addr[inst[i].value.wh->endwhile] + OPC_SIZE_JMP_IF;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
		else if (inst[i].type == inst_endwhile_t) {
			int jump_to = addr[inst[i].value.endwhile] + OPC_SIZE_JMP_IFN;
			memcpy(&addr[i] + 1, &jump_to, sizeof(jump_to));
		}
	}
	free(instructions_addr);
}