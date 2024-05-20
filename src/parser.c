#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/token_class.h"
#include "../include/Ast.h"
#include "../include/utilities.h"
#include "../include/make_if.h"
#include "../include/make_for.h"
#include "../include/make_return.h"
#include "../include/make_while.h"
#include "../include/make_funcdef.h"
#include "../include/make_jmp_links.h"

//take a math expression and see if it ok
//return nothing 
//print error and exit(1)
void check_syntax(Ast*x){
    


}

int line = -1; 

ast_and_len tok_to_Ast(Token*tok,int start,int end){
    int len=end-start+1;
    Ast*e=malloc(sizeof(Ast)*(len));
    int offset=0;
    for(int i=start;i<end;i++){
        e[i-start-offset].left=NULL;
        e[i-start-offset].right=NULL;
        e[i-start-offset].type=Ast_object_t;
        e[i-start-offset].isAst=0;
        switch(tok[i].type){//techniquement faudrait mettre aussi les token end
            case nil:
                e[i-start-offset].type=Ast_object_t;
                e[i-start-offset].root.obj->type=Obj_nil_t;
                break;
            case ount:
                e[i-start-offset].root.obj=malloc(sizeof(Object));
                e[i-start-offset].type=Ast_object_t;
                e[i-start-offset].root.obj->type=Obj_ount_t;
                e[i-start-offset].root.obj->val.i=*tok[i].value.i;
                break;
            case str:
                e[i-start-offset].root.obj=malloc(sizeof(Object));
                e[i-start-offset].type=Ast_object_t;
                e[i-start-offset].root.obj->type=Obj_string_t;
                e[i-start-offset].root.obj->val.s = uti_strdup(tok[i].value.s);
                break;
            case floap:
                e[i-start-offset].root.obj=malloc(sizeof(Object));
                e[i-start-offset].type=Ast_object_t;
                e[i-start-offset].root.obj->type=Obj_floap_t;
                e[i-start-offset].root.obj->val.f=*tok[i].value.f;
                break;
            case boolean_t:
                e[i-start-offset].root.obj=malloc(sizeof(Object));
                e[i-start-offset].type=Ast_object_t;
                e[i-start-offset].root.obj->type=Obj_boolean_t;
                e[i-start-offset].root.obj->val.b=*tok[i].value.b;
                break;
            case comp:
                e[i-start-offset].root.obj=malloc(sizeof(Object));
                e[i-start-offset].type=Ast_object_t;
                e[i-start-offset].root.obj->type=Obj_complex_t;
                e[i-start-offset].root.obj->val.c[0]=tok[i].value.c[0];
                e[i-start-offset].root.obj->val.c[1]=tok[i].value.c[1];
                break;
            case op:
                e[i-start-offset].type=Ast_op_t;
                e[i-start-offset].root.op=*tok[i].value.t;
                break;
            case keyword:
                    e[i-start-offset].type=Ast_keyword_t;
                    e[i-start-offset].root.kw=*tok[i].value.t;
                    break;
                
            case syntax:
                if(*tok[i].value.t == r_brack_L){
                    int op_brac = i;
                    int cl_brac = search_rrbrack(tok, i);
                    if (cl_brac == -1){
                        printf("ERROR missing closing '}' in expression on line %d\n",tok[i].line);
                        exit(1);
                    }
                    Token* to_parse = malloc(sizeof(Token) * (cl_brac - op_brac -1 +1));
                    for(int k = op_brac + 1; k < cl_brac; k++){
                        to_parse[k-(op_brac	+ 1)] = tok[k];
                    }
                    to_parse[cl_brac - op_brac -1] = end_token;
                    Instruction* code=malloc(sizeof(Instruction));
                    int code_len = 0;
                    code = parse(to_parse,-1,-1,code,&code_len);
    				code = make_jmp_links(code, code_len);

                    len-= (token_len(to_parse)+1);
                    e=realloc(e,sizeof(Ast)*len);
                    e[i - start-offset].type = Ast_anonym_func_t;
                    e[i - start-offset].isAst=1;
                    e[i - start-offset].root.ano_func = malloc(sizeof(anonym_func));
                    e[i - start-offset].root.ano_func->code = code;
                    e[i - start-offset].root.ano_func->code_len = code_len;
                    offset+=cl_brac-i;
                    i=cl_brac;

                    free(to_parse);

                    break;
                }
                else{
                    e[i-start-offset].type=Ast_syntax_t;
                    e[i-start-offset].root.sy=*tok[i].value.t;
                    break;
                }
            case identifier:
                e[i-start-offset].type=Ast_varcall_t;
                e[i-start-offset].root.varcall=malloc(sizeof(char)*(1+strlen(tok[i].value.s)));
                strcpy(e[i-start-offset].root.varcall,tok[i].value.s);
                break;
            default:
                break;
        }
    }
    return (ast_and_len){.value = e , .len=len-1};
}


