
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/utilities.h"
#include <limits.h>

//used for abs_path
#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

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

char*abs_path(){
    char *path=malloc(sizeof(char)*(PATH_MAX+1));
    #ifdef _WIN32
        if (GetModuleFileName(NULL, path, PATH_MAX) == 0) {
            perror("GetModuleFileName");
            exit(EXIT_FAILURE);
        }
    #elif __APPLE__
        uint32_t size = PATH_MAX;
        if (_NSGetExecutablePath(path, &size) != 0) {
            perror("_NSGetExecutablePath");
            exit(EXIT_FAILURE);
        }
    #else
        if (readlink("/proc/self/exe", path, PATH_MAX) == -1) {
            perror("readlink");
            exit(EXIT_FAILURE);
        }
    #endif
    return path;
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
        if(v[i]=='/'|| v[i]=='\\'){
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
    fclose(f);
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
    //long long int x=0;
    //int len=strlen(s);
    //long long int pow_10=1;
    //for(int i=0;i<len;i++){
    //    x+=pow_10*(long long int)(s[i]-'0');
    //    pow_10*=10;
    //}
    long long int*a=malloc(sizeof(long long int));
    *a=atoll(s);
    return a;
}

long long int str_to_llint(char*s){
    long long int x=0;
    int len=strlen(s);
    long long int pow_10=1;
    for(int i=len-1;i>-1;i--){
        x+=pow_10*(long long int)(s[i]-'0');
        pow_10*=10;
    }
    return x;
}

//long double str_to_ldouble(char*s){
//    long double x=0;
//    int n=-1;
//    int len=strlen(s);
//    for(int i=0;i<len;i++){
//        if(s[i]=='.'){
//            n=i;
//            continue;
//        }
//        if(n!=-1 && s[i]=='.'){
//            return x;
//        }
//    }
//    char*left=malloc(sizeof(char)*n);
//    for(int i=0;i<n;i++){
//        left[i]=s[i];
//    }
//    x+=str_to_llint(left);
//    char*right=malloc(sizeof(char)*(len-n-1));
//    for(int i=n+1;i<len;i++){
//        right[i-n-1]=s[i];
//    }
//    long long int x2=str_to_llint(right);
//    len=strlen(right);
//    int tens=0;
//    for(int i=0;i<len;i++){
//        if(right[i]=='0'){
//            tens+=1;
//        }
//    }
//    x+=x2/(pow(10,tens));
//    free(right);
//    free(left);
//    return x;
//}

void*realloc_c(void*mem,long long int old_size,long long int new_size){
    if(old_size==new_size){
        return mem;
    }
    if(old_size<new_size){
        void*x=malloc(new_size);
        memcpy(x,mem,old_size);
        return (void*)x;
    }
    if(old_size>new_size){
        void*x=malloc(new_size);
        memcpy(x,mem,new_size);
        return (void*)x;
    }
    if(new_size==0){
        return NULL;
    }
}

int is_letter(char v){
    if('A'<=v && v<='Z'){
        return 1;
    }
    if('a' <= v && v<='z'){
        return 1;
    }
    return 0;
}


#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__ || __linux__
    #include <dlfcn.h>  
#endif
#include "../include/memlib.h"


void *get_module_loader(char* filename) {
    if(!(is_letter(filename[0]) && filename[1]==':') && filename[0] != '/'){//check if it's absolute
        char * interpreter =abs_path();
        char* dir=dirname(interpreter);
        free(interpreter);
        char *lib_dir=str_cat_new(dir,"/libs/");
        free(dir);
        filename=str_cat_new(lib_dir,filename);
        free(lib_dir);
    }
    void* loader_function = NULL;
    #ifdef _WIN32
        HINSTANCE handle = LoadLibrary(filename);
        if (handle != NULL) {
            loader_function = GetProcAddress(handle, "__loader");
        }
        else{
            filename=str_cat_new(filename,".dll");
            HINSTANCE handle = LoadLibrary(filename);
            if (handle != NULL) {
                loader_function = GetProcAddress(handle, "__loader");
            }
            else{
                printf("ERROR in loading library %s\n",filename);
                exit(1);
            }
            free(filename);
        }
        
    #elif __APPLE__ || __linux__
        void* handle = dlopen(filename, RTLD_LAZY);
        if (handle != NULL) {
            loader_function = dlsym(handle, "__loader");
        }
        else{
            filename=str_cat_new(filename,".so");
            handle = dlopen(filename, RTLD_LAZY);
            if (handle != NULL) {
                loader_function = dlsym(handle, "__loader");
            }
            else{
                printf("ERROR in loading library %s\n",filename);
                exit(1);
            }
            free(filename);
        }
    #endif
    return loader_function;
}