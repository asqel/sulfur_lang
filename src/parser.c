#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/token_class.h"
#include "../include/Ast.h"
#include "../include/utilities.h"

//take a math expression and see if it ok
//return nothing 
//print error and exit(1)
void check_syntax(Ast*x){
    


}

Ast*tok_to_Ast(Token*tok,int start,int end){
    int len=end-start+1;
    Ast*e=malloc(sizeof(Ast)*(len));
    for(int i=start;i<end;i++){
        e[i-start].left=NULL;
        e[i-start].right=NULL;
        e[i-start].type=Ast_object_t;
        e[i-start].isAst=0;
        switch(tok[i].type){//techniquement faudrait mettre aussi les token end
            case nil:
                e[i-start].type=Ast_object_t;
                e[i-start].root.obj->type=Obj_nil_t;
                break;
            case ount:
                e[i-start].root.obj=malloc(sizeof(Object));
                e[i-start].type=Ast_object_t;
                e[i-start].root.obj->type=Obj_ount_t;
                e[i-start].root.obj->val.i=malloc(sizeof(long long int));
                *e[i-start].root.obj->val.i=*tok[i].value.i;
                break;
            case str:
                e[i-start].root.obj=malloc(sizeof(Object));
                e[i-start].type=Ast_object_t;
                e[i-start].root.obj->type=Obj_string_t;
                e[i-start].root.obj->val.s=malloc(sizeof(char)*(1+strlen(tok[i].value.s)));
                strcpy(e[i-start].root.obj->val.s,tok[i].value.s);
                break;
            case floap:
                e[i-start].root.obj=malloc(sizeof(Object));
                e[i-start].type=Ast_object_t;
                e[i-start].root.obj->type=Obj_floap_t;
                e[i-start].root.obj->val.f=malloc(sizeof(long double));
                *e[i-start].root.obj->val.f=*tok[i].value.f;
                break;
            case boolean_t:
                e[i-start].root.obj=malloc(sizeof(Object));
                e[i-start].type=Ast_object_t;
                e[i-start].root.obj->type=Obj_boolean_t;
                e[i-start].root.obj->val.b=malloc(sizeof(short int));
                *e[i-start].root.obj->val.b=*tok[i].value.b;
                break;
            case comp:
                e[i-start].root.obj=malloc(sizeof(Object));
                e[i-start].type=Ast_object_t;
                e[i-start].root.obj->type=Obj_complex_t;
                e[i-start].root.obj->val.c=malloc(sizeof(long double)*2);
                e[i-start].root.obj->val.c[0]=tok[i].value.c[0];
                e[i-start].root.obj->val.c[1]=tok[i].value.c[1];
                break;
            case op:
                e[i-start].type=Ast_op_t;
                e[i-start].root.op=*tok[i].value.t;
                break;
            case keyword:
                e[i-start].type=Ast_keyword_t;
                e[i-start].root.kw=*tok[i].value.t;
                break;
            case syntax:
                e[i-start].type=Ast_syntax_t;
                e[i-start].root.sy=*tok[i].value.t;
                break;
            case identifier:
                e[i-start].type=Ast_varcall_t;
                e[i-start].root.varcall=malloc(sizeof(char)*(1+strlen(tok[i].value.s)));
                strcpy(e[i-start].root.varcall,tok[i].value.s);
                break;
            default:
                break;
        }
    }
    return e;
}


