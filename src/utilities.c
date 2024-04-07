#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sulfur.h"
#include <math.h>
#include "../include/utilities.h"
#include "../include/memlib.h"
#include <limits.h>

//used for abs_path
#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

#ifdef __profanOS__
#include <profan/filesys.h>
#include <profan/syscall.h>
#define PATH_MAX 256
#else
#include <linux/limits.h>
#endif

//check if a char is in a string
//*x pointer to the string  |  v char to search
int str_contains_char(char *x,char v){
    int n=0;
    for (unsigned int i = 0; i < strlen(x); i++){
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
    #elif __profanOS__
        char *tmp = getenv("PWD");
        if (tmp == NULL) tmp = "/";
        strcpy(path, tmp);
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
    for(unsigned int i=0;i<strlen(x);i++){
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
char* uti_dirname(char*v){
    int n = -1;
    int l = strlen(v);

    for(int i = 0; i < l; i++){
        if(v[i] == '/' || v[i] == '\\'){
            n = i;
        }
    }
    if(n==-1){
        return uti_strdup(" ");
    }
    char old = v[n];
    v[n] = '\0';
    char* s = malloc(sizeof(char) * (1 + strlen(v)));
    strcpy(s, v);
    v[n] = old;
    
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

char*str_cat_new3(char *s1, char *s2, char *s3){
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int l3 = strlen(s3);
    char *s = malloc(sizeof(char) * (l1 + l2 + l3 +1));
    for(int i = 0; i < l1; i++){
        s[i]=s1[i];
    }
    for(int i = 0; i < l2; i++){
        s[i+l1]=s2[i];
    }
    for(int i = 0; i < l3; i++) {
        s[i + l1 + l2] = s3[i];
    }
    s[l1 + l2 + l3] = '\0';
    return s;
}

int uti_is_path_relative(char *path) {
    #ifdef _WIN32
        return !(((path[0] <= 'Z' && path[0] >= 'A') || (path[0] <= 'z' && path[0] >= 'a')) && path[1] == ':' && (path[2] == '/' || path[2] == '\\'));
    #else
        return path[0] != '/';
    #endif
}

char *read_file(char *path) {
    #ifdef __profanOS__
    sid_t file_id = fu_path_to_sid(ROOT_SID, path);

    if (IS_NULL_SID(file_id) || !fu_is_file(file_id)) {
        return NULL;
    }
    
    uint32_t size = fu_get_file_size(file_id);
    char *text = malloc(size + 1);
    text[size] = '\0';

    fu_file_read(file_id, text, 0, size);

    return text;

    #else

    // check if file exists
    FILE *f = fopen(path, "r");
    if (f == NULL) return NULL;

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
    #endif
}

char *uti_read_bin_file(char *path, int *len) {
    #ifdef __profanOS__
    sid_t file_id = fu_path_to_sid(ROOT_SID, path);

    if (IS_NULL_SID(file_id) || !fu_is_file(file_id)) {
        return NULL;
    }
    
    uint32_t size = fu_get_file_size(file_id);
    char *text = malloc(size);

    fu_file_read(file_id, text, 0, size);
    
    *len = size;
    return text;
    #else
    // check if file exists
    FILE *f = fopen(path, "r");
    if (f == NULL) return NULL;

    char *text = malloc(sizeof(char));
    int n = 1;
    char c = fgetc(f);
    text[n-1] = c;
    while(c != EOF){
        n++;
        text = realloc(text, sizeof(char) * n);
        c = fgetc(f);
        text[n - 1] = c;
    }
    text[n-1]='\0';
    fclose(f);
    *len = n - 2;
    return realloc(text, sizeof(char) * (n - 1));
    #endif
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
    #ifndef __profanOS__
        *a=atoll(s);
    #else
        *a=(long long int) atoi(s);
    #endif
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
        free(mem);
        return (void*)x;
    }
    if(old_size>new_size){
        void*x=malloc(new_size);
        memcpy(x,mem,new_size);
        free(mem);
        return (void*)x;
    }
    if(new_size==0){
        free(mem);
        return NULL;
    }
    return NULL;
}

int is_letter(char v){
    if('A'<=v && v<='Z')
        return 1;
    if('a' <= v && v<='z')
        return 1;
    return 0;
}

#ifndef ONE_FILE
    #ifdef _WIN32
        #include <windows.h>
    #elif __profanOS__
        //nothing dont import dlfcn
    #elif __APPLE__ || __linux__
        #include <dlfcn.h>
    #endif
#endif

#include "../include/memlib.h"
#include <string.h>


void *get_module_loader(char* filename) {
    #ifdef ONE_FILE
    void* mod = get_standard_module(filename);
    if (mod) {
        return mod;
    }
    return NULL;
    #endif

    #ifndef ONE_FILE
    filename = uti_strdup(filename);
    if(!(is_letter(filename[0]) && filename[1]==':') && filename[0] != '/'){//check if it's absolute
        char * interpreter =abs_path();
        char* dir = uti_dirname(interpreter);
        free(interpreter);
        char *lib_dir=str_cat_new(dir,"/libs/");
        free(dir);
        char *old_filename = filename;
        filename=str_cat_new(lib_dir,filename);
        free(old_filename);
        free(lib_dir);
    }
    void* loader_function = NULL;
    #ifdef _WIN32
        HINSTANCE handle = LoadLibrary(filename);
        if (handle != NULL) {
            add_dyn_lib(handle);
            loader_function = GetProcAddress(handle, "__loader");
        }
        else{
            char *old_filename = filename;
            char *filename=str_cat_new(filename,".dll");
            free(old_filename);
            HINSTANCE handle = LoadLibrary(filename);
            if (handle != NULL) {
                add_dyn_lib(handle);
                loader_function = GetProcAddress(handle, "__loader");
            }
            else{
                printf("ERROR in loading library %s\n",filename);
                exit(1);
            }
        }
    #elif __profanOS__
        //nothing
    #elif __APPLE__ || __linux__
        void* handle = dlopen(filename, RTLD_LAZY);
        if (handle != NULL) {
            add_dyn_lib(handle);
            loader_function = dlsym(handle, "__loader");
        }
        else{
            char *old_filename = filename;
            filename=str_cat_new(filename,".so");
            free(old_filename);
            handle = dlopen(filename, RTLD_LAZY);
            if (handle != NULL) {
                add_dyn_lib(handle);
                loader_function = dlsym(handle, "__loader");
            }
            else{
                printf("ERROR in loading library %s\n",filename);
                exit(1);
            }
        }
    #endif
    free(filename);
    return loader_function;
    #endif
}

#ifdef ONE_FILE

extern Object __load_math(Sulfur_ctx ctx);
extern Object __load_graphic(Sulfur_ctx ctx);
extern Object __load_poppy(Sulfur_ctx ctx);
extern Object __load_lilypad(Sulfur_ctx ctx);
extern Object __load_why(Sulfur_ctx ctx);

void* get_standard_module(char* filename){
    if(!strcmp(filename, "math")){
        return &__load_math;
    }
    if(!strcmp(filename, "graphic")){
        return &__load_graphic;
    }
    if(!strcmp(filename, "poppy")){
        return &__load_poppy;
    }
    if(!strcmp(filename, "lilypad")){
        return &__load_lilypad;
    }
    if(!strcmp(filename, "why")){
        return &__load_why;
    }
    return NULL;
}
#endif

char *uti_strdup(char *src) {
    int len = strlen(src);
    char *res = malloc(sizeof(char) * (1 + len));
    strcpy(res, src);
    return res;
}


char *normalize_path(char *path) {
    // normalize absolute files (/folder/folder2/../file.txt -> /folder/file.txt)
    // may return a copy of path
    // path must have / not back slash as separator
    int len = strlen(path);
    char *res = calloc(1 + len, sizeof(char));
    int p = 0;
    int str_end = 0;
    while (p < len) {
        if (path[p] == '.' && path[p + 1] == '/')
            p += 2;
        else if (path[p] == '.' && path[p + 1] == '.' && path[p + 2] == '/') {
            char *new_res = uti_dirname(res);
            memset(res, 0, sizeof(char) * (1 + len));
            memcpy(res, new_res, (strlen(new_res) + 1) * sizeof(char));
            res[str_end] = '/';
            p += 3;
        }
        else {

            res[str_end] = path[p] == '\\' ? '/' : path[p];
            p++;
            str_end++;
        }
    }
    res[str_end] = '\0';
    return realloc(res, sizeof(char) * (str_end + 1));
}