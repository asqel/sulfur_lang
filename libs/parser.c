#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token_class.h"
#include "Ast.h"


//take a math expression and see if it ok
//return nothing 
//print error and exit(1)
void check_syntax(Ast*x){
    


}

//return new pointer and free old 
Ast*make_list_funccal_varcal_unary(Ast*x){
    Ast*a=malloc(sizeof(Ast));
    int n=0;
    int len=Ast_len(x);

    for(int i=0;i<len;i++){
        n++;
        a=realloc(a,sizeof(Ast)*n);
        a[n-1].left=NULL;
        a[n-1].right=NULL;
        if(x[i].type==Ast_varcall_t&&i+1<len&&x[i+1].type==Ast_token_t&&x[i+1].root.tok->type==syntax&&*x[i+1].root.tok->value.t==par_L){
            if(i+2<len&&x[i+2].type==Ast_token_t&&x[i+2].root.tok->type==syntax&&*x[i+2].root.tok->value.t==par_R){
                a[n-1].type=Ast_funccall_t;
                a[n-1].root.fun=malloc(sizeof(funccall));
                a[n-1].root.fun->nbr_arg=0;
                a[n-1].root.fun->name=malloc(sizeof(char)*(1+strlen(x[i].root.varcall)));
                strcpy(a[n-1].root.fun->name,x[i].root.varcall); 
            }
        }


    }

}