int find_highest_op(Ast*e,int len){
    //search for .(dot) / :(colon)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_syntax_t&&e[i].root.sy==dot&&!e[i].isAst){//en theorie ya pasa besoin de verifier isAst
            return i;
        }
        if(e[i].type == Ast_syntax_t && e[i].root.sy == colon && !e[i].isAst){
            return i;
        }
    }
    //search for **(pow)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&& e[i].root.op==OP_EXPONENT&&!e[i].isAst){
            return i;
        }
    }
    //search for *(mul) /(div) \(floordiv) %(mod)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_MULTIPLY||e[i].root.op==OP_DIVIDE||e[i].root.op==OP_FLOOR_DIVIDE||e[i].root.op==OP_MODULUS){
                return i;
            }
        }
    }
    //search for +(plus) -(minus)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_PLUS||e[i].root.op==OP_MINUS){
                return i;
            }
        }
    }
    //search for >>(rshift) <<(lshift)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_RSHIFT||e[i].root.op==OP_LSHIFT){
                return i;
            }
        }
    }
    //search for <(less) >(greater) <=(less_eq) >=(greater_eq)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_LESS||e[i].root.op==OP_LESS_EQUAL||e[i].root.op==OP_GREATER||e[i].root.op==OP_GREATER_EQUAL){
                return i;
            }
        }
    }
    //search for ==(eq) !=(not_eq)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_EQUAL||e[i].root.op==OP_NOT_EQUAL){
                return i;
            }
        }
    }
    //search for ^(bit xor)
    for (int i = 0; i < len; i++) {
        if(e[i].type == Ast_op_t &&! e[i].isAst){
            if(e[i].root.op==OP_BIT_XOR){
                return i;
            }
        }
    }
    //search for &&(and)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_AND){
                return i;
            }
        }
    }
    //search for ||(or)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_OR){
                return i;
            }
        }
    }
    //search for =(assign) +=(plus_assign) -=(minus_assign)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_ASSIGN){
                return i;
            }
            if(e[i].root.op==OP_PLUS_ASSIGN){
                return i;
            }
            if(e[i].root.op==OP_MINUS_ASSIGN){
                return i;
            }
        }
        
    }
    return -1;

}

int count_comma(Ast*e,int len){
    int com_c=0;
    int i=0;
    while(i<len){
        if(e[i].type==Ast_syntax_t && e[i].root.sy==comma){
            com_c++;
            i++;
            continue;
        }
        if(e[i].type==Ast_varcall_t && i+1<len && e[i+1].type==syntax && e[i+1].root.sy==par_L){
            int op_par=i+1;
            int cl_par=-1;
            int count=0;
            for(int k=op_par+1;k<len;k++){
                if(e[k].type==syntax && e[k].root.sy==par_L){
                    count++;
                    continue;
                }
                if(e[k].type==syntax && e[k].root.sy==par_R && count>0){
                    count--;
                    continue;
                }
                if(e[k].type==syntax && e[k].root.sy==par_R && count==0){
                    cl_par=k;
                    break;
                }
            }
            if(cl_par==-1){
                printf("ERROR missing closing ')' in expression on function call\n");
                exit(1);
            }
            i=cl_par+1;
            continue;
        }
        else{
            i++;
        }
    }
    return com_c;
}

typedef struct parse_arg{
    Ast*v;
    int l;
}parse_arg;

