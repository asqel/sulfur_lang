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
