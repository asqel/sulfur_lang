#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/token_class.h"

char KEYWORDS[10][7]={"if","while","else","elif","for","class","return","def","from","to"};
int keyword_len=10;

Token nil_token={-1,nil,{.i=&(long long int){0}}};
Token end_token={-1,end,{.i=&(long long int){1}}};


char DIGITS[10]="1234567890";

char OPS[20][3]={"+","*","-","/","^","%","\\","|","&","!","=","==","+=","-=","!=","<=",">=","<",">"};
int ops_len=20;

char SYNTAX[13][2]={"#","{","}","[","]","(",")","@",":",";",".","?",","};
int syntax_len=13;





//return the length of array of Token
int token_len(Token*toks){
    int x=0;
    int i=0;
    while(1){
        if(toks[i].type==end){
            return x;
        }
        x++;
        i++;
    }
}

char*get_kw(short int x){
    return KEYWORDS[x];
}
char*get_op(short int x){
    return OPS[x];

}
char*get_sy(short int x){
    return SYNTAX[x];
}

int kw_to_enum(char*v){
    for(int i=0;i<keyword_len;i++){
        if(!strcmp(v,KEYWORDS[i])){
            return i;
        }
    }
    return -1;

}
int op_to_enum(char*v){
    for(int i=0;i<ops_len;i++){
        if(!strcmp(v,OPS[i])){
            return i;
        }
    }
    return -1;

}
int sy_to_enum(char*v){
    for(int i=0;i<syntax_len;i++){
        if(!strcmp(v,SYNTAX[i])){
            return i;
        }
    }
    return -1;
}

void token_print(Token tok,char*e){
    switch (tok.type){
        case floap:
            printf("Tf:[");
            printf("%lld",(long long int)*tok.value.f);
            printf(".%lld",(long long int)(*tok.value.f*100000000)-100000000*(long long int)*tok.value.f);
            printf("]");
            printf(e);
            break;
        case ount:
            printf("Ti:[%lld]%s",*(tok.value.i),e);
            break;
        case str:
            printf("Ts:[%s]%s",tok.value.s,e);
            break;
        case boolean:
            printf("Tb:[%d]%s",*(tok.value.b),e);
            break;
        case keyword:
            printf("Kw:[%s]%s",get_kw(*(tok.value.t)),e);
            break;
        case syntax:
            printf("Sy:[%s]%s",get_sy(*(tok.value.t)),e);
            break;
        case op:
            printf("Op:[%s]%s",get_op(*(tok.value.t)),e);
            break;
        case identifier:
            printf("Id:[%s]%s",tok.value.s,e);
        default:
            break;
    }
}

//print each Token of a list with at end of print the string to print
//toks:array of Token ending with end Token
//e: char to add at the end of each print 
void tokens_print(Token*toks,char*e){
    int l=token_len(toks);
    for (int i = 0; i < l; i++){
        printf("%d\n",i);
        token_print(toks[i],e);
    }
}

//free the value of given token
int free_tok_val(Token x){
    switch (x.type){
        case op:
            free(x.value.t);break;
        case nil:
            free(x.value.i);break;
        case ount:
            free(x.value.i);break;
        case str:
            free(x.value.s);break;
        case floap:
            free(x.value.f);break;
        case end:
            free(x.value.i);break;
        case boolean:
            free(x.value.b);break;
        case comp:
            free(x.value.c);break;
        case keyword:
            free(x.value.t);break;
        case syntax:
            free(x.value.t);break;
        case identifier:
            free(x.value.s);break;
        default:
            return 1;
    }
    return 0;
}

int id_acceptable(char v){
    char s[2]={v,'\0'};
    if(op_to_enum(s)!=-1){
        return 0;
    }
    if(sy_to_enum(s)!=-1){
        return 0;
    }
    if(v==' '||v=='\t'||v=='\n'){
        return 0;
    }
    
    return 1;
}

//count number of token in token list ended with endtoken
int Token_count(Token*tok,int type,__value value){
    int len=token_len(tok);
    int x=0;
    for(int i=0;i<len;i++){
        if(tok[i].type==type){
            switch (tok[i].type){
            case op:
                if(tok[i].value.t==value.t){
                    x++;
                }
                break;
            case nil:
                if(tok[i].value.i==value.i){
                    x++;
                }
                break;
            case ount:
                if(tok[i].value.i==value.i){
                    x++;
                }
                break;
            case str:
                if(tok[i].value.s==value.s){
                    x++;
                }
                break;
            case floap:
                if(tok[i].value.f==value.f){
                    x++;
                }
                break;
            case end:
                if(tok[i].value.i==value.i){
                    x++;
                }
                break;
            case boolean:
                if(tok[i].value.b==value.b){
                    x++;
                }
                break;
            case comp:
                if(tok[i].value.c==value.c){
                    x++;
                }
                break;
            case keyword:
                if(tok[i].value.t==value.t){
                    x++;
                }
                break;
            case syntax:
                if(tok[i].value.t==value.t){
                    x++;
                }
                break;
            case identifier:
                if(tok[i].value.s==value.s){
                    x++;
                }
                break;
            default:
                x=x;
            }

        }

    }
    return x;

}