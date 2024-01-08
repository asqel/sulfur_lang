#include <string.h>
#include <stdio.h>
#include "../include/Ast.h"
#include "../include/instruction.h"

void instruction_print(Instruction code);
void instructions_print(Instruction* code, int code_len);


void instruction_print(Instruction code){
    if(code.type == inst_varset_t){
        printf("VS{name : %s type: %s val:\n    ",code.value.vs->name, code.value.vs->type);
        print_ast(*code.value.vs->val);
        printf("\n}");
    }
    if(code.type == inst_new_varset_t){
        printf("NVS{name : %s val:\n    ",code.value.vs->name);
        print_ast(*code.value.vs->val);
        printf("\n}");
    }
    if(code.type == inst_endif){
       printf("ENDIF %d\n", code.value.endif);
    }
    if(code.type == inst_endifelse){
       printf("ENDIFELSE %d\n",code.value.endifelse);
    }
    if(code.type == inst_if_t){
        printf("IF %d{\n    ", code.value.i->endif_p);
        print_ast(*code.value.i->condition);
        printf("\n}");
    }
    if(code.type == inst_elif_t){
        printf("ELIF %d{\n    ", code.value.el->endif_p);
        print_ast(*code.value.el->condition);
        printf("\n}");
    }
    if(code.type == inst_else_t){
        printf("ELSE\n");
    }
    if(code.type == inst_expr_t){
        printf("EXPR{\n    ");
        print_ast(*code.value.expr);
        printf("\n}");
    }
    if(code.type == inst_endfor_t){
       printf("ENDFOR %d\n", code.value.endfor);
    }
    if(code.type == inst_endwhile_t){
       printf("ENDWHILE %d\n", code.value.endwhile);
    }
    if(code.type == inst_for_t){
        printf("FOR %d{start\n    ", code.value.fo->endfor);
        print_ast(*code.value.fo->start);
        printf("\nend \n    ");
        print_ast(*code.value.fo->end);
        printf("\nname : %s",code.value.fo->var_name);
        printf("\n}");
    }
    if(code.type == inst_while_t){
        printf("WHILE %d{\n    ", code.value.wh->endwhile);
        print_ast(*code.value.wh->condition);
        printf("\n}");
    }
    if(code.type == inst_return_t){
        printf("RETUNR{\n    ");
        print_ast(*code.value.ret);
        printf("\n}");
    }
    if(code.type == inst_swap_t){
        printf("SWAP");
    }
    if(code.type == inst_section_t){
        printf("SECTION : %s",code.value.section);
    }
    if(code.type == inst_goto_t){
        printf("GOTO : %s", code.value.goto_sec);
    }
    if(code.type == inst_pass_t){
        printf("PASS");
    }
    if (code.type == inst_jmp_t) {
        printf("JMP : %d", code.value.jmp);
    }
    if(code.type == inst_funcdef_t){
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
    if(code.type == inst_proceed_t){
        printf("PROCEED\n");
    }
    if(code.type == inst_stop_t){
        printf("STOP\n");
    }
}


void instructions_print(Instruction* code, int code_len){
    for(int i = 0; i < code_len; i++){
        printf("%d ",i);
        instruction_print(code[i]);
        printf("\n");
    }
}
