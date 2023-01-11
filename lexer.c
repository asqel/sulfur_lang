#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libs/token_class.c"
#include "libs/utilities.c"

Token*lexe(char*text){
    Token*toks;
    int n_tok=0;
    int len=strlen(text);
    int p=0;
    n_tok++;
    toks=malloc(sizeof(Token));
    toks[n_tok-1]=nil_token;
    while(p<len){
        if(text[p]=='\n'||text[p]=='\t'||text[p]==' '){
            p++;
            continue;
        }
        if((text[p]=='1'||text[p]=='0')&&p+1<len&&(text[p+1]=='b'||text[p+1]=='B')){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=boolean;
            toks[n_tok-1].value.b=malloc(sizeof(short int));
            *toks[n_tok-1].value.b=text[p]=='0'?0:1;
            p+=2;
            continue;
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
                toks[n_tok-1].value.f=malloc(sizeof(long long int));
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
            *toks[n_tok-1].value.f=strtold(s,NULL);
            toks[n_tok-1].type=floap;
            p=e;
            continue;


        }
        
        if(p+1<len){
            char s[3]={text[p],text[p+1],'\0'};
            int n=-1;
            for(int i=0;i<ops_len;i++){
                if(strcmp(s,OPS[i])){
                    n=i;
                    break;
                }
            }
            char k[2]={text[p],'\0'};
            if(n==-1){
                for(int i=0;i<ops_len;i++){
                    if(strcmp(k,OPS[i])){
                        n=i;
                        break;
                    }
                }
                if(n!=-1){
                    n_tok++;
                    toks=realloc(toks,sizeof(Token)*n_tok);
                    toks[n_tok-1].type=op;
                    toks[n_tok-1].value.t=malloc(sizeof(short int));
                    *toks[n_tok-1].value.t=n;
                    p++;
                    continue;
                }
            }
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=op;
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=n;
            p+=2;
            continue;

        }
        char k[2]={text[p],'\0'};
        int n=-1;
        for(int i=0;i<ops_len;i++){
            if(k[0]==OPS[i][0]&&k[1]==OPS[i][1]){
                n=i;
                break;
            }
        }
        if(n!=-1){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=op;
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=n;
            p++;
            continue;
        }
        n=-1;
        for(int i=0;i<syntax_len;i++){
            if(SYNTAX[i][0]==text[p]){
                n=i;
            }
        }
        if(n!=-1){
            n_tok++;
            toks=realloc(toks,sizeof(Token)*n_tok);
            toks[n_tok-1].type=syntax;
            toks[n_tok-1].value.t=malloc(sizeof(short int));
            *toks[n_tok-1].value.t=n;
            p++;
            continue;
        }
    }
    for(int i=0;i<n_tok-1;i++){
        toks[i]=toks[i+1];
    }//c'est pour enlever le premier token qui est normalemennt nil et ajouter le end
    toks[n_tok-1]=end_token;
    return toks;
}


int main(int arc,char** argv){
    Token*x=lexe("3.1415926535897932 1b 7-(=+!=");
    token_print(x,"\n");
    free(x);   
    return 0;
}