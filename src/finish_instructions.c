#include "../include/instruction.h"
#include "../include/make_requested_vars.h"
#include "../include/utilities.h"
#include "../include/sulfur.h"
#include "../include/finish_instructions.h"

int next_uid = 0;

 #define elif else if

#define get_uid() (next_uid++)

Instruction *ast_to_inst(Ast *x, int *res_len);

/*
everything will be pushed from ledt to right
a + b -> push a; push b; add
f(a, b, $c, d) ->
            push f
            push prepare_call
            push a
            push b
            push c
            unpack
            push d
            call_func

*/
// consider that the entire code is in a fake anonymous function
int is_in_ano_func = 1;

Instruction *extend_array_inst(Instruction **arr, int *len, Instruction *ext, int ext_len) {
    *arr = realloc(*arr, sizeof(Instruction) * (*len + ext_len));
    for (int i = 0; i < ext_len; i++)
        (*arr)[(*len)++] = ext[i];
    return *arr;
}

int current_line = 0;

Instruction *inst_to_bytecode(Instruction *code, int len, int *res_len) {
    Instruction *res = NULL;
    for (int i = 0; i < len; i++)  {
        current_line = code[i].line;
        if (code[i].type == inst_expr_t) {
            int expr_len = 0;
            Instruction *expr = NULL;
            expr = ast_to_inst(code[i].value.expr, &expr_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + expr_len));
            expr[0].line = code[i].line;
            for (int i = 0; i < expr_len; i++)
                res[(*res_len)++] = expr[i];
            free(expr);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = inst_S_pop_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
        }
        elif (code[i].type == inst_return_t) {
            int expr_len = 0;
            Instruction *expr = NULL;
            expr = ast_to_inst(code[i].value.ret, &expr_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + expr_len));
            expr[0].line = code[i].line;
            for (int i = 0; i < expr_len; i++)
                res[(*res_len)++] = expr[i];
            free(expr);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = (is_in_ano_func ?  inst_S_ret_t : inst_S_fret_t);
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
        }
        elif (code[i].type == inst_while_t) {
            /*
            while (a) {
                INST
            }
            0 | push a
            1 | jmpifn +2 + len(INST)
            2 |     INST
            3 | jmp -len(INST) - 1 -len(a)
            4 | ...
            */
            int endwhile_idx = -1;
            for(int k = i; k < len; k++)
                if (code[k].line == code[i].value.wh->endwhile) {
                    endwhile_idx = k;
                    break;
                }
            if (endwhile_idx == -1) {
                PRINT_ERR("ERROR converting while to bytecode\n");
                exit(1);
            }
            int cond_len = 0;
            Instruction *cond = ast_to_inst(code[i].value.wh->condition, &cond_len);
            int body_len = 0;
            Instruction *body = inst_to_bytecode(&code[i + 1], endwhile_idx - i - 1, &body_len);
            cond[0].line = code[i].line;
            extend_array_inst(&res, res_len, cond, cond_len);
            free(cond);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_rjmpifn_t;
            res[*res_len].value.jmp_length = 2 + body_len;
            (*res_len)++;
            extend_array_inst(&res, res_len, body, body_len);
            free(body);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].facultative = 0;
            res[*res_len].line = code[endwhile_idx].line;
            res[*res_len].type = inst_S_rjmp_t;
            res[*res_len].value.jmp_length = -body_len - 1 - cond_len;
            (*res_len)++;
            i = endwhile_idx;
        }
        elif (code[i].type == inst_elif_t || code[i].type == inst_else_t || code[i].type == inst_endif || code[i].type == inst_endifelse) {
            PRINT_ERR("ERROR converting to bytecode unexepected elif else endif or endifelse");
            exit(1);
        }
        elif (code[i].type == inst_if_t) {
            /*  if (X)
                    ... (body 1)
                    endif
                elif (Y)
                    ... (body 2) 
                    endif
                elif (Z)
                    ... (body 3)
                    endif
                else
                    ... (body 4)
                    endif
                endifelse
                ---------
                push X
                rjmpifn len(body 1) + 2
                    ... (body 1)
                    rjmp END
                push Y
                rjmpifn len(body 2) + 2
                    ... (body 2)
                    rjmp END
                push Z
                rjmpifn len(body 3) + 2
                    ... (body 3)
                    rjmp  END
                ... (body 4)
                END:
            */
            int *to_link_idx = NULL;
            int to_link_idx_len = 0;

            int endif_idx = -1;
            for(int k = i; k < len; k++)
            if (code[k].line == code[i].value.i->endif_p) {
                endif_idx = k;
                break;
            }
            if (endif_idx == -1) {
                PRINT_ERR("ERROR converting if to bytecode\n");
                exit(1);
            }
            int body_len = 0;
            Instruction *body = inst_to_bytecode(&(code[i + 1]), endif_idx - i - 1, &body_len);
            int expr_len = 0;
            Instruction *expr = NULL;
            expr = ast_to_inst(code[i].value.i->condition, &expr_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + expr_len));
            expr[0].line = code[i].line;
            for (int i = 0; i < expr_len; i++)
                res[(*res_len)++] = expr[i];
            free(expr);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = inst_S_rjmpifn_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.jmp_length = body_len + 2;
            (*res_len)++;

            extend_array_inst(&res, res_len, body, body_len);
            free(body);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = inst_S_rjmp_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.jmp_length = 42424242;
            (*res_len)++;
            if (code[endif_idx + 1].type != inst_elif_t && code[endif_idx + 1].type != inst_else_t) {
                i = endif_idx + 1;
                res[*res_len - 1].value.jmp_length = 1;
                continue;;
            }
            to_link_idx_len++;
            to_link_idx = realloc(to_link_idx, sizeof(int) * to_link_idx_len);
            to_link_idx[to_link_idx_len - 1] = *res_len - 1;
            i = endif_idx + 1;
            while (code[i].type == inst_elif_t) {
                int endif_idx = -1;
                for(int k = i; k < len; k++)
                if (code[k].line == code[i].value.el->endif_p) {
                    endif_idx = k;
                    break;
                }
                if (endif_idx == -1) {
                    PRINT_ERR("ERROR converting elif to bytecode\n");
                    exit(1);
                }

                int body_len = 0;
                Instruction *body = inst_to_bytecode(&(code[i + 1]), endif_idx - i - 1, &body_len);

                int expr_len = 0;
                Instruction *expr = ast_to_inst(code[i].value.el->condition, &expr_len);
                expr[0].line = code[i].line;
                res = realloc(res, sizeof(Instruction) * (*res_len + expr_len));
                for (int i = 0; i < expr_len; i++)
                    res[(*res_len)++] = expr[i];
                free(expr);

                res = realloc(res, sizeof(Instruction) * (*res_len + 1));
                res[*res_len].type = inst_S_rjmpifn_t;
                res[*res_len].line = -1;
                res[*res_len].facultative = 0;
                res[*res_len].value.jmp_length = body_len + 2;
                (*res_len)++;
                
                extend_array_inst(&res, res_len, body, body_len);
                free(body);

                res = realloc(res, sizeof(Instruction) * (*res_len + 1));
                res[*res_len].type = inst_S_rjmp_t;
                res[*res_len].line = -1;
                res[*res_len].facultative = 0;
                res[*res_len].value.jmp_length = 42424242;
                (*res_len)++;

                to_link_idx_len++;
                to_link_idx = realloc(to_link_idx, sizeof(int) * to_link_idx_len);
                to_link_idx[to_link_idx_len - 1] = *res_len - 1;
                i = endif_idx + 1;
            }
            int endifelse_idx = *res_len;
            if (code[i].type == inst_else_t) {
                int endif_idx = -1;
                for(int k = i; k < len; k++)
                    if (code[k].line == code[i].value.endif) {
                        endif_idx = k;
                        break;
                    }
                if (endif_idx == -1) {
                    PRINT_ERR("ERROR converting else to bytecode\n");
                    exit(1);
                }
                int body_len = 0;
                Instruction *body = inst_to_bytecode(&(code[i + 1]), endif_idx - i - 1, &body_len);
                extend_array_inst(&res, res_len, body, body_len);
                free(body);
                endifelse_idx = *res_len;
                i = endif_idx + 1;
            }
            if (code[i].type != inst_endifelse) {
                PRINT_ERR("ERROR converting if elif else to bytecode\n");
                exit(1);
            }
            for (int k = 0; k < to_link_idx_len; k++) {
                res[to_link_idx[k]].value.jmp_length = endifelse_idx - to_link_idx[k];
            }
            free(to_link_idx);
        }
        elif (code[i].type == inst_pass_t) {
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = inst_S_nop_t;
            res[*res_len].line = code[i].line;
            (*res_len)++;
        }
        elif (code[i].type == inst_for_t) {
            /*
            for(i from start to end)
                ...(body)
            endfor

            push start
            dup
            set(i)
            pop
            push end
            eq
            rjmpif 4 + len(body)
                ...(body)
                push a
                push b
                for(i, -2 - len(body))
            */
            int endfor_idx = -1;
            for(int k = i; k < len; k++)
                if (code[k].line == code[i].value.fo->endfor) {
                    endfor_idx = k;
                    break;
                }
            if (endfor_idx == -1) {
                PRINT_ERR("ERROR converting for to bytecode\n");
                exit(1);
            }
            int start_len = 0;
            Instruction *start = ast_to_inst(code[i].value.fo->start, &start_len);
            int end_len = 0;
            Instruction *end = ast_to_inst(code[i].value.fo->end, &end_len);
            int body_len = 0;
            Instruction *body = inst_to_bytecode(&(code[i + 1]), endfor_idx - i - 1, &body_len);
            start[0].line = code[i].line;
            extend_array_inst(&res, res_len, start, start_len);

            res = realloc(res, sizeof(Instruction) * (*res_len + 3));
            res[*res_len].type = inst_S_dup_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
            res[*res_len].type = inst_S_var_set_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.var_idx = code[i].value.fo->var_name;
            (*res_len)++;
            res[*res_len].type = inst_S_pop_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
            extend_array_inst(&res, res_len, end, end_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + 2));
            res[*res_len].type = inst_S_op_eq_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
            res[*res_len].type = inst_S_rjmpif_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.jmp_length = 4 + body_len;
            (*res_len)++;

            extend_array_inst(&res, res_len, body, body_len);
            extend_array_inst(&res, res_len, start, start_len);
            extend_array_inst(&res, res_len, end, end_len);

            res = realloc(res, sizeof(Instruction) * (*res_len + 2));
            res[*res_len].type = inst_S_for_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.for_bc_info[0] = code[i].value.fo->var_name;
            res[*res_len].value.for_bc_info[1] = -2 - body_len;
            (*res_len)++;
            i = endfor_idx;
            free(body);
            free(start);
            free(end);
        }
        elif (code[i].type == inst_funcdef_t) {
            int body_len = code[i].value.fc->code_len;
            char old_is_in = is_in_ano_func;
            is_in_ano_func = 0;
            Instruction *body = finish_instrcutions(code[i].value.fc->code, &body_len);
            is_in_ano_func = old_is_in;

            res = realloc(res, sizeof(Instruction) * (*res_len + 5 + code[i].value.fc->args_len));
            res[*res_len].type = inst_S_fstart_def_t;
            res[*res_len].line = code[i].line;
            res[*res_len].facultative = 0;
            (*res_len)++;
            res[*res_len].type = code[i].value.fc->args_mod == 'o' ? inst_S_fset_mod_fixed : inst_S_fset_mod_packed;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
            res[*res_len].type = inst_S_fset_param_len_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.ount = code[i].value.fc->args_len;
            (*res_len)++;
            for (int k = 0; k < code[i].value.fc->args_len; k++) {
                res[*res_len].type = inst_S_fadd_param_t;
                res[*res_len].line = -1;
                res[*res_len].facultative = 0;
                res[*res_len].value.ount = get_requested_var(code[i].value.fc->args[k]);
                (*res_len)++;
            }
            res[*res_len].type = inst_S_fset_name_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.ount = get_requested_var(code[i].value.fc->info.name);
            (*res_len)++;
            res[*res_len].type = inst_S_fend_def_t;
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            res[*res_len].value.ount = 1 + body_len;
            (*res_len)++;
            extend_array_inst(&res, res_len, body, body_len);
            free(body);
        }

    }
    return res;
}



