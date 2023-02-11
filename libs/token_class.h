#ifndef TOKEN_CLASS_H
#define TOKEN_CLASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    int line;
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
    def_t,//function 
    from_t,
    to_t
};

extern char KEYWORDS[8][7];
extern int keyword_len;

extern Token nil_token;
extern Token end_token;

extern char DIGITS[10];

extern char OPS[20][3];
extern int ops_len;

extern char SYNTAX[13][2];
extern int syntax_len;

//return the length of array of Token
int token_len(Token*toks);
char*get_kw(short int x);
char*get_op(short int x);
char*get_sy(short int x);

int kw_to_enum(char*v);
int op_to_enum(char*v);
int sy_to_enum(char*v);

void token_print(Token tok,char*e);

//print each Token of a list with at end of print the string to print
//toks:array of Token ending with end Token
//e: char to add at the end of each print 
void tokens_print(Token*toks,char*e);

//free the value of given token
int free_tok_val(Token x);

int id_acceptable(char v);

int Token_count(Token*tok,int type,__value value);//ca marche pas en fait 
//faut faire token type value

#endif