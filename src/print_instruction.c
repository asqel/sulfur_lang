#include <string.h>
#include <stdio.h>
#include "../include/Ast.h"
#include "../include/instruction.h"

void instruction_print(Instruction code);
void instructions_print(Instruction* code, int code_len);

#define elif else if

void instruction_print(Instruction code){
    if(code.type == inst_varset_t){
        printf("VS{name : %s type: %s val:\n    ",code.value.vs->name, code.value.vs->type);
        print_ast(*code.value.vs->val);
        printf("\n}");
    }
    elif(code.type == inst_new_varset_t){
        printf("NVS{name : %s val:\n    ",code.value.vs->name);
        print_ast(*code.value.vs->val);
        printf("\n}");
    }
    elif(code.type == inst_endif){
       printf("ENDIF %d\n", code.value.endif);
    }
    elif(code.type == inst_endifelse){
       printf("ENDIFELSE %d\n",code.value.endifelse);
    }
    elif(code.type == inst_if_t){
        printf("IF %d{\n    ", code.value.i->endif_p);
        print_ast(*code.value.i->condition);
        printf("\n}");
    }
    elif(code.type == inst_elif_t){
        printf("ELIF %d{\n    ", code.value.el->endif_p);
        print_ast(*code.value.el->condition);
        printf("\n}");
    }
    elif(code.type == inst_else_t){
        printf("ELSE\n");
    }
    elif(code.type == inst_expr_t){
        printf("EXPR{\n    ");
        print_ast(*code.value.expr);
        printf("\n}");
    }
    elif(code.type == inst_endfor_t){
       printf("ENDFOR %d\n", code.value.endfor);
    }
    elif(code.type == inst_endwhile_t){
       printf("ENDWHILE %d\n", code.value.endwhile);
    }
    elif(code.type == inst_for_t){
        printf("FOR %d{start\n    ", code.value.fo->endfor);
        print_ast(*code.value.fo->start);
        printf("\nend \n    ");
        print_ast(*code.value.fo->end);
        printf("\nname : %s",code.value.fo->var_name);
        printf("\n}");
    }
    elif(code.type == inst_while_t){
        printf("WHILE %d{\n    ", code.value.wh->endwhile);
        print_ast(*code.value.wh->condition);
        printf("\n}");
    }
    elif(code.type == inst_return_t){
        printf("RETUNR{\n    ");
        print_ast(*code.value.ret);
        printf("\n}");
    }
    elif(code.type == inst_swap_t){
        printf("SWAP");
    }
    elif(code.type == inst_section_t){
        printf("SECTION : %s",code.value.section);
    }
    elif(code.type == inst_goto_t){
        printf("GOTO : %s", code.value.goto_sec);
    }
    elif(code.type == inst_pass_t){
        printf("PASS");
    }
    elif (code.type == inst_jmp_t) {
        printf("JMP : %d", code.value.jmp);
    }
    elif(code.type == inst_funcdef_t){
        printf("FUNCDEF : %s (", code.value.fc->info.name);
        if (code.value.fc->args_mod == '+') {
            for(int i = 0; i < code.value.fc->args_len - 1; i++) {
                if (i != 0) printf(" ");
                printf("%s", code.value.fc->args[i]);
            }
            printf(" $%s", code.value.fc->args[code.value.fc->args_len - 1]);
        }
        else {
            for(int i = 0; i < code.value.fc->args_len; i++) {
                if (i != 0) printf(" ");
                printf("%s", code.value.fc->args[i]);
            }
        }
        printf("){\n");
        instructions_print(code.value.fc->code, code.value.fc->code_len);
        printf("}");
    }
    elif(code.type == inst_proceed_t){
        printf("PROCEED");
    }
    elif(code.type == inst_stop_t){
        printf("STOP");
    }
    elif (code.type == inst_S_push_nil_t) {
        printf("PUSH nil");
    }
    elif (code.type == inst_pass_t) {
        printf("pass");
    }
    elif (code.type == inst_S_rjmp_uid_t) {
        printf("rjmp %d", code.value.jmp);
    }
    elif (code.type == inst_S_rjmpif_uid_t) {
        printf("rjmpif %d", code.value.jmp);
    }
    elif (code.type == inst_S_rjmpifn_uid_t) {
        printf("rjmpifn %d", code.value.jmp);
    }
    elif (code.type == inst_S_push_0b_t) {
        printf("PUSH 0b");
    }
    elif (code.type == inst_S_push_1b_t) {
        printf("PUSH 1b");
    }
    elif (code.type == inst_S_push_ount_t) {
        printf("PUSH %d", code.value.ount);
    }
    elif (code.type == inst_S_push_floap_t) {
        printf("PUSH %f", code.value.floap);
    }
    elif (code.type == inst_S_push_i_t) {
        printf("PUSH $i");
    }
    elif (code.type == inst_S_pop_t) {
        printf("POP");
    }
    elif (code.type == inst_S_op_t) {
	    if (code.value.op == inst_S_op_add_t)
            printf("add");
	    elif (code.value.op == inst_S_op_sub_t)
            printf("sub");
	    elif (code.value.op == inst_S_op_mul_t)
            printf("mul");
	    elif (code.value.op == inst_S_op_div_t)
            printf("div");
	    elif (code.value.op == inst_S_op_fldiv_t)
            printf("fldiv");
	    elif (code.value.op == inst_S_op_mod_t)
            printf("mod");
	    elif (code.value.op == inst_S_op_eq_t)
            printf("eq");
	    elif (code.value.op == inst_S_op_ge_t)
            printf("ge");
	    elif (code.value.op == inst_S_op_geq_t)
            printf("geq");
	    elif (code.value.op == inst_S_op_le_t)
            printf("le");
	    elif (code.value.op == inst_S_op_leq_t)
            printf("leq");
	    elif (code.value.op == inst_S_op_lshift_t)
            printf("lshift");
	    elif (code.value.op == inst_S_op_rshift_t)
            printf("rshift");
	    elif (code.value.op == inst_S_op_noteq_t)
            printf("noteq");
        elif (code.value.op == inst_S_op_negate_t)
            printf("negate");
        elif (code.value.op == inst_S_op_pow_t)
            printf("pow");
        else
            printf("OP INVALID (%d)", code.value.op);
    }
    elif (code.type == inst_S_push_var_t)
        printf("push $(%d)", code.value.var_idx);
    elif (code.type == inst_S_push_global_var_t)
        printf("push #(%d)", code.value.var_idx);
    else
        printf("inst INVALID (%d)", code.type);
}

void instructions_print(Instruction* code, int code_len){
    for(int i = 0; i < code_len; i++){
        printf("%d ",i);
        instruction_print(code[i]);
        printf("\n");
    }
}