Ast *check_do_what_here_why(Ast *x, int *why) {
    if (x->type == Ast_object_t) {
        if ((*x).root.obj->type == Obj_ount_t && (*x).root.obj->val.i == 42) {
            Ast *res = malloc(sizeof(Ast));
            res->type = Ast_funccall_t;
            res->isAst = 1;
            res->root.fun = malloc(sizeof(funccall));
            res->root.fun->name = uti_strdup("println");
            res->root.fun->args = malloc(sizeof(Ast));
            res->root.fun->nbr_arg = 1;
            res->root.fun->args[0].type = Ast_object_t;
            res->root.fun->args[0].isAst = 0;
            res->root.fun->args[0].right = NULL;
            res->root.fun->args[0].left = NULL;
            res->root.fun->args[0].root.obj = malloc(sizeof(Object));
            res->root.fun->args[0].root.obj->type = Obj_string_t;
            res->root.fun->args[0].root.obj->val.s = uti_strdup("the answer to life the universe and everything");
            free_ast(*x);
            free(x);
            *why = 1;
            return res;
        }
        else if ((*x).root.obj->type == Obj_ount_t && (*x).root.obj->val.i == 69) {
            Ast *res = malloc(sizeof(Ast));
            res->type = Ast_funccall_t;
            res->isAst = 1;
            res->root.fun = malloc(sizeof(funccall));
            res->root.fun->name = uti_strdup("println");
            res->root.fun->args = malloc(sizeof(Ast));
            res->root.fun->nbr_arg = 1;
            res->root.fun->args[0].type = Ast_object_t;
            res->root.fun->args[0].isAst = 0;
            res->root.fun->args[0].right = NULL;
            res->root.fun->args[0].left = NULL;
            res->root.fun->args[0].root.obj = malloc(sizeof(Object));
            res->root.fun->args[0].root.obj->type = Obj_string_t;
            res->root.fun->args[0].root.obj->val.s = uti_strdup("nice");
            free_ast(*x);
            free(x);
            *why = 1;
            return res;
        }
    }
    *why = 0;
    return x;
}


