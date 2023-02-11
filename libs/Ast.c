
#include "Ast.h"



int Ast_has_r(Ast x){
    return x.right!=NULL;
}

int Ast_has_l(Ast x){
    return x.left!=NULL;
}


int Ast_len(Ast*x){
    int n=0;
    for(int i=0;x[i].type!=Ast_end_t;i++){
        n++;
    }
    return n;
}
