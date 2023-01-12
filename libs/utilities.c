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
        if(x[i]==v){
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
        if(v[i]=='\\'){
            n=i;
        }
    }
    if(n==-1){
        return NULL;
    }
    char*s=malloc(sizeof(char)*(n+1));
    for(int i=0;i<n;i++){
        s[i]=v[i];
    }
    s[n+1]='\0';
    printf("tamere° %s tamere ",s);
    return s;
}
//return new str containing s1+s2
char*str_cat_new(char*s1,char*s2){
    int l1=strlen(s1);
    int l2=strlen(s2);
    char*s=malloc(sizeof(char)*(l1+l2));
    for(int i=0;i<l1;i++){
        s[i]=s1[i];
    }
    for(int i=0;i<l2;i++){
        s[i+l1-1]=s2[i];
    }
    return s;
}

char*read_file(char*path){
    printf(path);
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
    return text;
}

//return the max power of 10 that can fit into a number
//for number 
//ex:
//    1000->3 ; 76787766->7 bc 10^7<76787766<10^8
int get_power10(float x){
    int n=0;
}