//start included
//end not included
//there no check if end and start are out of range
Ast*make_ast(Token*tok,int start,int end){
    Ast*e=malloc(sizeof(Ast)*(end-start+1));
    for(int i=start;i<end;i++){
        e[i-start].left=NULL;
        e[i-start].right=NULL;
        e[i-start].type=Ast_object_t;
        switch(tok[i].type){//techniquement faudrait mettre aussi les token end
        case nil:
            e[i-start].type=Ast_object_t;
            e[i-start].root.obj->type=Obj_nil_t;
            e[i-start].root.obj->val.b=malloc(sizeof(short int));
            *e[i-start].root.obj->val.b=-1;
            break;
        case ount:
            e[i-start].type=Ast_object_t;
            e[i-start].root.obj->type=Obj_ount_t;
            e[i-start].root.obj->val.i=malloc(sizeof(long long int));
            *e[i-start].root.obj->val.i=*tok[i].value.i;
            break;
        case str:
            e[i-start].type=Ast_object_t;
            e[i-start].root.obj->type=Obj_string_t;
            e[i-start].root.obj->val.s=malloc(sizeof(char)*(1+strlen(tok[i].value.s)));
            strcpy(e[i-start].root.obj->val.s,tok[i].value.s);
            break;
        case floap:
            e[i-start].type=Ast_object_t;
            e[i-start].root.obj->type=Obj_floap_t;
            e[i-start].root.obj->val.f=malloc(sizeof(long double));
            *e[i-start].root.obj->val.f=*tok[i].value.f;
            break;
        case boolean:
            e[i-start].type=Ast_object_t;
            e[i-start].root.obj->type=Obj_boolean_t;
            e[i-start].root.obj->val.b=malloc(sizeof(short int));
            *e[i-start].root.obj->val.b=tok[i].value.b;
            break;
        case comp:
            e[i-start].type=Ast_object_t;
            e[i-start].root.obj->type=Obj_complex_t;
            e[i-start].root.obj->val.c=malloc(sizeof(long double)*2);
            e[i-start].root.obj->val.c[0]=tok[i].value.c[0];
            e[i-start].root.obj->val.c[1]=tok[i].value.c[1];
            break;
        case op:
            e[i-start].type=Ast_token_t;
            e[i-start].root.tok=malloc(sizeof(Token));
            e[i-start].root.tok->line=tok[i].line;
            e[i-start].root.tok->type=tok[i].type;
            e[i-start].root.tok->value.t=malloc(sizeof(short int));
            *e[i-start].root.tok->value.t=tok[i].value.t;

        case keyword:
            e[i-start].type=Ast_token_t;
            e[i-start].root.tok=malloc(sizeof(Token));
            e[i-start].root.tok->line=tok[i].line;
            e[i-start].root.tok->type=tok[i].type;
            e[i-start].root.tok->value.t=malloc(sizeof(short int));
            *e[i-start].root.tok->value.t=tok[i].value.t;
        case syntax:
            e[i-start].type=Ast_token_t;
            e[i-start].root.tok=malloc(sizeof(Token));
            e[i-start].root.tok->line=tok[i].line;
            e[i-start].root.tok->type=tok[i].type;
            e[i-start].root.tok->value.t=malloc(sizeof(short int));
            *e[i-start].root.tok->value.t=tok[i].value.t;
        case identifier:
        e[i-start].type=Ast_varcall_t;
        e[i-start].root.varcall=malloc(sizeof(char)*(1+strlen(tok[i].value.s)));
        strcpy(e[i-start].root.varcall,tok[i].value.s);
        default:
            break;
        }
    }//e contains only tokens from start to end 
    e[end-start].type=Ast_end_t;
    check_syntax(e);
    e=make_list_funccal_varcal_unary(e);


    



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

int cond_pars(int start,int end,int len,int p){
    if(start==-1&&end==-1){
        return p<len;
    }
    if(start==-1){
        return p<end;
    }
    
}


//to parse everything pass tokens ,-1,-1,NULL
Instruction*parse(Token*tok,int start,int end,Instruction*inst){

    int len=token_len(tok);
    int p=0;
    if(start!=-1){
        p=start;
    }
    if(inst==NULL){
        Instruction*inst=malloc(sizeof(Instruction));
    }
    int n_inst=0;
    while(cond_parse(start,end,len,p)){
        //les else et les elif sont gerer par la partie if make du parser
        if(tok[p].type==keyword&&(*tok[p].value.t==elif_t||*tok[p].value.t==else_t)){
            printf("ERROR expected if instruction above on line %d",tok[p].line);
        }
        //if maker
        if(tok[p].type==keyword&&*tok[p].value.t==if_t){
            if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
                int n=search_rpar(tok,p+1);
                if(n==-1){
                    printf("ERROR missing closing ')' on line %d after if",tok[p+1].line);
                    exit(-1);
                }
                if(n==p+2){
                    printf("ERROR empty condition on line %d after if",tok[p+1].line);
                    exit(-1);
                }
                Ast*x=make_ast(tok,p+2,n);
                p=n+1;
                if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                    int n2=search_rrbrack(tok,p);
                    if(n2==-1){
                        printf("ERROR missing '}' on line %d after if",tok[n].line);
                        exit(-1);
                    }

                    n_inst++;
                    inst=realloc(inst,sizeof(Instruction)*n_inst);
                    inst[n_inst-1].type=inst_if_t;
                    inst[n_inst-1].value.i=malloc(sizeof(If)); 
                    inst[n_inst-1].value.i->condition=x;
                    int if_p=n_inst-1;

                    inst=parse(tok,p+1,n2,inst);
                    
                    n_inst++;
                    inst=realloc(inst,sizeof(Instruction)*n_inst);
                    inst[n_inst-1].type=inst_endif;

                    inst[if_p].value.i->endif_p=n_inst-1;

                    p=n2+1;
                    if(p<len){
                        int*endif_p=malloc(sizeof(int));
                        int n_endif_p=1;
                        endif_p[0]=n_inst-1;
                        if(tok[p].type==keyword&&(*tok[p].value.t==elif_t)){
                            while(p<len&&tok[p].type==keyword&&(*tok[p].value.t==elif_t)){
                                if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==par_L){
                                    n=search_rpar(tok,p+1);
                                    if(n==-1){
                                        printf("ERROR missing closing ')' on line %d after elif",tok[p+1].line);
                                        exit(-1);
                                    }
                                    if(p+2==n){
                                        printf("ERROR empty condition on line %d after elif",tok[p+2].line);
                                        extit(-1);
                                    }
                                    x=make_ast(tok,p+2,n);
                                    p=n+1;
                                    if(p<len&&tok[p].type==syntax&&*tok[p].value.t==r_brack_L){
                                        n2=search_rrbrack(tok,p);
                                        if(n2==-1){
                                            printf("ERROR missing closing '}' on line %d after elif",tok[p].line);
                                            exit(-1);
                                        }
                                        n_inst++;
                                        inst=realloc(inst,sizeof(Instruction)*n_inst);
                                        inst[n_inst-1].type=inst_elif_t;
                                        inst[n_inst-1].value.el=malloc(sizeof(Elif));
                                        inst[n_inst-1].value.el->condition=x;
                                        int elif_p=n_inst-1;

                                        inst=parse(tok,p+1,n2,inst);

                                        inst[n_inst-1].type=inst_endif;
                                        n_endif_p++;
                                        endif_p=realloc(endif_p,sizeof(n_endif_p));
                                        endif_p[n_endif_p-1]=n_inst-1;

                                        inst[elif_p].value.el->endif_p=n_inst-1;
                                        p=n2;


                                    }
                                    else{
                                        printf("ERROR mmissing '{' on line %d after elif",tok[n].line);
                                    }
                                    
                                }
                                else{
                                    printf("ERROR missing '(' on line %d after elif",tok[p].line);
                                    exit(-1);
                                }
                            }
                        }
                        if(p<len&&tok[p].type==keyword&&(*tok[p].value.t==else_t)){
                            int elsecount=0;
                            while(p<len&&tok[p].type==keyword&&(*tok[p].value.t==inst_else_t)){
                                if(elsecount>1){
                                    print("ERROR only 1 expected but got 2 else on line %d",tok[p].line);
                                    exit(-1);
                                }
                                if(p+1<len&&tok[p+1].type==syntax&&*tok[p+1].value.t==r_brack_L){
                                    n=search_rrbrack(tok,p+1);
                                    if(n==-1){
                                        printf("ERROR missing closing '}' on line %d after else",tok[p+1].line);
                                        exit(-1);
                                    }
                                    n_inst++;
                                    inst=realloc(inst,sizeof(Instruction));
                                    inst[n_inst-1].type=inst_else_t;
                                    int else_p=n_inst-1;

                                    inst=parse(tok,p+2,n,inst);

                                    n_inst++;
                                    inst=realloc(inst,sizeof(Instruction)*n_inst);
                                    inst[n_inst-1].type=inst_endif;
                                    n_endif_p++;
                                    endif_p=realloc(endif_p,sizeof(int)*n_endif_p);
                                    endif_p[n_endif_p]=n_inst-1;

                                    inst[else_p].value.i->endif_p=n_inst-1;
                                    p=n2;

                                }
                                else{
                                    printf("ERROR missing '{' on line %d after else",tok[p].line);
                                }
                                
                                
                            }
                        }
                        n_inst++;
                        inst=realloc(inst,sizeof(Instruction)*n_inst);
                        inst[n_inst-1].type=inst_endifelse;
                        for(int i=0;i<n_endif_p;i++){
                            inst[endif_p[i]].value.endifelse=n_inst-1;
                        }
                        free(endif_p);
                        continue;
                    }
                    continue;
                    
                }
                else{
                    printf("ERROR missing '{' on line %d after if",tok[p].line);
                    exit(-1);
                }

            }
            else{
                printf("ERROR missing '(' on line %d after if",tok[p].line);
                exit(-1);
            }
        }
        
        if(tok[p].type==identifier){
            if(p+2<len){
                if(tok[p+1].type==identifier) {
                    int n=find_semicol(tok,p);
                    if(tok[p+2].type==op&&*tok[p+2].value.t==OP_ASSIGN&&n!=-1){
                        n_inst++;
                        inst=realloc(inst,sizeof(Instruction)*n_inst);
                        inst[n_inst-1].type=inst_varset_t;

                        inst[n_inst-1].value.vs=malloc(sizeof(varset));
                        inst[n_inst-1].value.vs->name=malloc(sizeof(char)*(strlen(tok[p].value.s))+1);
                        strcpy(inst[n_inst-1].value.vs->name,tok[p].value.s);//set name
           
                        inst[n_inst-1].value.vs->type=malloc(sizeof(char)*(strlen(tok[p+1].value.s))+1);
                        strcpy(inst[n_inst-1].value.vs->type,tok[p+1].value.s);//set type
                        Ast*v=make_ast(tok,p+3,n);
                        inst[n_inst-1].value.vs->val=v;

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
        else{
            p++;
        }
    }
    return inst;


}