int find_highest_op(Ast*e,int len){
    //search for .(dot)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_syntax_t&&e[i].root.sy==dot&&!e[i].isAst){//en theorie ya pasa besoin de verifier isAst
            return i;
        }
    }
    //search for ^(pow)
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
    //search for &(and)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_AND){
                return i;
            }
        }
    }
    //search for &(and)
    for(int i=0;i<len;i++){
        if(e[i].type==Ast_op_t&&!e[i].isAst){
            if(e[i].root.op==OP_OR){
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
                printf("ERROR missing closing ')' in expression on function call");
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
                    printf("ERROR missing closing ')' in function call");
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
                        if(to_parse[i].type==Ast_funccall_t && i+1<to_parse_len){
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
                                    printf("ERROR missing closing ')' in expression on function call");
                                    exit(1);
                                }
                                int l=cl_par-op_par+2;
                                a[k].l+=l;
                                a[k].v=realloc(a[k].v,sizeof(Ast)*a[k].l);
                                for(int j=op_par;j<=cl_par;j++){
                                    a[k].v[j-op_par+a[k].l-l]=to_parse[j];
                                }
                                i=cl_par;
                                continue;
                            }
                        }
                        a[k].l++;
                        a[k].v=realloc(a[k].v,sizeof(Ast)*(a[k].l));
                        a[k].v[a[k].l-1]=to_parse[i];
                    }

                    funccall f;
                    f.args=malloc(sizeof(Ast)*(k+1));
                    for(int i=0;i<=k;i++){
                        f.args[i]=*make_ast(a[i].v,a[i].l);
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
            int opening_par=p;
            int closing_par=-1;

            int i=opening_par+1;
            int count=0;
            while(i<len){
                if(e[i].type==Ast_syntax_t&&e[p].root.sy==par_L){
                    count++;
                }
                else if(e[i].type==Ast_syntax_t&&e[p].root.sy==par_R&& count>0){
                    count--;
                }
                else if(e[i].type==Ast_syntax_t&&e[p].root.sy==par_R&& count==0){
                    closing_par=i;
                    break;
                }
                i++;
            }

            if(closing_par==-1){
                printf("ERROR missing closing ')' in expression");
                exit(1);
            }
            Ast*expr=malloc(sizeof(Ast)*(closing_par-opening_par-1));
            for(int i=opening_par+1;i<closing_par;i++){
                expr[i-(opening_par+1)]=e[i];
            }
            expr=make_ast(expr,closing_par-opening_par-1);
            e[opening_par]=*expr;
            e[opening_par].isAst=1;

            for(int i=closing_par;i<len;i++){
                e[opening_par+1+(i-closing_par)]=e[i];
            }
            len=len-closing_par-opening_par+1-1;
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
     
    //make unary + - !
    p=0;
    while(p<len){
        if(e[p].type==Ast_op_t && e[p].root.op==OP_NOT){// !
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '!' in expression");
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
        else if(e[p].type==Ast_op_t && e[p].root.op==OP_PLUS&&p-1>=0&&e[p-1].type==Ast_op_t){
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '+'(unary) in expression");
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
        else if(e[p].type==Ast_op_t && e[p].root.op==OP_MINUS&&p-1>=0&&e[p-1].type==Ast_op_t){
            if(!(p+1<len&&(e[p+1].isAst||e[p+1].type==Ast_object_t||e[p+1].type==Ast_varcall_t))){
                printf("ERROR missing operand after '-'(unary) in expression");
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
                printf("ERROR missing operand after '-'(unary) in expression");
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
            printf("ERROR in expression #0");
            exit(1);
        }
        if(n-1<0){
            printf("ERROR missing left operand in expression #1");
            exit(1);
        }
        if(n+1>=len){
            printf("ERROR missing right operand in expression #2");
            exit(1);
        }
        if(!(e[n-1].isAst||e[n-1].type==Ast_object_t||e[n-1].type==Ast_varcall_t)){
            printf("ERROR missing left operand in expression #3 %d\n",e[n-1].type);
            exit(1);
        }
        if(!(e[n+1].isAst||e[n+1].type==Ast_object_t||e[n+1].type==Ast_varcall_t)){
            printf("ERROR missing right operand in expression #4");
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
        if(t[i].type==syntax&&*t[i].value.t==r_brack_L){
            k++;
        }
        else if(t[i].type==syntax&&*t[i].value.t==r_brack_R&&k>0){
            k--;
        }
        else if(t[i].type==syntax&&*t[i].value.t==r_brack_R&&k==0){
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
    
}


int count_elseelif(Token*tok,int p){
    int n=0;
    int len=token_len(tok);
    while(p<len&&tok[p].type==keyword&&*tok[p].value.t==elif_t){
        if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
            int opening_par=p+1;
            int closing_par=search_rpar(tok,opening_par);
            if(closing_par==-1){
                printf("ERROR missing closing ')' on line %d after elif",tok[opening_par].line);
            }
            p=closing_par+1;
            if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                int opening_rbrack=p;
                int closing_rback=search_rrbrack(tok,p);
                if(closing_rback==-1){
                    printf("ERROR missing closing '}' on line %d after elif",tok[p+1].line);
                    exit(-1);
                }
                p=closing_rback+1;
                n++;
            }
            else{
                printf("ERROR missing opening '{' on line %d after else",tok[p-1].line);
                exit(-1);
            }
        }
        else{
            printf("ERROR missing opening '(' on line %d after elif",tok[p].line);
            exit(-1);
        }
    }
    if(p<len&&tok[p].type==keyword&&*tok[p].value.t==else_t){
        if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==r_brack_L){
            int opening_rbrack=p+1;
            int closing_rback=search_rrbrack(tok,opening_rbrack);
            if(closing_rback==-1){
                printf("ERROR missing closing '}' on line %d after else",tok[p+1].line);
                exit(-1);
            }
            p=closing_rback+1;
            n++;
        }
        else{
            printf("ERROR missing opening '{' on line %d after else",tok[p].line);
            exit(-1);
        }
    }


    return n;
}

//to parse everything pass your tokens ,-1,-1,NULL,a pointer that will ccount tthe length of instructions
Instruction*parse(Token*tok,int start,int end,Instruction*inst,int*n_inst){

    int len=token_len(tok);
    int p=0;
    if(start!=-1){
        p=start;
    }
    if(inst==NULL){
        Instruction*inst=malloc(sizeof(Instruction));
    }
    while(cond_parse(start,end,len,p)){
        //les else et les elif sont gerer par la partie if make du parser
        if(tok[p].type==keyword&&(*tok[p].value.t==elif_t||*tok[p].value.t==else_t)){
            printf("ERROR expected if instruction above on line %d",tok[p].line);
            exit(1);
        }
        //if maker
        if(tok[p].type==keyword&&*tok[p].value.t==if_t){
            if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
                int opening_par=p+1;
                int closing_par=search_rpar(tok,p+1);
                if (closing_par==-1){
                    printf("ERROR missing closing ')' on line %d after if",tok[p+1].line);
                    exit(-1);
                }
                p=closing_par+1;
                if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                    int opening_rbrack=p;
                    int closing_rback=search_rrbrack(tok,p);
                    if(closing_rback==-1){
                        printf("ERROR missing closing '}' on line %d after if",tok[p+1].line);
                        exit(-1);
                    }
                    (*n_inst)++;
                    inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                    inst[*n_inst-1].type=inst_if_t;
                    inst[*n_inst-1].value.i=malloc(sizeof(If));
                    
                    inst[*n_inst-1].value.i->condition=make_ast(tok_to_Ast(tok,opening_par+1,closing_par),closing_par-(opening_par+1));
                    int if_index=*n_inst-1;
                    
                    inst=parse(tok,opening_rbrack+1,closing_rback,inst,n_inst);

                    (*n_inst)++;
                    inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                    inst[*n_inst-1].type=inst_endif;
                    inst[if_index].value.i->endif_p=*n_inst-1;

                    int*endifelse_ps=malloc(sizeof(int)*(1+count_elseelif(tok,closing_rback+1)));//list des endif ou on doit leur rajouter le pointer vers un endifelse
                    int endif_n=1;
                    endifelse_ps[0]=*n_inst-1;
                    p=closing_rback+1;

                    while(p<len&&tok[p].type==keyword&&*tok[p].value.t==elif_t){
                        if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
                            opening_par=p+1;
                            closing_par=search_rpar(tok,opening_par);
                            if(closing_par==-1){
                                printf("ERROR missing closing ')' on line %d after elif",tok[opening_par].line);
                            }
                            p=closing_par+1;
                            if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                                int opening_rbrack=p;
                                int closing_rback=search_rrbrack(tok,p);
                                if(closing_rback==-1){
                                    printf("ERROR missing closing '}' on line %d after elif",tok[p+1].line);
                                    exit(-1);
                                }
                                (*n_inst)++;
                                inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                                inst[*n_inst-1].type=inst_elif_t;
                                inst[*n_inst-1].value.el=malloc(sizeof(Elif));
                                inst[*n_inst-1].value.el->condition=make_ast(tok_to_Ast(tok,opening_par+1,closing_par),closing_par-(opening_par+1));
                                int elif_index=*n_inst-1;
                                
                                inst=parse(tok,opening_rbrack+1,closing_rback,inst,n_inst);
            
                                (*n_inst)++;
                                inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                                inst[*n_inst-1].type=inst_endif;
                                inst[elif_index].value.el->endif_p=*n_inst-1;
                                p=closing_rback+1;

                                endif_n++;
                                endifelse_ps[endif_n-1]=*n_inst-1;//ffaut faire ca en bas aussi avec le else oublie pas hein !
                            }
                            else{
                                printf("ERROR missing opening '{' on line %d after elif",tok[p-1].line);
                                exit(-1);
                            }
                        }
                        else{
                            printf("ERROR missing opening '(' on line %d after elif",tok[p].line);
                            exit(-1);
                        }
                    }
                    if(p<len&&tok[p].type==keyword&&*tok[p].value.t==else_t){
                        if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==r_brack_L){
                            opening_rbrack=p+1;
                            closing_rback=search_rrbrack(tok,opening_rbrack);
                            if(closing_rback==-1){
                                printf("ERROR missing closing '}' on line %d after else",tok[p+1].line);
                                exit(-1);
                            }

                            (*n_inst)++;
                            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                            inst[*n_inst-1].type=inst_else_t;

                            int else_index=*n_inst-1;
                            inst=parse(tok,opening_rbrack+1,closing_rback,inst,n_inst);
                            (*n_inst)++;
                            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                            inst[*n_inst-1].type=inst_endif;
                            inst[else_index].value.endif=*n_inst-1;
                            
                            p=closing_rback+1;


                            endif_n++;
                            endifelse_ps[endif_n-1]=*n_inst-1;
                        }
                        else{
                            printf("ERROR missing opening '{' on line %d after else",tok[p].line);
                            exit(-1);
                        }
                    }
                    (*n_inst)++;
                    inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                    inst[*n_inst-1].type=inst_endifelse;

                    for(int i=0;i<endif_n;i++){
                        inst[endifelse_ps[i]].value.endifelse=*n_inst-1;
                    }
                    free(endifelse_ps);
                    continue;
                }
                

            }
            else{
                printf("ERROR on if missing opening '(' line %d",tok[p].line);
                exit(-1);
            }
        }
        //var set
        if(tok[p].type==identifier){
            if(p+2<len){
                if(tok[p+1].type==identifier) {
                    int n=find_semicol(tok,p);
                    if(tok[p+2].type==op&&*tok[p+2].value.t==OP_ASSIGN&&n!=-1){
                        if(n==p+3){
                            printf("ERROR missing expression after '=' on line %d",tok[p+2].line);
                            exit(-1);
                        }
                        (*n_inst)++;
                        inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                        inst[*n_inst-1].type=inst_varset_t;

                        inst[*n_inst-1].value.vs=malloc(sizeof(varset));
                        inst[*n_inst-1].value.vs->name=malloc(sizeof(char)*(strlen(tok[p+1].value.s))+1);
                        strcpy(inst[*n_inst-1].value.vs->name,tok[p+1].value.s);//set name
           
                        inst[*n_inst-1].value.vs->type=malloc(sizeof(char)*(strlen(tok[p].value.s))+1);
                        strcpy(inst[*n_inst-1].value.vs->type,tok[p].value.s);//set type
                       
                        Ast *a=tok_to_Ast(tok,p+3,n);
                        Ast*v=make_ast(a,n-(p+3+1-1));
                        inst[*n_inst-1].value.vs->val=v;//set val
                        p=n+1;
                        continue;

                    }
                    else{
                        if (n==-1){
                            printf("ERROR : missing semicollon ';' on line %d after Token ",tok[p+2].line);
                            token_print(tok[p+2],"\n");
                            exit(1);
                        }
                        if(tok[p+2].type!=op||*tok[p+2].value.t!=OP_ASSIGN){
                            printf("ERROR : missing assignement '=' on line %d after token",tok[p+2].line);
                            token_print(tok[p+2],"\n");
                            exit(1);
                        }
                    }
                }
            }

        }
        //var new set
        if(tok[p].type==identifier){
            if(p+1<len&&tok[p+1].type==op&&*tok[p+1].value.t==OP_ASSIGN){
                int n=find_semicol(tok,p+1);
                if(n==-1){
                    printf("ERROR missing ';' on line %d",tok[p+1].line);
                    exit(-1);
                }
                if(n==p+2){
                    printf("ERROR missing expression on line %d after '='",tok[p+1].line);
                    exit(-1);
                }
                (*n_inst)++;
                inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                inst[*n_inst-1].type=inst_new_varset_t;

                inst[*n_inst-1].value.vs=malloc(sizeof(varset));
                inst[*n_inst-1].value.vs->name=malloc(sizeof(char)*(strlen(tok[p].value.s))+1);
                strcpy(inst[*n_inst-1].value.vs->name,tok[p].value.s);//set name

                Ast*v=make_ast(tok_to_Ast(tok,p+2,n),n-(p+2));
                inst[*n_inst-1].value.vs->val=v;//set val
                p=n+1;
                continue;
            }
        }
        //for 
        if(tok[p].type==keyword&&*tok[p].value.t==for_t){
            if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
                int n=search_rpar(tok,p+1);
                if(n==-1){
                    printf("ERROR missing closing ')' on line %d after for",tok[p+1].line);
                    exit(-1);
                }
                if(n==p+2){
                    printf("ERROR empty for-statement on line %d after for\nCorrect for-statement: for(i from a to b){",tok[p+1].line);
                    exit(-1);
                }
                if(p+2<len&&tok[p+2].type==identifier){
                    int id_idx=p+2;
                    if(p+3<len&&tok[p+3].type==keyword&&*tok[p+3].value.t==from_t){
                        int n2=-1;
                        for(int i=p+4;i<len;i++){
                            if(tok[i].type==keyword&&*tok[i].value.t==to_t){
                                n2=i;
                                break;
                            }
                        }
                        if(n2==-1){
                            printf("ERROR missing 'to' in for-statement on line %d after for\nCorrect for-statement: for(i from a to b){",tok[p+1].line);
                            exit(-1);
                        }
                        if(n+1<len&&tok[n+1].type==syntax&&*tok[n+1].value.t==r_brack_L){
                            int k=search_rrbrack(tok,n+1);
                            if(k==-1){
                                printf("ERROR missing closing '}' after for-statement on line %d after for\nCorrect for-statement: for(i from a to b){",tok[n+1].line);
                                exit(-1);
                            }

                            Ast*x=make_ast(tok_to_Ast(tok,p+4,n2),n2-(p+4));
                            Ast*x2=make_ast(tok_to_Ast(tok,n2+1,n),n-(n2+1));

                            (*n_inst)++;
                            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                            inst[*n_inst-1].type=inst_for_t;
                            inst[*n_inst-1].value.fo=malloc(sizeof(For));
                            
                            inst[*n_inst-1].value.fo->var_name=malloc(sizeof(char)*(1+strlen(tok[id_idx].value.s)));
                            strcpy(inst[*n_inst-1].value.fo->var_name,tok[id_idx].value.s);
                            
                            inst[*n_inst-1].value.fo->start=x;
                            inst[*n_inst-1].value.fo->end=x2;

                            int for_idx=*n_inst-1;

                            inst=parse(tok,n+2,k,inst,n_inst);

                            (*n_inst)++;
                            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                            inst[*n_inst-1].type=inst_endfor_t;
                            inst[*n_inst-1].value.endfor=for_idx;
                            inst[for_idx].value.fo->endfor=*n_inst-1;
                            p=k+1;
                            continue;
                        }
                        else{
                            printf("ERROR missing '{' after for-statement on line %d after for",tok[n].line);
                            exit(-1);
                        }
                    }

                }
                else{
                    printf("ERROR in for-statement on line %d after for\nCorrect for-statement: for(i from a to b){",tok[p+1].line);
                    exit(-1);
                }
            }
        }
        //return
        if(tok[p].type==keyword && *tok[p].value.t==return_t){
            int n=find_semicol(tok,p);
            if(n==-1){
                printf("ERROR missing ';' on line %d after return",tok[p].line);
                exit(1);
            }
            if(n==p+1){
                //if its "return;" then it is considered to be "return nil;"
                (*n_inst)++;
                inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                inst[*n_inst-1].type=inst_return_t;
                inst[*n_inst-1].value.ret=malloc(sizeof(Ast));
                inst[*n_inst-1].value.ret->left=NULL;
                inst[*n_inst-1].value.ret->right=NULL;

                inst[*n_inst-1].value.ret->type=Ast_object_t;

                inst[*n_inst-1].value.ret->root.obj=malloc(sizeof(Object));
                inst[*n_inst-1].value.ret->root.obj->type=Obj_nil_t;
                p=n+1;
                continue;
            }
            else{
                Ast*x=make_ast(tok_to_Ast(tok,p+1,n),n-(p+1));
                (*n_inst)++;
                inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                inst[*n_inst-1].type=inst_return_t;
                inst[*n_inst-1].value.ret=x;
                p=n+1;
                continue;
            }

        }
        //swap
        if(tok[p].type==keyword && *tok[p].value.t==swap_t){
            int n=find_semicol(tok,p);
            if(n==-1){
                printf("ERROR missing ';' after swap on line %d",tok[p].line);
            }
            if(n!=p+3){
                printf("ERROR on swap only 2 identifier expected after swap on line %d",tok[p].line);
                exit(1);
            }
            if(tok[p+1].type!=identifier||tok[p+2].type!=identifier){
                printf("ERROR on swap only 2 identifier expected after swap on line %d",tok[p].line);
                exit(1);
            }
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
            inst[*n_inst-1].type=inst_swap_t;
            inst[*n_inst-1].value.sw=malloc(sizeof(Swap));
            inst[*n_inst-1].value.sw->len=2;
            inst[*n_inst-1].value.sw->ids=malloc(sizeof(char*));
            inst[*n_inst-1].value.sw->ids[0]=malloc(sizeof(char)*(1+strlen(tok[p+1].value.s)));
            inst[*n_inst-1].value.sw->ids[1]=malloc(sizeof(char)*(1+strlen(tok[p+2].value.s)));
            strcpy(inst[*n_inst-1].value.sw->ids[0],tok[p+1].value.s);
            strcpy(inst[*n_inst-1].value.sw->ids[1],tok[p+2].value.s);
            p=n+1;
            continue;
        }
        if(tok[p].type==keyword&&*tok[p].value.t==while_t){
            if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
                int opening_par=p+1;
                int closing_par=search_rpar(tok,p+1);
                if (closing_par==-1){
                    printf("ERROR missing closing ')' on line %d after while",tok[p+1].line);
                    exit(-1);
                }
                p=closing_par+1;
                if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                    int opening_rbrack=p;
                    int closing_rback=search_rrbrack(tok,p);
                    if(closing_rback==-1){
                        printf("ERROR missing closing '}' on line %d after while",tok[p+1].line);
                        exit(-1);

                    }
                    (*n_inst)++;
                    inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                    inst[*n_inst-1].type=inst_while_t;
                    inst[*n_inst-1].value.wh=malloc(sizeof(While));
                    inst[*n_inst-1].value.i->condition=make_ast(tok_to_Ast(tok,opening_par+1,closing_par),closing_par-(opening_par+1));
                    int while_index=*n_inst-1;
                    
                    inst=parse(tok,opening_rbrack+1,closing_rback,inst,n_inst);

                    (*n_inst)++;
                    inst=realloc(inst,sizeof(Instruction)*(*n_inst));
                    inst[*n_inst-1].type=inst_endwhile_t;
                    inst[*n_inst-1].value.endwhile=while_index;
                    inst[while_index].value.wh->endwhile=*n_inst-1;
                    p=closing_rback+1;
                    continue;
                }
            }
        }
        if(tok[p].type==identifier&&p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==colon){
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
            inst[*n_inst-1].type=inst_section_t;
            inst[*n_inst-1].value.section=malloc(sizeof(char)*(1+strlen(tok[p].value.s)));
            strcpy(inst[*n_inst-1].value.section,tok[p].value.s);
            p+=2;
            continue;
        }
        if(tok[p].type==keyword&&*tok[p].value.t==goto_t){
            if(!(p+1<len&&tok[p+1].type==identifier)){
                printf("missing identifier after goto on line %d",tok[p].line);
                exit(1);
            }
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
            inst[*n_inst-1].type=inst_goto_t;
            inst[*n_inst-1].value.goto_sec=malloc(sizeof(char)*(1+strlen(tok[p+1].value.s)));
            strcpy(inst[*n_inst-1].value.section,tok[p+1].value.s);
            p+=2;
            continue;
        }
        else{
            int n=find_semicol(tok,p);
            if(n==-1){
                printf("ERROR unexpected token on line %d",tok[p].line);
                exit(-1);
            }
            Ast*x=make_ast(tok_to_Ast(tok,p,n),n-(p));
            (*n_inst)++;
            inst=realloc(inst,sizeof(Instruction)*(*n_inst));
            inst[*n_inst-1].type=inst_expr_t;
            inst[*n_inst-1].value.expr=x;
            p=n+1;
            continue;
            
        }
    }
    return inst;


}