//start included
//end not included
//there no check if end and start are out of range
//when finished the operator is stored in the ast.type as Ast_(op)_t ex: Ast_add_t
Ast*make_ast(Ast*e,int len){
    //e contains only tokens turn into ast from start to end(not included) 
    check_syntax(e);


    //make funccall 'function(...)'
    int p=0;
    while(p<len){
        if(e[p].type==Ast_varcall_t&&p+1<len){
            if(e[p+1].type==Ast_syntax_t && e[p+1].root.sy==par_L){
                int opening_par=p+1;
                int closing_par=-1;
                //search closing par
                int count=0;
                for(int i=opening_par+1;i<len;i++){
                    if(e[i].type==Ast_syntax_t&&e[i].root.sy==par_L){
                        count++;
                    }
                    else if(e[i].type==Ast_syntax_t&&e[i].root.sy==par_R&& count>0){
                        count--;
                    }
                    else if(e[i].type==Ast_syntax_t&&e[i].root.sy==par_R&& count==0){
                        closing_par=i;
                        break;
                    }
                }

                if(closing_par==-1){
                    printf("ERROR missing closing ')' in function call\n");
                    exit(1);
                }
                if(closing_par==opening_par+1){//funccall without arg
                    funccall f;
                    f.args=NULL;
                    f.nbr_arg=0;
                    f.name=e[opening_par-1].root.varcall;
                    for(int i=closing_par+1;i<len;i++){
                        e[i-2]=e[i];
                    }
                    len-=2;
                    e=realloc(e,sizeof(Ast)*len);
                    e[opening_par-1].type=Ast_funccall_t;
                    e[opening_par-1].isAst=1;
                    e[opening_par-1].root.fun=malloc(sizeof(funccall));
                    *e[opening_par-1].root.fun=f;
                    p++;
                    continue;
                }
                Ast*to_parse=malloc(sizeof(Ast)*(closing_par-opening_par-1));
                int to_parse_len=closing_par-opening_par-1;
                for(int i=opening_par+1;i<closing_par;i++){
                    to_parse[i-(opening_par+1)]=e[i];
                }
                if(count_comma(to_parse,to_parse_len)==0){
                    Ast*o=make_ast(to_parse,to_parse_len);
                    funccall f;
                    f.args=o;
                    f.name=e[opening_par-1].root.varcall;
                    f.nbr_arg=1;
                    for(int i=closing_par+1;i<len;i++){
                        e[i-(closing_par+1)+opening_par]=e[i];
                    }

                    len-=(to_parse_len+2);
                    e=realloc_c(e,sizeof(Ast)*(len+(to_parse_len+2)),sizeof(Ast)*len);
                    e[opening_par-1].type=Ast_funccall_t;
                    e[opening_par-1].isAst=1;
                    e[opening_par-1].root.fun=malloc(sizeof(funccall));
                    *e[opening_par-1].root.fun=f;
                    p++;
                    continue;
                }
                else{
                    parse_arg*a=malloc(sizeof(parse_arg));
                    int k=0;
                    a[0].l=0;
                    a[0].v=malloc(sizeof(Ast));
                    for(int i=0;i<to_parse_len;i++){
                        if(to_parse[i].type==Ast_syntax_t && to_parse[i].root.sy==comma){
                            k++;
                            a=realloc(a,sizeof(parse_arg)*(k+1));
                            a[k].l=0;
                            a[k].v=malloc(sizeof(Ast));
                            continue;

                        }
                        if(to_parse[i].type==Ast_varcall_t && i+1<to_parse_len){
                            if(to_parse[i+1].type==Ast_syntax_t && to_parse[i+1].root.sy==par_L){
                                int op_par=i+1;
                                int cl_par=-1;
                                int count=0;
                                for(int u=op_par+1;u<to_parse_len;u++){
                                    if(to_parse[u].type==Ast_syntax_t && to_parse[u].root.sy==par_L){
                                        count++;
                                        continue;
                                    }
                                    if(to_parse[u].type==Ast_syntax_t && to_parse[u].root.sy==par_R && count>0){
                                        count--;
                                        continue;
                                    }
                                    if(to_parse[u].type==Ast_syntax_t && to_parse[u].root.sy==par_R && count==0){
                                        cl_par=u;
                                        break;
                                    }
                                }
                                if(cl_par==-1){
                                    printf("ERROR missing closing ')' in expression on function call\n");
                                    exit(1);
                                }
                                int l=cl_par-op_par+2;
                                a[k].l+=l;
                                a[k].v=realloc(a[k].v,sizeof(Ast)*a[k].l);
                                for(int j=op_par-1;j<=cl_par;j++){
                                    a[k].v[j-(op_par-1)+a[k].l-l]=to_parse[j];
                                }
                                i=cl_par;
                                continue;
                            }
                        }
                        a[k].l++;
                        a[k].v=realloc(a[k].v,sizeof(Ast)*(a[k].l));
                        a[k].v[a[k].l-1]=to_parse[i];
                    }
                    free(to_parse);

                    funccall f;
                    f.args=malloc(sizeof(Ast)*(k+1));
                    Ast*tmp;
                    for(int i=0;i<=k;i++){
                        tmp=make_ast(a[i].v, a[i].l);
                        f.args[i]=*tmp;
                        free(tmp);
                    }
                    f.name=e[opening_par-1].root.varcall;
                    f.nbr_arg=k+1;
                    for(int i=closing_par+1;i<len;i++){
                        e[i-(closing_par+1)+opening_par]=e[i];
                    }

                    len-=closing_par-opening_par+1;
                    e=realloc(e,sizeof(Ast)*len);
                    e[opening_par-1].type=Ast_funccall_t;
                    e[opening_par-1].isAst=1;
                    e[opening_par-1].root.fun=malloc(sizeof(funccall));
                    *e[opening_par-1].root.fun=f;
                    p++;
                    free(a);
                    continue;
                }
            }
            else{
                p++;
            }
        }
        else{
            p++;
        }
    }

    //make expr '(...)'
    p=0;
    while(p<len){
        if(e[p].type==Ast_syntax_t&&e[p].root.sy==par_L){
            int op_par=p;
            int cl_par=-1;
            int c=0;
            for(int i=op_par+1;i<len;i++){
                if(e[i].type==Ast_syntax_t && e[i].root.sy==par_L){
                    c++;
                    continue;
                }
                if(e[i].type==Ast_syntax_t && e[i].root.sy==par_R && c>0){
                    c--;
                    continue;
                }
                if(e[i].type==Ast_syntax_t && e[i].root.sy==par_R && c==0){
                    cl_par=i;
                    break;
                }
            }
            if(cl_par==-1){

                printf("ERROR missing closing ')' in expression\n");
                exit(1);
            }
            int to_parse_len=cl_par-op_par-1;
            Ast*to_parse=malloc(sizeof(Ast)*(to_parse_len));
            for(int i=op_par+1;i<cl_par;i++){
                to_parse[i-(op_par+1)]=e[i];
            }

            Ast*expr=make_ast(to_parse,to_parse_len);
            // free(to_parse);
            e[op_par]=*expr;
            free(expr);
            e[op_par].isAst=1;
            for(int i=cl_par+1;i<len;i++){
                e[i-(cl_par+1)+op_par+1]=e[i];
            }
            len-=(to_parse_len+1);
            e=realloc(e,sizeof(Ast)*len);
            p++;
        }
        else{
            p++;
        }

    }
    //TODO here:list sub [...]


    //remove unary ++ -> +  -- -> + !! -> ' '  -> +- -> -  -+ ->
    p=0;
    while (p<len){
        if(e[p].type==Ast_op_t&&e[p].root.op==OP_PLUS){
            if(p+1<len&&e[p+1].type==Ast_op_t&&e[p+1].root.op==OP_PLUS){//++
                for(int i=p+1;i<len;i++){
                    e[i-1]=e[i];
                }
                len--;
                e=realloc(e,sizeof(Ast)*len);
            }
            else if(p+1<len&&e[p+1].type==Ast_op_t&&e[p+1].root.op==OP_MINUS){//+-
                for(int i=p+1;i<len;i++){
                    e[i-1]=e[i];
                }
                len--;
                e=realloc(e,sizeof(Ast)*len);
            }
        }

        else if(e[p].type==Ast_op_t&&e[p].root.op==OP_MINUS){
            if(p+1<len&&e[p+1].type==Ast_op_t&&e[p+1].root.op==OP_MINUS){//--
                e[p+1].root.op=OP_PLUS;
                for(int i=p+1;i<len;i++){
                    e[i-1]=e[i];
                }
                len--;
                e=realloc(e,sizeof(Ast)*len);
            }
            else if(p+1<len&&e[p+1].type==Ast_op_t&&e[p+1].root.op==OP_PLUS){//-+
                e[p+1].root.op=OP_PLUS;
                for(int i=p+1;i<len;i++){
                    e[i-1]=e[i];
                }
                len--;
                e=realloc(e,sizeof(Ast)*len);
            }
        }
        else if(e[p].type==Ast_op_t&&e[p].root.op==OP_NOT){
            if(p+1<len&&e[p+1].type==Ast_op_t&&e[p+1].root.op==OP_NOT){
                for(int i=p+2;i<len;i++){
                    e[i-2]=e[p];
                }
                len-=2;
                e=realloc(e,sizeof(Ast)*len);
            }
        }

        p++;
    }
     
    //make unary + - ! $
    p=0;
    while(p<len){
        if(e[p].type==Ast_op_t && e[p].root.op==OP_NOT){// !
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '!' in expression\n");
                exit(1);
            }
            e[p].isAst=1;
            e[p].type=Ast_not_t;
            e[p].right=malloc(sizeof(Ast));
            *e[p].right=e[p+1];
            for(int i=p+2;i<len;i++){
                e[i-1]=e[i];
            }
            len--;
            e=realloc(e,sizeof(Ast)*len);
            p++;
        }

        else if(e[p].type==Ast_op_t && e[p].root.op==OP_PLUS&&p-1>=0&&(e[p-1].type==Ast_op_t || (e[p-1].type == Ast_syntax_t && e[p-1].root.sy == colon))){
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '+'(unary) in expression\n");
                exit(1);
            }
            e[p].isAst=1;
            e[p].type=Ast_add_t;
            e[p].left=NULL;
            e[p].right=malloc(sizeof(Ast));
            *e[p].right=e[p+1];
            for(int i=p+2;i<len;i++){
                e[i-1]=e[i];
            }
            len--;
            e=realloc(e,sizeof(Ast)*len);
            p++;
        }

        if(e[p].type==Ast_op_t && e[p].root.op==OP_UNPACK){// !
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '$' in expression\n");
                exit(1);
            }
            e[p].isAst=1;
            e[p].type=Ast_unpack_t;
            e[p].right=malloc(sizeof(Ast));
            *e[p].right=e[p+1];
            for(int i=p+2;i<len;i++){
                e[i-1]=e[i];
            }
            len--;
            e=realloc(e,sizeof(Ast)*len);
            p++;
        }

        else if(e[p].type==Ast_op_t && e[p].root.op==OP_MINUS&&p-1>=0&&(e[p-1].type==Ast_op_t || (e[p-1].type == Ast_syntax_t && e[p-1].root.sy == colon))){
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '-'(unary) in expression\n");
                exit(1);
            }
            e[p].isAst=1;
            e[p].type=Ast_sub_t;
            e[p].left=NULL;
            e[p].right=malloc(sizeof(Ast));
            *e[p].right=e[p+1];
            for(int i=p+2;i<len;i++){
                e[i-1]=e[i];
            }
            len--;
            e=realloc(e,sizeof(Ast)*len);
            p++;
        }

        else if(e[p].type==Ast_op_t && e[p].root.op==OP_MINUS && p==0){
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '-'(unary) in expression\n");
                exit(1);
            }
            e[p].isAst=1;
            e[p].type=Ast_sub_t;
            e[p].left=NULL;
            e[p].right=malloc(sizeof(Ast));
            *e[p].right=e[p+1];
            for(int i=p+2;i<len;i++){
                e[i-1]=e[i];
            }
            len--;
            e=realloc(e,sizeof(Ast)*len);
            p++;

        }
        else{
            p++;
        }   
    }

    //make operators
    p=0;
    while(len>1){
       int n=find_highest_op(e,len);
        if(n==-1){
            printf("ERROR in expression missing operator on line %d\n",line);
            exit(1);
        }
        if(n-1<0){
            printf("ERROR missing left operand in expression on line %d on '%s' operator\n",line, get_op_str(e[n]));
            exit(1);
        }
        if(n+1>=len){
            printf("ERROR missing right operand in expression on line %d on '%s' operator\n",line, get_op_str(e[n]));
            exit(1);
        }
        if(!(e[n-1].isAst || e[n-1].type==Ast_object_t || e[n-1].type==Ast_varcall_t || e[n-1].type == Ast_anonym_func_t)){
            printf("ERROR missing left operand in expression on line %d on '%s' operator\n",line, get_op_str(e[n]));
            exit(1);
        }
        if(!(e[n+1].isAst || e[n+1].type==Ast_object_t || e[n+1].type==Ast_varcall_t || e[n+1].type == Ast_anonym_func_t)){
            printf("ERROR missing right operand in expression on line %d on '%s' operator\n",line, get_op_str(e[n]));
            exit(1);
        }
        Ast op_ast;
        op_ast.isAst=1;
        op_ast.type=op_tok_to_op_ast(e[n].type==Ast_syntax_t?e[n].root.sy:e[n].root.op,e[n].type);;
        op_ast.right=malloc(sizeof(Ast));
        op_ast.left=malloc(sizeof(Ast));

        *op_ast.right=e[n+1];
        *op_ast.left=e[n-1];
        Ast*e2=malloc(sizeof(Ast)*(len-2));
        for(int i=0;i<n;i++){
            e2[i]=e[i];
        }
        for(int i=n+2;i<len;i++){
            e2[i-2]=e[i];
        }
        len-=2;
        e2[n-1]=op_ast;
        //utiliser le realloc fait que certain valeur de certaine struct se modifie donc bah non 
        free(e);
        e=e2;
    }
    return e;
}


