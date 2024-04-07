#include "../include/instruction.h"

int next_uid = 0;

 #define elif else if

#define get_uid() (next_uid++)

Instruction	*finish_instrcutions(Instruction *code, int *instruction_len) {
	// do uid
	// uid will be stored in .line
	for (int i = 0; i < *instruction_len; i++)
		code[i].line = get_uid();
	for (int i = 0; i < *instruction_len; i++) {
		if (code[i].type == inst_funcdef_t) {
			code[i].value.fc->code = finish_instrcutions(code[i].value.fc->code, &code[i].value.fc->code_len);
		}
	}
	/*
	
	0	| push b
	1	| push a
	2	| dup
	3	| set "i"
	4	| eq
	5	| jmp A+x+1
	6	| ...
	7+x	| push b
	8+x | push a
	9+x | push "i", hash("i")
	A+x | FOR(6)
	
	*/
	// make links
	// jmp to uuid instead of index
	// same for endloopss if elif ...
	for (int i = 0; i <  *instruction_len; i++) {
		switch (code[i].type){
			case inst_jmp_t:
				code[i].value.jmp = code[code[i].value.jmp].line;
				break;
			case inst_endfor_t:
				code[i].value.endfor = code[code[i].value.endfor].line;
				break;
			case inst_endwhile_t:
				code[i].value.endwhile = code[code[i].value.endwhile].line;
				break;
			case inst_for_t:
				code[i].value.fo->endfor = code[code[i].value.fo->endfor].line;
				break;
			case inst_while_t:
				code[i].value.wh->endwhile = code[code[i].value.wh->endwhile].line;
				break;
			case inst_if_t:
				code[i].value.i->endif_p = code[code[i].value.i->endif_p].line;
				break;
			case inst_elif_t:
				code[i].value.el->endif_p = code[code[i].value.el->endif_p].line;
				break;
			case inst_endif:
				code[i].value.endif = code[code[i].value.endif].line;
				break;
			case inst_endifelse:
				code[i].value.endifelse = code[code[i].value.endifelse].line;
				break;
			default:
				break;
		}
	}
	/*
	arr[i] = arr + i*4
	i[arr] = i + arr*4
	EXPR:
		-> push/pop/op
		ex:
			1 + 2 -> push 2; push 1; ADD; pop
	
	*/
	Instruction *res = NULL;
	int res_len = 0;
	for (int i = 0; i < *instruction_len; i++)  {
		if (code[i].type == inst_for_t) {
			
		}
		//if (code[i].type == inst_expr_t) {
		//	int ast_len = 0;
		//	Instruction *ast = NULL;
		//	Instruction *ast = ast_to_inst(code[i].value.expr, &ast_len);
		//	res = realloc(res, sizeof(Instruction) * (res_len + ast_len + 1));
		//	for (int k = 0; k < ast_len; k++) {
		//		res[res_len++] = ast[i];
		//		if (k == 0)
		//			res[res_len - 1].line =	code[i].line;
		//		else
		//			res[res_len - 1].line = -1;
		//	}
		//	free(ast);
		//	res[res_len].facultative = 0;
		//	res[res_len].line = -1;
//
		//}
	}
	// make link a again replace .line (uid) by .line (index)
	return NULL;

}



Instruction *ast_to_inst(Ast *x, int *res_len) {
	Instruction *res = NULL;
	if (x->type == Ast_object_t) {
		if ((*x->root.obj).type == Obj_boolean_t) {
			(*res_len)++;
			res = realloc(res, sizeof(Instruction) *(*res_len));
			res[*res_len - 1].facultative = 0;
			res[*res_len - 1].line = -1;
			res[*res_len - 1].type =(*x->root.obj).val.b ? inst_S_push_1b_t : inst_S_push_0b_t;
			return res;
		}
		if ((*x->root.obj).type == Obj_complex_t) {
			(*res_len)++;
			res = realloc(res, sizeof(Instruction) *(*res_len));
			res[*res_len - 1].facultative = 0;
			res[*res_len - 1].line = -1;
			res[*res_len - 1].type = inst_S_push_i_t;
			return res;
		}
		if ((*x->root.obj).type == Obj_nil_t) {
			(*res_len)++;
			res = realloc(res, sizeof(Instruction) *(*res_len));
			res[*res_len - 1].facultative = 0;
			res[*res_len - 1].line = -1;
			res[*res_len - 1].type = inst_S_push_nil_t;
			return res;
		}
		// can only  push string / floap / ount from constants
		if ((*x->root.obj).type != Obj_string_t && (*x->root.obj).type != Obj_floap_t && (*x->root.obj).type != Obj_ount_t)
		(*res_len)++;
		res = realloc(res, sizeof(Instruction) *(*res_len));
		res[*res_len - 1].facultative = 0;
		res[*res_len - 1].line = -1;
		res[*res_len - 1].type = inst_S_push_t;
		res[*res_len - 1].value.obj = malloc(sizeof(Object));
		*res[*res_len - 1].value.obj = Obj_cpy(*x->root.obj);
		return res;

	}
	if (x->type == Ast_add_t) {
		int right_len = 0;
		Instruction *right = ast_to_inst(x->right, &right_len);
		int left_len = 0;
		Instruction *left = ast_to_inst(x->right, &left_len);
		res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len));
		for (int i = 0; i < right_len; i++)
			res[(*res_len)++] = right[i];
		free(right);
		for (int i = 0; i < left_len; i++)
			res[(*res_len)++] = left[i];
		free(left);
	}
	return NULL;
}