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

enum Token_t{
    nil,
    ount,
    str,
    floap,
    end,
    boolean,
    comp,
    op,//+ * - / ^(pow) %(mod) \(floor div) |(or) &(and) !(not) = == += -= != <= >= < >
    keyword,//if while for else elif class return def
    syntax//#"'{}[]()`@:;.?,
};
enum Syntaxs{
    hashtag,// # 
    d_quote,// "
    s_quote,// '
    r_brack_L,// {
    r_brack_R,// }
    brack_L,// [
    brack_R,// ]
    par_L,// (
    par_R,// )
    backtick,// `
    at,// @
    colon,// :
    semicolon,// ;
    dot,// .
    q_mark,// ?
    comma// ,
};
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

char OPS[][3]={"+","*","-","/","^","%","//","|","&","!","=","==","+=","-=","!=","<=",">=","<",">"};
int ops_len=20;

char SYNTAX[][2]={"#","\"","'","{","}","[","]","(",")","`","@",":",";",".","?",","};
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
void token_print(Token*toks,char*end){
    int l=token_len(toks);
    for (int i = 0; i < l; i++){
        switch (toks[i].type){
        case floap:
            printf("Tf:[%Lf]%s",*(toks[i].value.f),end);
            break;
        case ount:
            printf("Ti:[%d]%s",*(toks[i].value.i),end);
            break;
        case str:
            printf("Ts:[%s]%s",toks[i].value.s,end);
            break;
        case boolean:
            printf("Tb:[%d]%s",*(toks[i].value.b),end);
            break;
        case keyword:
            printf("Kw:[%s]%s",get_kw(*(toks[i].value.t)),end);
            break;
        case syntax:
            printf("Sy:[%s]%s",get_sy(*(toks[i].value.t)),end);
            break;
        case op:
            printf("Op:[%s]%s",get_op(*(toks[i].value.t)),end);//faut faire le get_op et get_sy
            break;
        default:
            break;
    }
    

    }
}
