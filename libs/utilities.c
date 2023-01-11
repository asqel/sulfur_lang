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
int str_ar_contains_str(char**list,char*v,int list_len){
    for(int i=0;i<list_len;i++){
        if(strcmp(list[i],v)){
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