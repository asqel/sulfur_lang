#include "../include/instruction.h"
#include "../include/make_requested_vars.h"
#include "../include/sulfur.h"

int next_uid = 0;

 #define elif else if

#define get_uid() (next_uid++)

Instruction *ast_to_inst(Ast *x, int *res_len);

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
	EXPR:
		-> push/pop/op
		ex:
			1 + 2 -> push 2; push 1; ADD; pop
	
	*/
	Instruction *res = NULL;
	int res_len = 0;
	for (int i = 0; i < *instruction_len; i++)  {
		if (code[i].type == inst_expr_t) {
			int expr_len = 0;
			Instruction *expr = NULL;
			expr = ast_to_inst(code[i].value.expr, &expr_len);
			res = realloc(res, sizeof(Instruction) * (res_len + expr_len));
			for (int i = 0; i < ops_len; i++)
				res[res_len++] = expr[i];
			free(expr);
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
	//for(int i = 0; i < res_len; i++) {
	//	if (res[i].type == inst_S_push_ount_t) {
	//		printf("push %d\n", res[i].value.ount);
	//	}
	//	if (res[i].type == inst_S_op_t) {
	//		if (res[i].value.op == inst_S_op_add_t) {
	//			printf("ADD\n");
	//		}
	//		else {
	//			printf("UNKNOWN OP\n");
	//		}
	//	}
	//}
	// make link a again replace .line (uid) by .line (index)
	instruction_free_array(code, *instruction_len);
	*instruction_len = res_len;
	return res;

}

int is_op_simple(int type) {
	switch (type){
		case Ast_add_t:
		case Ast_sub_t:
		case Ast_mul_t:
		case Ast_div_t:
		case Ast_fldiv_t:
		case Ast_mod_t:
		case Ast_eq_t:
		case Ast_ge_t:
		case Ast_geq_t:
		case Ast_le_t:
		case Ast_leq_t:
		case Ast_lshift_t:
		case Ast_rshift_t:
		case Ast_noteq_t:
			return 1;
			
		default:
			return 0;
	}
}

int get_bytecode_op(int type) {
	switch (type) {
		case Ast_add_t:
			return inst_S_op_add_t;
		case Ast_sub_t:
			return inst_S_op_sub_t;
		case Ast_mul_t:
			return inst_S_op_mul_t;
		case Ast_div_t:
			return inst_S_op_div_t;
		case Ast_fldiv_t:
			return inst_S_op_fldiv_t;
		case Ast_mod_t:
			return inst_S_op_mod_t;
		case Ast_eq_t:
			return inst_S_op_eq_t;
		case Ast_ge_t:
			return inst_S_op_ge_t;
		case Ast_geq_t:
			return inst_S_op_geq_t;
		case Ast_le_t:
			return inst_S_op_le_t;
		case Ast_leq_t:
			return inst_S_op_leq_t;
		case Ast_lshift_t:
			return inst_S_op_lshift_t;
		case Ast_rshift_t:
			return inst_S_op_rshift_t;
		case Ast_noteq_t:
			return inst_S_op_noteq_t;
		default:
			return 0;
			break;
	}
}

Instruction *ast_to_inst(Ast *x, int *res_len) {
	Instruction *res = NULL;
	if (x->type == Ast_varcall_idx_t) {
		if (!strcmp(CTX.requested_vars[x->root.var_idx], "nil")) {
			(*res_len)++;
			res = realloc(res, sizeof(Instruction) *(*res_len));
			res[*res_len - 1].facultative = 0;
			res[*res_len - 1].line = -1;
			res[*res_len - 1].type = inst_S_push_nil_t;
			return res;
		}
	}
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
		if ((*x->root.obj).type != Obj_string_t && (*x->root.obj).type != Obj_floap_t && (*x->root.obj).type != Obj_ount_t) {
			printf("ERROR cannot push value of type %s\n", Obj_type_as_str(x->root.obj->type));
			exit(1);
		}
		if ((*x->root.obj).type == Obj_ount_t) {
			(*res_len)++;
			res = realloc(res, sizeof(Instruction) *(*res_len));
			res[*res_len - 1].facultative = 0;
			res[*res_len - 1].line = -1;
			res[*res_len - 1].type = inst_S_push_ount_t;
			res[*res_len - 1].value.ount = x->root.obj->val.i;
			return res;
		}
		elif (x->root.obj->type == Obj_floap_t) {
			(*res_len)++;
			res = realloc(res, sizeof(Instruction) *(*res_len));
			res[*res_len - 1].facultative = 0;
			res[*res_len - 1].line = -1;
			res[*res_len - 1].type = inst_S_push_floap_t;
			res[*res_len - 1].value.ount = x->root.obj->val.f;
			return res;
		}
		if (x->root.obj->type != Obj_string_t) {
			printf("ERROR cannot convert ast obj type %d to bytecode ast\n", x->root.obj->type);
			exit(1);
		}
		(*res_len)++;
		res = realloc(res, sizeof(Instruction) *(*res_len));
		res[*res_len - 1].facultative = 0;
		res[*res_len - 1].line = -1;
		res[*res_len - 1].type = inst_S_push_t;
		res[*res_len - 1].value.obj = malloc(sizeof(Object));
		res[*res_len - 1].value.obj->type = Obj_string_t;
		res[*res_len - 1].value.obj->val.s = strdup(x->root.obj->val.s);
		return res;

	}
	if (x->type == Ast_sub_t) {
		// check if unary
		if(x->left==NULL && x->right!=NULL){
		
		}

	}
	elif (is_op_simple(x->type)) {
		int right_len = 0;
		Instruction *right = ast_to_inst(x->right, &right_len);
		int left_len = 0;
		Instruction *left = ast_to_inst(x->left, &left_len);
		res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len));
		for (int i = 0; i < right_len; i++)
			res[(*res_len)++] = right[i];
		free(right);
		for (int i = 0; i < left_len; i++)
			res[(*res_len)++] = left[i];
		free(left);
		res = realloc(res, sizeof(Instruction) * (*res_len + 1));
		res[*res_len].type = inst_S_op_t;
		res[*res_len].value.op = get_bytecode_op(x->type);
		(*res_len)++;
		return res;
	}
	return NULL;
}





/*
X && Y	:
	0 | push X
	1 | jmp ifn 6
	2 | push y
	3 | jmp ifn 6
	4 | push 1b
	5 | jmp 7
	6 | push 0b

	relative jumps:
	0 | push X
	1 | jmp ifn 5
	2 | push y
	3 | jmp ifn 3
	4 | push 1b
	5 | jmp 2
	6 | push 0b


X || Y	 :
	0 | push X
	1 | jmp if 6
	2 | push Y
	3 | jmp if 6
	4 | push 0b
	5 | jmp 7
	6 | push 1b

	relative jumps:
	0 | push X
	1 | jmp if 5
	2 | push Y
	3 | jmp if 3
	4 | push 0b
	5 | jmp 2
	6 | push 1b

while(1) {;} :
    jmp 0
*/
