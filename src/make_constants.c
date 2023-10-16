#include "../include/make_constants.h"
#include "../include/instruction.h"

Object *constants = NULL;
int constants_len = 0;

void make_constants_in_ast(Ast *x){

}

void make_constants(Instruction *code, int code_len){
    for (int i = 0; i < code_len; i++){
        if (code[i].type == inst_if_t){
            make_constants_in_ast(code->value.i->condition);
        }
        else if (code[i].type == inst_elif_t){
            make_constants_in_ast(code->value.el->condition);
        }
        else if (code[i].type == inst_while_t){
            make_constants_in_ast(code->value.wh->condition);
        }
        else if (code[i].type == inst_for_t){
            make_constants_in_ast(code->value.fo->end);
        }
    }
}