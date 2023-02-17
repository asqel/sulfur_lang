#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "token_class.h"
#include "utilities.h"
#include "lexer.h"

Token*lexe(char*text){
    int line=1;
    Token*toks;
    int n_tok=0;
    int len=strlen(text);
    int p=0;
    n_tok++;
    toks=malloc(sizeof(Token));
    toks[n_tok-1]=nil_token;
    while(p<len){
        if(text[p]=='\n'){
            line++;
            p++;
            continue;
        }
        if(text[p]=='\t'||text[p]==' '){
            p++;
            continue;
        }
        if((text[p]=='1'||text[p]=='0')&&p+1<len&&(text[p+1]=='b'||text[p+1]=='B')){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=boolean;
            toks[n_tok-1].line=line;
            toks[n_tok-1].value.b=malloc(sizeof(short int));
            *toks[n_tok-1].value.b=text[p]=='0'?0:1;
            p+=2;
            continue;
        }
        if(text[p]=='$' && p+1<len&& (text[p]=='i'||text[p]=='I')){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=comp;
            toks[n_tok-1].line=line;
            toks[n_tok-1].value.c=malloc(sizeof(long double)*n_tok);
            toks[n_tok-1].value.c[0]=0.0;
            toks[n_tok-1].value.c[0]=1.0;


        }
        if(str_contains_char(DIGITS,text[p])){
            int e=p;
            while(e<len&&(str_contains_char(DIGITS,text[e])||text[e]=='.')){
                e++;
            }
            char*s=malloc(sizeof(char)*(e-p));
            for(int i=p;i<e;i++){
                s[i-p]=text[i];
            }
            int n=str_count(s,'.');
            if(n==0){
                n_tok++;
                toks=realloc(toks,sizeof(Token)*n_tok);
                toks[n_tok-1].line=line;
                toks[n_tok-1].value.i=malloc(sizeof(long long int));
                printf("--%x--",toks[n_tok-1].value.i);
                *toks[n_tok-1].value.i=atoll(s);
                toks[n_tok-1].type=ount;
                p=e;
                continue;
            }
            if(n>1){
                printf("ya une erreur la sur les floap en fait");
                exit(0);
            }
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].value.f=malloc(sizeof(long double));
            long double v=0;
            int len=strlen(s);
            int dotidx=0;
            for(int i=0;i<len;i++){
                if(s[i]=='.'){
                    dotidx=i;
                    break;
                }
            }
            for(int i=dotidx-1;i>=0;i--){
                char c[2]={s[i],'\0'};
                v+=pow(10,dotidx-i-1)*atoi(c);
            }
            for(int i=dotidx+1;i<len;i++){
                char c[2]={s[i],'\0'};
                v+=pow(10,dotidx-i)*atoi(c);
            }
            *toks[n_tok-1].value.f=v;
            toks[n_tok-1].type=floap;
            toks[n_tok-1].line=line;
            p=e;
            continue;
        }
        if(p+1<len){//ops 2 char
            char s[3]={text[p],text[p+1],'\0'};
            int x=op_to_enum(s);
            if(x!=-1){
                n_tok++;
                toks=realloc(toks,sizeof(Token)*n_tok);
                toks[n_tok-1].value.t=malloc(sizeof(short int));
                *toks[n_tok-1].value.t=x;
                toks[n_tok-1].type=op;
                toks[n_tok-1].line=line;
                p++;
                continue; 
            }
        }
        char s[2]={text[p],'\0'};
        if(op_to_enum(s)!=-1){//ops single char
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=op_to_enum(s);
            toks[n_tok-1].type=op;
            toks[n_tok-1].line=line;
            p++;
            continue;  
        }
        char s2[2]={text[p],'\0'};
        if(sy_to_enum(s2)!=-1){//syntax
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=sy_to_enum(s2);
            toks[n_tok-1].type=syntax;
            toks[n_tok-1].line=line;
            p++;
            continue;  
        }
        if(text[p]=='\''){
            p++;
            int n=0;
            char*s=malloc(sizeof(char));
            while(p<len&&text[p]!='\''){
                n++;
                s=realloc(s,sizeof(char)*n);
                s[n-1]=text[p];
                p++;
            }
            s=realloc(s,sizeof(char)*(n+1));
            s[n]='\0';
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=str;
            toks[n_tok-1].value.s=s;
            toks[n_tok-1].line=line;
            p++;
            continue;
        }
        if(text[p]=='"'){
            p++;
            int n=0;
            char*s=malloc(sizeof(char));
            while(p<len&&text[p]!='"'){
                n++;
                s=realloc(s,sizeof(char)*n);
                s[n-1]=text[p];
                p++;
            }
            s=realloc(s,sizeof(char)*(n+1));
            s[n]='\0';
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=str;
            toks[n_tok-1].value.s=s;
            toks[n_tok-1].line=line;
            p++;
            continue;
        }
        int st=p;
        int e=p;
        for(int i=p;i<len&&id_acceptable(text[i]);i++){
            e=i;
        }
        char*m=malloc(sizeof(char)*(e-st+2));
        for(int i=st;i<=e;i++){
            m[i-st]=text[i];
        }
        m[e-st+1]='\0';
        int n=kw_to_enum(m);
        if(n!=-1){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=keyword;
            toks[n_tok-1].line=line;
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=kw_to_enum(m);
            p=e+1;
            continue;
        }
        n_tok++;
        toks=realloc(toks,sizeof(Token)*n_tok);
        toks[n_tok-1].type=identifier;
        toks[n_tok-1].value.s=m;
        toks[n_tok-1].line=line;
        p=e+1;
        continue;


        
        
        
        
    }
    for(int i=0;i<n_tok-1;i++){
        toks[i]=toks[i+1];
    }//c'est pour enlever le premier token qui est normalemennt nil et ajouter le end
    toks[n_tok-1]=end_token;
    return toks;
}
 



