#include "../../include/Ast.h"
#include "../../include/bytecode/bytecode.h"
#include "../../include/bytecode/op_codes.h"
#include <stdint.h>

Bytecode_t *ast_to_bytecode(Ast x, Bytecode_t *code) {
	if (x.type == Ast_varcall_t) {
		if (is_varname_blt(x.root.varcall)) {
			uint32_t var_idx = bc_get_blt_var_idx(x.root.varcall);
			bytecode_append_char(code, 1, OPC_PUSH_VAR);
			bytecode_append_llint(code, var_idx);
			return code;
		}
		uint32_t var_idx = bc_get_var_idx(x.root.varcall);
		var_idx = var_idx | 2<<31;
		bytecode_append_char(code, 1, OPC_PUSH_VAR);
		bytecode_append_llint(code, var_idx);
		return code;
	}
	if (x.type == Ast_object_t) {
		S_sulfur_int addr = bc_get_const_addr(x.root.obj);
		bytecode_append_char(code, 1, OPC_PUSH);
		bytecode_append_llint(code, addr);
		return code;
	}
	if (x.type == Ast_anonym_func_t) {
		/*
			R_CALL 9		|	we want to use call so that returns works (9 = sizeof(R_CALL 9))
			Instructions	|
			rest of the ast	|
		*/
		bytecode_append_char(code, 1, OPC_R_CALL);
		bytecode_append_llint(code, 9);
		bytecode_from_ano_func(code, x.root.ano_func);
		return code;
	}
	if (x.type  == Ast_funccall_t) {
		/*
			...
			push arg3
			push arg2
			push arg1
			push nbr_arg
			push func    // same as pushing a variable
			CALL_FUNC
		*/
		for (int i = x.root.fun->nbr_arg; i >= 0; i++) {
			ast_to_bytecode(x.root.fun->args[i], code);
			bytecode_append_char(code, 1, OPC_PUSH_I);
		}
		bytecode_append_llint(code, x.root.fun->nbr_arg);
		//push the object containing the function
		{
			if (is_varname_blt(x.root.fun->name)) {
				uint32_t var_idx = bc_get_blt_var_idx(x.root.fun->name);
				bytecode_append_char(code, 1, OPC_PUSH_VAR);
				bytecode_append_llint(code, var_idx);
				return code;
			}
			uint32_t var_idx = bc_get_var_idx(x.root.fun->name);
			var_idx = var_idx | 2<<31;
			bytecode_append_char(code, 1, OPC_PUSH_VAR);
			bytecode_append_llint(code, var_idx);
		}
		return code;
	}
	if (x.type == Ast_add_t) {
		ast_to_bytecode(*x.right, code);
		ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, OPC_ADD);
	}
	if (x.type == Ast_sub_t) {
		ast_to_bytecode(*x.right, code);
		if (x.left) ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, x.left ? OPC_SUB :OPC_NEGATE);
	}
	if (x.type == Ast_mul_t) {
		ast_to_bytecode(*x.right, code);
		ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, OPC_MUL);
	}
	if (x.type == Ast_div_t) {
		ast_to_bytecode(*x.right, code);
		ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, OPC_DIV);
	}
	if (x.type == Ast_fldiv_t) {
		ast_to_bytecode(*x.right, code);
		ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, OPC_FLDIV);
	}
	if (x.type == Ast_mod_t) {
		ast_to_bytecode(*x.right, code);
		ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, OPC_MOD);
	}
	if (x.type == Ast_or_t) {
		ast_to_bytecode(*x.right, code);
		ast_to_bytecode(*x.left, code);
		bytecode_append_char(code, 1, OPC_OR);
	}

	//Bytecode_t code = new_bytecode();
	//if (x.type == Ast_funccall_t) {
	//	for (int i = x.root.fun->nbr_arg - 1; i >= 0; i--) {
	//		Bytecode_t temp = ast_to_bytecode(x.root.fun->args[i], ast_len);
	//		bytecode_cat(&code, temp);
	//		free_bytecode(temp);
	//	}
	//	//ush nbr of arg
	//	bytecode_add_char_arr(&code, "\x01\x02", 2);
	//	bytecode_append_llint(&code, add_constant_int(x.root.fun->nbr_arg));
//
	//	//push function
	//	S_sulfur_int name_idx = add_constant_str(x.root.fun->name);
	//	bytecode_add_char_arr(&code, "\x01\x01", 2);
	//	bytecode_append_llint(&code, name_idx);