int find_semicol(Token*tok,int p){
    int len=token_len(tok);
    for(int i=0;i+p<len;i++){
        if(tok[i+p].type == syntax && *tok[i+p].value.t == r_brack_L){
            int cl_brack = search_rrbrack(tok,i+p);
            if (cl_brack == -1){
                printf("missing closing '}' in expression on line %d \n",tok[i+p].line);
                exit(1);
            }
            i=cl_brack-p;
        }
        if(tok[i+p].type==syntax&&*tok[i+p].value.t==semicolon){
            return i+p;
        }
    }
    return -1;
}

int search_rpar(Token*t,int start){
    int len=token_len(t);
    int k=0;
    for(int i=start+1;i<len;i++){
        if(t[i].type==syntax&&*t[i].value.t==par_L){
            k++;
        }
        else if(t[i].type==syntax&&*t[i].value.t==par_R&&k>0){
            k--;
        }
        else if(t[i].type==syntax&&*t[i].value.t==par_R&&k==0){
            return i;
        }
    }
    return -1;
}

int search_rrbrack(Token*t,int start){
    int len=token_len(t);
    int k=0;
    for(int i=start+1;i<len;i++){
        if(t[i].type == syntax && *t[i].value.t == r_brack_L){
            k++;
        }
        else if(t[i].type == syntax&&*t[i].value.t == r_brack_R&&k>0){
            k--;
        }
        else if(t[i].type == syntax && *t[i].value.t == r_brack_R && k == 0){
            return i;
        }
    }
    return -1;
}

