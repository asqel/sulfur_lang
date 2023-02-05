
#include "Ast.h"



int Ast_has_r(Ast x){
    return x.right!=NULL;
}

int Ast_has_l(Ast x){
    return x.left!=NULL;
}