//
	//	bytecode_add_char_arr(&code, "\x02", 1);
	//	(*ast_len) += 3;
	//}
	//else if (x.type == Ast_anonym_func_t) {
	//	bytecode_add_char_arr(&code, "\x03", 1);
	//	bytecode_append_llint(&code, x.root.ano_func->code_len);
	//	(*ast_len)++;
	//	for (int i = 0; i < x.root.ano_func->code_len; i++) {
	//		//Bytecode_t temp = make_bytecode_inst(x.root.ano_func->code[i]);
	//		//bytecode_cat(&code, temp);
	//		//free_bytecode(temp);
	//	}
//
	//}
	//else if(x.left == NULL && x.right == NULL && !x.isAst){
	//	if(x.type == Ast_varcall_t){
	//		bytecode_add_char_arr(&code, "\x01\x01", 2);
	//		bytecode_append_llint(&code, add_constant_str(x.root.fun->name));
	//		(*ast_len)++;
	//	}
	//	else if(x.type == Ast_object_t){
	//		bytecode_add_char_arr(&code, "\x01\x02", 2);
	//		bytecode_append_llint(&code, add_constants(*x.root.obj));
	//		(*ast_len)++;
	//	}
	//	else{
	//		printf("ERROR in Ast converting to bytecode %d\n", x.type);
	//		exit(1);
	//	}
	//}
	//if (x.isAst) {
	//	if (x.type == Ast_add_t) {
	//		if (x.left!=NULL && x.right!=NULL) {
	//			Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//			Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//			bytecode_cat(&code, b);
	//			bytecode_cat(&code, a);
	//			bytecode_append_char(&code, 1, '\x05');
	//			(*ast_len)++;
	//			free_bytecode(b);
	//			free_bytecode(a);
	//		}
	//	}
	//	else if(x.type == Ast_le_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x14');
	//		free_bytecode(b);
	//		free_bytecode(a);
	//	}
	//	else if(x.type == Ast_ge_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x15');
	//		free_bytecode(b);
	//		free_bytecode(a);
	//	}
	//	else if(x.type == Ast_plus_assign_t){
    //        Ast ast;
	//		Ast ast2;
    //        ast.isAst = 1;
    //        ast.type = Ast_assign_t;
    //        ast.left = x.left;
    //        ast.right = &ast2;
    //        ast.right->type = Ast_add_t;
    //        ast.right->left = x.left;
    //        ast.right->right = x.right;
//
	//		Bytecode_t temp = ast_to_bytecode(ast, ast_len);
	//		bytecode_cat(&code, temp);
	//		free_bytecode(temp);
    //    }
	//	else if (x.type == Ast_minus_assign_t) {
	//		Ast ast;
	//		Ast ast2;
    //        ast.isAst = 1;
    //        ast.type = Ast_assign_t;
    //        ast.left = x.left;
    //        ast.right = &ast2;
    //        ast.right->type = Ast_sub_t;
    //        ast.right->left = x.left;
    //        ast.right->right = x.right;
