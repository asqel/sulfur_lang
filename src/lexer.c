#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/token_class.h"
#include "../include/utilities.h"
#include "../include/lexer.h"

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
        if(text[p]=='/'&&p+1<len&&text[p+1]=='/'){
            while(p<len&&text[p]!='\n'){
                p++;
            }
            continue;
        }
        if(text[p]=='/'&&p+1<len&&text[p+1]=='*'){
            p+=2;
            while(p<len){
                if(text[p]=='*'&&p+1<len&&text[p+1]=='/'){
                    p+=2;
                    break;
                }
                else if(text[p]=='\n'){
                    line++;
                    p++;
                }
                else{
                    p++;
                }
            }
            continue;
        }
        if(text[p]=='\n'){
            line++;
            p++;
            continue;
        }
        if(text[p]=='\t'||text[p]==' '||text[p]=='\r'){
            p++;
            continue;
        }
        if((text[p]=='1'||text[p]=='0')&&p+1<len&&(text[p+1]=='b'||text[p+1]=='B')){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=boolean_t;
            toks[n_tok-1].line=line;
            toks[n_tok-1].value.b=malloc(sizeof(short int));
            *toks[n_tok-1].value.b=text[p]=='0'?0:1;
            p+=2;
            continue;
        }
        if(text[p]=='$' && p+1<len&& (text[p+1]=='i'||text[p+1]=='I')){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=comp;
            toks[n_tok-1].line=line;
            toks[n_tok-1].value.c=malloc(sizeof(long double)*n_tok);
            toks[n_tok-1].value.c[0]=0.0;
            toks[n_tok-1].value.c[1]=1.0;
            p +=2;
            continue;


        }
        if(text[p] == '0' && p + 2 < len && text[p + 1]){
            int start = p + 2;
            int end = -1;
            int i = start;
            long long int result = 0;
            while(i < len && (('0' <= text[i] && text[i] <= '9') || ('a' <= text[i] && text[i] <= 'f') || ('A' <= text[i] && text[i] <= 'F')) )
                i++;
            end = i;
            long long int power_sixteen = 1;
            for(i = end - 1; i >= start; i--){
                if(text[i] <= '9' && text[i] >= '0'){
                    result += power_sixteen * (text[i] - '0');
                    power_sixteen *= 16;
                }
                else if(text[i] <= 'f' && text[i] >= 'a'){
                    result += power_sixteen * (text[i] - 'a' + 10);
                    power_sixteen *= 16;
                }
                else if(text[i] <= 'F' && text[i] >= 'A'){
                    result += power_sixteen * (text[i] - 'A' + 10);
                    power_sixteen *= 16;
                }
            }
            n_tok++;
            toks = realloc(toks,sizeof(Token) * n_tok);
            toks[n_tok - 1].line = line;
            toks[n_tok - 1].value.i = malloc(sizeof(long long int));
            *toks[n_tok - 1].value.i = result;
            toks[n_tok - 1].type = ount;
            p = end;
            continue;

            
        }
        if(str_contains_char(DIGITS,text[p])){
            int e=p;
            int longeur=1;
            e++;
            while(e<len&&(str_contains_char(DIGITS,text[e])||text[e]=='.')){
                e++;
                longeur++;
            }
            char*s=malloc(sizeof(char)*(longeur+1));
            for(int i=0;i<longeur;i++){
                s[i]=text[p+i];
            }
            s[longeur]='\0';

            int n=str_count(s,'.');
            if(n==0){
                n_tok++;
                toks=realloc(toks,sizeof(Token)*n_tok);
                toks[n_tok-1].line=line;
                toks[n_tok-1].value.i=str_to_llint_p(s);
                toks[n_tok-1].type=ount;
                p+=longeur;
                free(s);
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
            p+=longeur;
            free(s);
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
                p+=2;
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
        int len=e-st+2;
        char*m=malloc(sizeof(char)*(e-st+2));
        for(int i=0;i<len-1;i++){
            m[i]=text[st+i];
        }
        m[len-1]='\0';
        int n=kw_to_enum(m);
        if(n!=-1){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=keyword;
            toks[n_tok-1].line=line;
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=kw_to_enum(m);
            p=e+1;
            free(m);
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
 



