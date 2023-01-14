#include <stdlib.h>
#include <stdio.h>


typedef union __value{
    long long int*i;//int len=1
    long double *f;//float len=1
    short int * b;//boolean len=1
    long double*c;//complex len=2 [real,imaginary]
    short int*t;//token
    char *s;//string
}__value;

//token list end with an end token
typedef struct Token{
    int type;
    __value value;
}Token;

//Token type
enum Token_t{
    nil,
    ount,
    str,
    floap,
    end,
    boolean,
    comp,//complex nnumber value : {double float , double float}
    op,//+ * - / ^(pow) %(mod) \(floor div) |(or) &(and) !(not) = == += -= != <= >= < >
    keyword,//if while for else elif class return def
    syntax,//#"'{}[]()`@:;.?,
    identifier
};

//Token value for syntax
enum Syntaxs{
    hashtag,// # 
    r_brack_L,// {
    r_brack_R,// }
    brack_L,// [
    brack_R,// ]
    par_L,// (
    par_R,// )
    at,// @
    colon,// :
    semicolon,// ;
    dot,// .
    q_mark,// ?
    comma// ,
};

//Token value for Operators
enum Operators {
  OP_PLUS,// +
  OP_MULTIPLY,// *
  OP_MINUS,// -
  OP_DIVIDE,// /
  OP_EXPONENT,// ^
  OP_MODULUS,// %
  OP_FLOOR_DIVIDE,/* \  */
  OP_OR,// |
  OP_AND,// &
  OP_NOT,// !
  OP_ASSIGN,// =
  OP_EQUAL,// ==
  OP_PLUS_ASSIGN,// +=
  OP_MINUS_ASSIGN,// -=
  OP_NOT_EQUAL,// !=
  OP_LESS_EQUAL,// <=
  OP_GREATER_EQUAL,// >=
  OP_LESS,// <
  OP_GREATER// >
};

//Token value for keywords
enum keyword{
    if_t,
    while_t,
    else_t,
    elif_t,
    for_t,
    class_t,
    return_t,
    def_t//function 

};

char KEYWORDS[][7]={"if","while","else","elif","for","class","return","def"};
int keyword_len=8;

Token nil_token={nil,{.i=&(long long int){0}}};
Token end_token={end,{.i=&(long long int){1}}};

char DIGITS[]="1234567890";

char OPS[][3]={"+","*","-","/","^","%","\\","|","&","!","=","==","+=","-=","!=","<=",">=","<",">"};
int ops_len=20;

char SYNTAX[][2]={"#","{","}","[","]","(",")","@",":",";",".","?",","};
int syntax_len=16;

//return the length of array of Token
int token_len(Token*toks){
    int x=0;
    for(int i=0;toks[i].type!=end;i++){
        x++;
    }
    return x;
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
            printf("Tf:[%Lf]%s",*(tok.value.f),e);
            break;
        case ount:
            printf("Ti:[%d]%s",*(tok.value.i),e);
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
            printf("Op:[%s]%s",get_op(*(tok.value.t)),e);//faut faire le get_op et get_sy
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
        token_print(toks[i],e);
    }
}

//free the value of given token
int free_tok_val(Token x){
    switch (x.type){
        case op:
            free(x.value.t);return 0;
        case nil:
            free(x.value.i);return 0;
        case ount:
            free(x.value.i);return 0;
        case str:
            free(x.value.s);return 0;
        case floap:
            free(x.value.f);return 0;
        case end:
            free(x.value.i);return 0;
        case boolean:
            free(x.value.b);return 0;
        case comp:
            free(x.value.c);return 0;
        case keyword:
            free(x.value.t);return 0;
        case syntax:
            free(x.value.t);return 0;
        case identifier:
            free(x.value.s);return 0;
        default:
            return 1;
    }
}

int id_acceptable(char v){
    char*s=malloc(sizeof(char)*2);
    s[0]=v;
    s[1]='\0';
    if(op_to_enum(s)!=-1){
        free(s);
        return 0;
    }
    if(sy_to_enum(s)!=-1){
        free(s);
        return 0;
    }
    free(s);
    return 1;
}