//
	//		Bytecode_t temp = ast_to_bytecode(ast, ast_len);
	//		bytecode_cat(&code, temp);
	//		free_bytecode(temp);
	//	}
	//	else if(x.type == Ast_leq_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		bytecode_append_char(&code, 1, '\x13');
	//		(*ast_len)++;
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_fldiv_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		bytecode_append_char(&code, 1, '\x09');
	//		(*ast_len)++;
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_or_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		bytecode_append_char(&code, 1, '\x0C');
	//		(*ast_len)++;
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_sub_t){
    //        if(x.left != NULL && x.right != NULL){
    //            Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//			Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//			bytecode_cat(&code, b);
	//			bytecode_cat(&code, a);
	//			bytecode_append_char(&code, 1, '\x06');
	//			(*ast_len)++;
	//			free_bytecode(a);
	//			free_bytecode(b);
    //        }
    //        if(x.left == NULL && x.right != NULL){
    //            Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//			bytecode_cat(&code, b);
	//			(*ast_len)++;
	//			bytecode_append_char(&code, 1, '\x0F');
	//			free_bytecode(b);
    //        }
    //    }
	//	else if(x.type == Ast_pow_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x0B');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_eq_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x10');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_noteq_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x11');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_div_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x08');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_mul_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x07');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_geq_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x12');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_mod_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x0A');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_and_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x0D');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_rshift_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x16');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_lshift_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x17');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if (x.type == Ast_not_t) {
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\x0E');
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_unpack_t){
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\xF5');
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_colon_t) {
	//		Bytecode_t a = ast_to_bytecode(*x.left, ast_len);
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		bytecode_cat(&code, a);
	//		(*ast_len)++;
	//		bytecode_append_char(&code, 1, '\xF1');
	//		free_bytecode(a);
	//		free_bytecode(b);
	//	}
	//	else if(x.type == Ast_dot_t) {
	//		
	//		if(x.right->type == Ast_varcall_t) {
	//			Bytecode_t b = ast_to_bytecode(*x.left, ast_len);
	//			bytecode_cat(&code, b);
	//			free_bytecode(b);
	//			bytecode_add_char_arr(&code, "\x01\x02", 2);
	//			bytecode_append_llint(&code, add_constant_str(x.right->root.varcall));
	//			bytecode_append_char(&code, 1, '\xF1');
	//			(*ast_len) += 2;
	//		}
	//		if(x.right->type == Ast_funccall_t) {
	//			for (int i = x.right->root.fun->nbr_arg - 1; i >= 0 ; i--) {
	//				Bytecode_t temp = ast_to_bytecode(x.right->root.fun->args[i], ast_len);
	//				bytecode_cat(&code, temp);
	//				free_bytecode(temp);
	//			}
	//			//ush nbr of arg
	//			bytecode_add_char_arr(&code, "\x01\x02", 2);
	//			bytecode_append_llint(&code, add_constant_int(x.right->root.fun->nbr_arg));
//
	//			//push container
	//			Bytecode_t b = ast_to_bytecode(*x.left, ast_len);
	//			bytecode_cat(&code, b);
	//			free_bytecode(b);
	//			//push member name
	//			bytecode_add_char_arr(&code, "\x01\x02", 2);
	//			bytecode_append_llint(&code, add_constant_str(x.right->root.fun->name));
	//			//push_dot
	//			bytecode_append_char(&code, 1, '\xF1');
	//			bytecode_append_char(&code, 1, '\x02');
	//			(*ast_len) += 4;
	//		}
	//	}
	//	else if(x.type == Ast_assign_t){
	//		Bytecode_t b = ast_to_bytecode(*x.right, ast_len);
	//		bytecode_cat(&code, b);
	//		free_bytecode(b);
	//		if (x.left->type == Ast_varcall_t) {
	//			bytecode_add_char_arr(&code, "\x01\x02", 2);
	//			bytecode_append_llint(&code, add_constant_str(x.left->root.varcall));
	//			bytecode_append_char(&code, 1, '\xF0');
	//			(*ast_len)++;
	//		}
	//		else if (x.left->type == Ast_dot_t) {
	//			Bytecode_t in_what = ast_to_bytecode(*x.left->left, ast_len);
	//			bytecode_cat(&code, in_what);
	//			if (x.left->right->type != Ast_varcall_t) {
	//				printf("ERROR ast dot operator not an identifier\n");
	//				exit(1);
	//			}
	//			bytecode_add_char_arr(&code, "\x01\x02", 2);
	//			bytecode_append_llint(&code, add_constant_str(x.left->right->root.varcall));
	//			bytecode_append_char(&code, 1, '\xF2');
	//			(*ast_len)++;
	//			free_bytecode(in_what);
	//		}
	//		else if (x.left->type == Ast_colon_t) {
	//			Bytecode_t in_what = ast_to_bytecode(*x.left->left, ast_len);
	//			bytecode_cat(&code, in_what);
	//			Bytecode_t where = ast_to_bytecode(*x.left->right, ast_len);
	//			bytecode_cat(&code, where);
	//			bytecode_append_char(&code, 1, '\xF4');
	//			(*ast_len)++;
	//			free_bytecode(in_what);
	//			free_bytecode(where);
	//		}
	//	}
	//}
	//return code;

}





/*

l = list(3,4,5)

(l:-1 > 3) && (1 + l:3) 
 (3  > 3)  && (1 + COMMING_ERROR("out of range"))
   (0b)    && (COMMING_ERROR("out of range"))
 		   0b
if the left side of of && is 0b the right side will be deleted so no error will occure




*/