int cond_parse(int start,int end,int len,int p){
    if(start==-1&&end==-1){
        return p<len;
    }
    if(start!=-1){
        return p<end;
    }
    return 0;
    
}


int count_elseelif(Token*tok,int p){
    int n=0;
    int len=token_len(tok);
    while(p<len&&tok[p].type==keyword&&*tok[p].value.t==elif_t){
        if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
            int opening_par=p+1;
            int closing_par=search_rpar(tok,opening_par);
            if(closing_par==-1){
                printf("ERROR missing closing ')' on line %d after elif\n",tok[opening_par].line);
            }
            p=closing_par+1;
            if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                int opening_rbrack=p;
                int closing_rback=search_rrbrack(tok,p);
                if(closing_rback==-1){
                    printf("ERROR missing closing '}' on line %d after elif\n",tok[p+1].line);
                    exit(1);
                }
                p=closing_rback+1;
                n++;
            }
            else{
                printf("ERROR missing opening '{' on line %d after else\n",tok[p-1].line);
                exit(1);
            }
        }
        else{
            printf("ERROR missing opening '(' on line %d after elif\n",tok[p].line);
            exit(1);
        }
    }
    if(p < len&&tok[p].type == keyword && *tok[p].value.t == else_t){
        if(p+1 < len && tok[p+1].type == syntax && *tok[p+1].value.t == r_brack_L){
            int opening_rbrack=p+1;
            int closing_rback=search_rrbrack(tok,opening_rbrack);
            if(closing_rback==-1){
                for(int i= opening_rbrack;i<len;i++){
                    token_print(tok[i],"\n");
                }
                printf("ERROR missing closing '}' on line %d after else\n",tok[p+1].line);
                exit(1);
            }
            p=closing_rback+1;
            n++;
        }
        else{
            printf("ERROR missing opening '{' on line %d after else\n",tok[p].line);
            exit(1);
        }
    }


    return n;
}