Instruction	*finish_instrcutions(Instruction *code, int *instruction_len) {
    // do uid
    // uid will be stored in .line
    for (int i = 0; i < *instruction_len; i++)
        code[i].line = get_uid();
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
    res = inst_to_bytecode(code, *instruction_len, &res_len);

    // make link a again replace .line (uid) by .line (index)
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
        case Ast_pow_t:
        case Ast_mod_t:
        case Ast_eq_t:
        case Ast_ge_t:
        case Ast_geq_t:
        case Ast_le_t:
        case Ast_leq_t:
        case Ast_lshift_t:
        case Ast_rshift_t:
        case Ast_noteq_t:
        case Ast_colon_t:
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
        case Ast_pow_t:
            return inst_S_op_pow_t;
        case Ast_colon_t:
            return inst_S_col_get_t;
        default:
            return 0;
            break;
    }
}

Instruction *ast_to_inst(Ast *x, int *res_len) {
    Instruction *res = NULL;
    if (x->type == Ast_varcall_idx_t) {
        if (x->root.var_idx >= 0) {
            if (!strcmp(CTX.requested_vars[x->root.var_idx], "nil")) {
                (*res_len)++;
                res = realloc(res, sizeof(Instruction) * (*res_len));
                res[*res_len - 1].facultative = 0;
                res[*res_len - 1].line = -1;
                res[*res_len - 1].type = inst_S_push_nil_t;
                return res;
            }
            else {
                (*res_len)++;
                res = realloc(res, sizeof(Instruction) * (*res_len));
                res[*res_len - 1].facultative = 0;
                res[*res_len - 1].line = -1;
                res[*res_len - 1].type = inst_S_push_var_t;
                res[*res_len - 1].value.var_idx = x->root.var_idx;
                return res;
            }
        }
        else {
            (*res_len)++;
            res = realloc(res, sizeof(Instruction) *(*res_len));
            res[*res_len - 1].facultative = 0;
            res[*res_len - 1].line = -1;
            res[*res_len - 1].type = inst_S_push_global_var_t;
            res[*res_len - 1].value.var_idx = -x->root.var_idx - 1;
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
            PRINT_ERR("ERROR cannot push value of type %s on line %d\n", Obj_type_as_str(x->root.obj->type), current_line);
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
            PRINT_ERR("ERROR cannot convert ast obj type %d to bytecode ast on line %d\n", x->root.obj->type, current_line);
            exit(1);
        }
        (*res_len)++;
        res = realloc(res, sizeof(Instruction) *(*res_len));
        res[*res_len - 1].facultative = 0;
        res[*res_len - 1].line = -1;
        res[*res_len - 1].type = inst_S_push_str_t;
        res[*res_len - 1].value.var_idx = add_string_constant(x->root.obj->val.s);
        return res;

    }
    if (x->type == Ast_sub_t) {
        // check if unary
        if(x->left == NULL && x->right != NULL){
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left, &left_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + left_len));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            free(left);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = inst_S_op_t;
            res[*res_len].value.op = inst_S_op_negate_t;
            (*res_len)++;
            return res;
        }
        else {
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left, &left_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            free(left);
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].type = inst_S_op_t;
            res[*res_len].value.op = inst_S_op_sub_t;
            (*res_len)++;
            return res;
        }

    }
    elif (is_op_simple(x->type)) {
        /* a OP b
        push a
        push b
        OP
        */
        int right_len = 0;
        Instruction *right = ast_to_inst(x->right, &right_len);
        int left_len = 0;
        Instruction *left = ast_to_inst(x->left, &left_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len + 1));
        for (int i = 0; i < left_len; i++)
            res[(*res_len)++] = left[i];
        free(left);
        for (int i = 0; i < right_len; i++)
            res[(*res_len)++] = right[i];
        free(right);
        res[*res_len].type = inst_S_op_t;
        res[*res_len].value.op = get_bytecode_op(x->type);
        (*res_len)++;
        return res;
    }
    elif (x->type == Ast_and_t) {
        /*
        X && Y	:
            0 | push X
            1 | jmp ifn +5
            2 | push y
            3 | jmp ifn +3
            4 | push 1b
            5 | jmp +2
            6 | push 0b
        */
        int jmp1_idx;
        int jmp2_idx;
        int jmp3_idx;
        int left_len = 0;
        Instruction *left = ast_to_inst(x->left, &left_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + left_len + 1));
        for (int i = 0; i < left_len; i++)
            res[(*res_len)++] = left[i];
        free(left);
        res[(*res_len)].type = inst_S_rjmpifn_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        res[(*res_len)].value.jmp = 0xFFFFFFFF;
        jmp1_idx = *res_len;
        (*res_len)++;
        int right_len = 0;
        Instruction *right = ast_to_inst(x->right, &right_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 4));
        for (int i = 0; i < right_len; i++)
            res[(*res_len)++] = right[i];
        free(right);
        res[(*res_len)].type = inst_S_rjmpifn_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        res[(*res_len)].value.jmp = 0xFFFFFFFF;
        jmp2_idx = *res_len;
        (*res_len)++;
        res[(*res_len)].type = inst_S_push_1b_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        (*res_len)++;
        res[(*res_len)].type = inst_S_rjmp_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        res[(*res_len)].value.jmp = 0xFFFFFFFF;
        jmp3_idx = *res_len;
        (*res_len)++;
        res[(*res_len)].type = inst_S_push_0b_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        (*res_len)++;
        res[jmp1_idx].value.jmp = 4 + right_len;
        res[jmp2_idx].value.jmp = 3;
        res[jmp3_idx].value.jmp = 2;
        return res;
    }
    elif (x->type == Ast_or_t) {
        /*
        X || Y	 :
            0 | push X
            1 | jmp if 6
            2 | push Y
            3 | jmp if 6
            4 | push 0b
            5 | jmp 7
            6 | push 1b
        */
        int jmp1_idx;
        int jmp2_idx;
        int jmp3_idx;
        int left_len = 0;
        Instruction *left = ast_to_inst(x->left, &left_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + left_len + 1));
        for (int i = 0; i < left_len; i++)
            res[(*res_len)++] = left[i];
        free(left);
        res[(*res_len)].type = inst_S_rjmpif_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        res[(*res_len)].value.jmp = 0xFFFFFFFF;
        jmp1_idx = *res_len;
        (*res_len)++;
        int right_len = 0;
        Instruction *right = ast_to_inst(x->right, &right_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 4));
        for (int i = 0; i < right_len; i++)
            res[(*res_len)++] = right[i];
        free(right);
        res[(*res_len)].type = inst_S_rjmpif_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        res[(*res_len)].value.jmp = 0xFFFFFFFF;
        jmp2_idx = *res_len;
        (*res_len)++;
        res[(*res_len)].type = inst_S_push_0b_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        (*res_len)++;
        res[(*res_len)].type = inst_S_rjmp_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        res[(*res_len)].value.jmp = 0xFFFFFFFF;
        jmp3_idx = *res_len;
        (*res_len)++;
        res[(*res_len)].type = inst_S_push_1b_t;
        res[(*res_len)].facultative = 0;
        res[(*res_len)].line = -1;
        (*res_len)++;
        res[jmp1_idx].value.jmp = 4 + right_len;
        res[jmp2_idx].value.jmp = 3;
        res[jmp3_idx].value.jmp = 2;
        return res;
    }
    elif (x->type == Ast_dot_t) {
        // right can only be varcall (non global) or funcall
        /*
        a.b
        push a
        dot_get(b)
        ------
        a.b(x, y, z)
        push a
        dot_get(b)
        prepare_fcall
        push x
        push y
        push z
        fcall
        */
        if (x->right->type == Ast_varcall_idx_t) {
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left, &left_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + left_len + 1));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_dot_get_t;
            res[*res_len].value.var_idx = x->right->root.var_idx;
            (*res_len)++;
            return res;
        }
        else {
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left, &left_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + left_len + 2));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_dot_get_t;
            res[*res_len].value.var_idx = x->right->root.fun->name_idx;
            (*res_len)++;
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_prepare_fcall_t;
            (*res_len)++;
            for (int i = 0; i < x->right->root.fun->nbr_arg; i++) {
                int arg_len = 0;
                Instruction *arg = ast_to_inst(&(x->right->root.fun->args[i]), &arg_len);
                res = realloc(res, sizeof(Instruction) * (*res_len + arg_len));
                for (int i = 0; i < arg_len; i++)
                    res[(*res_len)++] = arg[i];
                free(arg);
            }
            res = realloc(res, sizeof(Instruction) * (*res_len + 1));
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_fcall_t;
            (*res_len)++;
            return res;
        }
    }
    elif (x->type == Ast_assign_t) {
        if (x->left->type == Ast_varcall_idx_t) {
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 1));
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            if (x->left->root.var_idx >= 0) {
                res[*res_len].type = inst_S_var_set_t;
                res[*res_len].value.var_idx = x->left->root.var_idx;
            }
            else {
                res[*res_len].type = inst_S_global_var_set_t;
                res[*res_len].value.var_idx = -x->left->root.var_idx - 1;
            }
            (*res_len)++;
            return res;
        }
        elif (x->left->type == Ast_dot_t) {
            /*
            a.b = c
            push a
            push c
            dot_set(b)
            */
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left->left, &left_len);
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len + 1));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            free(left);
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_dot_set_t;
            res[*res_len].value.var_idx = x->left->right->root.var_idx;
            (*res_len)++;
            return res;
        }
        elif (x->left->type == Ast_colon_t) {
            /*
            a:b = c
            push a
            push b
            push c
            col_set
            */
            int ast_a_len = 0;
            Instruction *ast_a = ast_to_inst(x->left->left, &ast_a_len);
            int ast_b_len = 0;
            Instruction *ast_b = ast_to_inst(x->left->right, &ast_b_len);
            int ast_c_len = 0;
            Instruction *ast_c = ast_to_inst(x->right, &ast_c_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + ast_a_len + ast_b_len + ast_c_len + 1));
            for (int i = 0; i < ast_a_len; i++)
                res[(*res_len)++] = ast_a[i];
            free(ast_a);
            for (int i = 0; i < ast_b_len; i++)
                res[(*res_len)++] = ast_b[i];
            free(ast_b);
            for (int i = 0; i < ast_c_len; i++)
                res[(*res_len)++] = ast_c[i];
            free(ast_c);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_col_set_t;
            (*res_len)++;
            return res;
        }
        else {
            PRINT_ERR("ERROR cannot convert ast (=) for left type %d on line %d\n", x->left->type, current_line);
            exit(1);
        }
        
    }
    elif (x->type == Ast_plus_assign_t) {
        if (x->left->type == Ast_varcall_idx_t) {
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 1));
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            if (x->left->root.var_idx >= 0) {
                res[*res_len].type = inst_S_var_inc_t;
                res[*res_len].value.var_idx = x->left->root.var_idx;
            }
            else {
                res[*res_len].type = inst_S_global_var_inc_t;
                res[*res_len].value.var_idx = -x->left->root.var_idx - 1;
            }
            (*res_len)++;
            return res;
        }
        elif (x->left->type == Ast_dot_t) {
            /*
            a.b = c
            push a
            push c
            dot_set(b)
            */
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left->left, &left_len);
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len + 1));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            free(left);
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_dot_inc_t;
            res[*res_len].value.var_idx = x->left->right->root.var_idx;
            (*res_len)++;
            return res;
        }
        elif (x->left->type == Ast_colon_t) {
            /*
            a:b = c
            push a
            push b
            push c
            col_set
            */
            int ast_a_len = 0;
            Instruction *ast_a = ast_to_inst(x->left->left, &ast_a_len);
            int ast_b_len = 0;
            Instruction *ast_b = ast_to_inst(x->left->right, &ast_b_len);
            int ast_c_len = 0;
            Instruction *ast_c = ast_to_inst(x->right, &ast_c_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + ast_a_len + ast_b_len + ast_c_len + 1));
            for (int i = 0; i < ast_a_len; i++)
                res[(*res_len)++] = ast_a[i];
            free(ast_a);
            for (int i = 0; i < ast_b_len; i++)
                res[(*res_len)++] = ast_b[i];
            free(ast_b);
            for (int i = 0; i < ast_c_len; i++)
                res[(*res_len)++] = ast_c[i];
            free(ast_c);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_col_inc_t;
            (*res_len)++;
            return res;
        }
        else {
            PRINT_ERR("ERROR cannot convert ast (+=) for left type %d on line %d\n", x->left->type, current_line);
            exit(1);
        }
    }
    elif (x->type == Ast_minus_assign_t) {
        if (x->left->type == Ast_varcall_idx_t) {
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 1));
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            if (x->left->root.var_idx >= 0) {
                res[*res_len].type = inst_S_var_dec_t;
                res[*res_len].value.var_idx = x->left->root.var_idx;
            }
            else {
                res[*res_len].type = inst_S_global_var_dec_t;
                res[*res_len].value.var_idx = -x->left->root.var_idx - 1;
            }
            (*res_len)++;
            return res;
        }
        elif (x->left->type == Ast_dot_t) {
            /*
            a.b = c
            push a
            push c
            dot_set(b)
            */
            int left_len = 0;
            Instruction *left = ast_to_inst(x->left->left, &left_len);
            int right_len = 0;
            Instruction *right = ast_to_inst(x->right, &right_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + right_len + left_len + 1));
            for (int i = 0; i < left_len; i++)
                res[(*res_len)++] = left[i];
            free(left);
            for (int i = 0; i < right_len; i++)
                res[(*res_len)++] = right[i];
            free(right);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_dot_dec_t;
            res[*res_len].value.var_idx = x->left->right->root.var_idx;
            (*res_len)++;
            return res;
        }
        elif (x->left->type == Ast_colon_t) {
            /*
            a:b = c
            push a
            push b
            push c
            col_set
            */
            int ast_a_len = 0;
            Instruction *ast_a = ast_to_inst(x->left->left, &ast_a_len);
            int ast_b_len = 0;
            Instruction *ast_b = ast_to_inst(x->left->right, &ast_b_len);
            int ast_c_len = 0;
            Instruction *ast_c = ast_to_inst(x->right, &ast_c_len);
            res = realloc(res, sizeof(Instruction) * (*res_len + ast_a_len + ast_b_len + ast_c_len + 1));
            for (int i = 0; i < ast_a_len; i++)
                res[(*res_len)++] = ast_a[i];
            free(ast_a);
            for (int i = 0; i < ast_b_len; i++)
                res[(*res_len)++] = ast_b[i];
            free(ast_b);
            for (int i = 0; i < ast_c_len; i++)
                res[(*res_len)++] = ast_c[i];
            free(ast_c);
            res[*res_len].facultative = 0;
            res[*res_len].line = -1;
            res[*res_len].type = inst_S_col_dec_t;
            (*res_len)++;
            return res;
        }
        else {
            PRINT_ERR("ERROR cannot convert ast (-=) for left type %d on line %d\n", x->left->type, current_line);
            exit(1);
        }
    }
    elif (x->type == Ast_not_t) {
        int right_len = 0;
        Instruction *right = ast_to_inst(x->right, &right_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 1));
        for (int i = 0; i < right_len; i++)
            res[(*res_len)++] = right[i];
        free(right);
        res[*res_len].facultative = 0;
        res[*res_len].line = -1;
        res[*res_len].type = inst_S_op_t;
        res[*res_len].value.op = inst_S_op_not_t;
        (*res_len)++;
        return res;
    }
    // this is handled in funccal part 
    elif (x->type == Ast_unpack_t) {
        PRINT_ERR("cannot unpack ($) value outside funccall on line %d\n", current_line);
        exit(1);
    }
    elif (x->type == Ast_unpack_t) {
        int right_len = 0;
        Instruction *right = ast_to_inst(x->right, &right_len);
        res = realloc(res, sizeof(Instruction) * (*res_len + right_len + 1));
        for (int i = 0; i < right_len; i++)
            res[(*res_len)++] = right[i];
        free(right);
        res[*res_len].facultative = 0;
        res[*res_len].line = -1;
        res[*res_len].type = inst_S_op_t;
        res[*res_len].value.op = inst_S_op_unpack_t;
        (*res_len)++;
        return res;
    }
    elif (x->type == Ast_anonym_func_t) {
        /*
        {INST}
        | call +2
        | jmp 1 + len(INST) (+ 2)
        | INST
        | (push nil)
        | (ret)
        */
        char need_ret = 0;
        if (x->root.ano_func->code[x->root.ano_func->code_len - 1].type != inst_return_t)
            need_ret = 1;
        Instruction *body = NULL;
        int body_len = x->root.ano_func->code_len;
        char old_is_ano = is_in_ano_func;
        is_in_ano_func = 1;
        body = finish_instrcutions(x->root.ano_func->code, &body_len);
        is_in_ano_func = old_is_ano;

        res = realloc(res, sizeof(Instruction) * (2 + *res_len));
        res[*res_len].type = inst_S_call_t; 
        res[*res_len].line = -1;
        res[*res_len].facultative = 0;
        res[(*res_len)++].value.jmp_length = 2;

        res[*res_len].type = inst_S_rjmp_t; 
        res[*res_len].line = -1;
        res[*res_len].facultative = 0;
        res[(*res_len)++].value.jmp_length = 1 + body_len + (need_ret ? 2 : 0);
        res = realloc(res, sizeof(Instruction) * (*res_len + body_len));
        for (int i = 0; i < body_len; i++)
            res[(*res_len)++] = body[i];
        if (need_ret) {
            res = realloc(res, sizeof(Instruction) * (2 + *res_len));
            res[*res_len].type = inst_S_push_nil_t; 
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
            res[*res_len].type = inst_S_ret_t; 
            res[*res_len].line = -1;
            res[*res_len].facultative = 0;
            (*res_len)++;
        }
        return res;
    }
    else {
        PRINT_ERR("cannot convert ast of type %d to bytecode on line %d\n", x->type, current_line);
        exit(1);
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
