
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/utilities.h"

//check if a char is in a string
//*x pointer to the string  |  v char to search
int str_contains_char(char *x,char v){
    int n=0;
    for (int i = 0; i < strlen(x); i++){
        if(x[i]==v){
            return 1;
        }
    }
    return 0;
    
}

//check if a string is in a list of string
//list:the list | v:the string to searche | list_len:length of the list
int str_ar_contains_str(char*list[],char*v,int list_len){
    for(int i=0;i<list_len;i++){
        if(!strcmp(list[i],v)){
            return 1;
        }
    }
    return 0;
}

int str_ar_contains_char(char*list[],char v,int list_len){
    for(int i=0;i<list_len;i++){
        printf(list[i]);
        if(list[i][0]==v){
            return 1;
        }
    }
    return 0;
}

//return the number of occurrence of a char in a string
//*x the string   ||  v the char to count
int str_count(char*x,char v){
    int n=0;
    for(int i=0;i<strlen(x);i++){
        if(*(x+i)==v){
            n++;
        }
    }
    return n;
}

//turn a path with \ into a path with /
void back_slash_to_path(char*v){
    int l=strlen(v);
    for(int i=0;i<l;i++){
        if(v[i]=='\\'){
            v[i]='/';
        }
    }
}
char*dirname(char*v){
    int n=-1;
    int l=strlen(v);
    for(int i=0;i<l;i++){
        if(v[i]=='/'){
            n=i;
        }
    }
    if(n==-1){
        return NULL;
    }
    char old=v[n];
    v[n]='\0';
    char*s=malloc(sizeof(char)*strlen(v));
    strcpy(s,v);
    v[n]=old;
    
    return s;
}
//return new str containing s1+s2
char*str_cat_new(char*s1,char*s2){
    int l1=strlen(s1);
    int l2=strlen(s2);
    char*s=malloc(sizeof(char)*(l1+l2+1));
    for(int i=0;i<l1;i++){
        s[i]=s1[i];
    }
    for(int i=0;i<l2;i++){
        s[i+l1]=s2[i];
    }
    s[l1+l2]='\0';
    return s;
}

char*read_file(char*path){
    FILE*f=fopen(path,"r");
    char*text=malloc(sizeof(char));
    int n=1;
    char c=fgetc(f);
    text[n-1]=c;
    while(c!=EOF){
        n++;
        text=realloc(text,sizeof(char)*n);
        c=fgetc(f);
        text[n-1]=c;
    }
    text[n-1]='\0';
    return text;
}

//return the max power of 10 that can fit into a number
//for number greater or equal than 1 
//ex:
//    1000->3 ; 76787766->7 bc 10^7<76787766<10^8
int get_power10(long double x){
    return (int)log10(x);
}

int get_nbr_of_digits(long long int x){
    return 1+(int)log10(x);
}

//faire des fonnction int to str qui renvoie un nouveau char* en utilisant nb_of_digits
//faire pareil pour les float en ajouter un parametre precision si p=0 alors le traiter comme un int si alors *10^1 et le traiter comme  un int et cherccher ou faut metre la virgule

long long int*str_to_llint_p(char*s){
    long long int*x=malloc(sizeof(long long int));
    *x=0;
    int len=strlen(s);
    long long int pow_10=1;
    for(int i=0;i<len;i++){
        (*x)+=pow_10*(long long int)(s[i]-'0');
        pow_10*=10;
    }
    return x;
}

long long int str_to_llint(char*s){
    long long int x=0;
    int len=strlen(s);
    long long int pow_10=1;
    for(int i=0;i<len;i++){
        x+=pow_10*(long long int)(s[i]-'0');
        pow_10*=10;
    }
    return x;
}