Instruction *parse_next_inst(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len, int *result){
    while (cond_parse(start,end,len,*p)) {
        int old_n_inst= *n_inst;
        //make if
        line = tok[*p].line;
        inst = make_if(tok, start, end, inst, n_inst, p, len);
        if(*n_inst != old_n_inst)
            return inst;
        //make for
        line = tok[*p].line;
        inst = make_for(tok, start, end, inst, n_inst, p, len);
        if(*n_inst != old_n_inst)
            return inst;
        //make return
        line = tok[*p].line;
        inst = make_return(tok, start, end, inst, n_inst, p, len);
        if(*n_inst != old_n_inst)
            return inst;
        //make while
        line = tok[*p].line;
        inst = make_while(tok, start, end, inst, n_inst, p, len);
        if(*n_inst != old_n_inst)
            return inst;
        //make funcdef
        line = tok[*p].line;
        inst = make_funcdef(tok, start, end, inst, n_inst, p, len);
        if(*n_inst != old_n_inst)
            return inst;

        if(tok[*p].type == identifier && *p + 1 < len) {
            if(tok[*p + 1].type == syntax && *tok[*p + 1].value.t == colon && *p + 2 < len){
                if(tok[*p + 2].type == syntax && *tok[*p + 2].value.t == colon){
                    (*n_inst)++;
                    inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                    inst[*n_inst - 1].type = inst_section_t;
                    inst[*n_inst - 1].value.section = malloc(sizeof(char) * (1 + strlen(tok[*p].value.s)));
					inst[*n_inst - 1].facultative = 0;
                    strcpy(inst[*n_inst - 1].value.section, tok[*p].value.s);
                    *p += 3;
                    return inst;
                }
            }
        }
        //make goto
        if(tok[*p].type == keyword && *tok[*p].value.t == goto_t){
            if(!(*p + 1 < len && tok[*p + 1].type == identifier)){
                printf("missing identifier after goto on line %d\n",tok[*p].line);
                exit(1);
            }
            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_goto_t;
			inst[*n_inst - 1].facultative = 0;
            inst[*n_inst - 1].value.goto_sec = malloc(sizeof(char) * (1 + strlen(tok[*p + 1].value.s)));
            strcpy(inst[*n_inst - 1].value.goto_sec,tok[*p + 1].value.s);
            *p += 2;
            return inst;
        }
        if(tok[*p].type == keyword && *tok[*p].value.t == proceed_t){
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction) * (*n_inst));
            inst[*n_inst-1].type = inst_proceed_t;
            inst[*n_inst - 1].facultative = 0;
            inst[*n_inst - 1].value.jmp_length = 1;
            if (*p + 1 < len && tok[*p + 1].type == ount) {
                inst[*n_inst - 1].value.jmp_length = *tok[*p + 1].value.i;
                *p += 1;
            }
            *p += 1;
            return inst;
        }
        if(tok[*p].type == keyword && *tok[*p].value.t == stop_t){
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
            inst[*n_inst-1].type = inst_stop_t;
            inst[*n_inst - 1].facultative = 0;
            inst[*n_inst - 1].value.jmp_length = 1;
            if (*p + 1 < len && tok[*p + 1].type == ount) {
                inst[*n_inst - 1].value.jmp_length = *tok[*p + 1].value.i;
                *p += 1;
            }
            *p += 1;
            return inst;
        }
        //else if (tok[*p].type == keyword && *tok[*p].value.t == import_t) {
        //    // check import(...)
        //    if (*p + 1 >= len || tok[*p + 1].type != syntax || par_L != *tok[*p + 1].value.t) {
        //        printf("ERROR import expected an opening '('\n");
        //        exit(1);
        //    }
        //    int par_l = *p + 1;
        //    int par_r = search_rpar(tok, par_l);
        //    if (par_r == -1) {
        //        printf("ERROR import expected a closing ')'\n");
        //        exit(1);
        //    }
        //    // check import(lib)
        //    if (par_l + 2 == par_r && tok[par_l + 1].type == identifier) {

        //    }
        //}
        else{
            if(tok[*p].type == syntax && *tok[*p].value.t == semicolon){
                (*p)++;
                continue;
            }
            if(tok[*p].type == end_token.type){
                (*n_inst)++;
                inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                inst[*n_inst - 1].line = line;
                inst[*n_inst - 1].type = inst_pass_t;
                inst[*n_inst - 1].facultative = 1;
                return inst;
            }
            int n=find_semicol(tok,*p);
            if(n==-1){
                token_print(tok[*p - 1],"n");
                printf("ERROR unexpected token on line %d %d %d\n",tok[*p].line, tok[*p].type, *p);
                exit(1);
            }
            //if(*p + 1 == n){
            //    (*p)++;
            //    continue;
            //}
            if(n == *p){
                (*p)++;
                continue;
            }
            ast_and_len val=tok_to_Ast(tok,*p,n);
            Ast*x=make_ast(val.value, val.len);
            int why = 0;
            x = check_do_what_here_why(x, &why);
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
            inst[*n_inst-1].type=inst_expr_t;
            inst[*n_inst-1].value.expr=x;
            inst[*n_inst - 1].facultative = 0;
            inst[*n_inst - 1].facultative = ((*p + 1 == n) && !why) ? 1 : 0;
            *p = n + 1;
            return inst;

        }
    }
    return inst;
}

//to parse everything pass your tokens ,-1,-1,NULL,a pointer that will ccount tthe length of instructions
Instruction *parse(Token *tok, int start, int end, Instruction *inst, int *n_inst) {
    int len = token_len(tok);
    int result = 0;
    int p = 0;

    if (start != -1) {
        p = start;
    }

    if (inst == NULL) {
        inst=malloc(sizeof(Instruction));
    }

    while (cond_parse(start,end,len,p)) {
        //les else et les elif sont gerer par la partie make if du parser
        if(tok[p].type==keyword&&(*tok[p].value.t==elif_t||*tok[p].value.t==else_t)){
            printf("ERROR expected if instruction above on line %d\n",tok[p].line);
            exit(1);
        }
        inst = parse_next_inst(tok, start, end, inst, n_inst, &p, len, &result);
    }
    inst = realloc(inst, sizeof(Instruction) * (*n_inst + 1));
    inst[*n_inst].type = inst_pass_t;
    inst[*n_inst].facultative = 1;
    inst[*n_inst].line = -1;
    (*n_inst)++;
    return make_requested_vars(inst, n_inst);
}
