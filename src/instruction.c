#include "../include/instruction.h"
#include "../include/utilities.h"

int inst_to_str(Instruction*i){
    if(i->type==inst_for_t){

        printf("For:[%s,%d]",i->value.fo->var_name,i->value.fo->endfor);
        return 1;
    }
    if(i->type==inst_endfor_t){
        printf("Endfor:[%d]",i->value.endfor);
    }
    if(i->type==inst_expr_t){
       printf("Expr:[]");
    }
}

void instruction_free_array(Instruction *code, int len){
    for(int i = 0; i < len; i++){
        instruction_free(code[i]);
    }
    free(code);
}

void instruction_free(Instruction code){
    switch (code.type){
        case inst_varset_t:
            free(code.value.vs->name);
            free(code.value.vs->type);
            free_ast(*code.value.vs->val);
            free(code.value.vs->val);
            //TODO other types and make free_ast function
        default:
            break;
    }

}
