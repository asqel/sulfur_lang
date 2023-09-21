#include "std.h"

#include "string_su.h"
#include "list_su.h"
#include "funccall_su.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"
#include "../../include/sulfur.h"

#ifdef __profanOS__
    #include <syscall.h>
#endif

#ifdef _WIN32
    char* __os__="windows";
#elif __APPLE__
    char* __os__="APPLE";
#elif __profanOS__
    char* __os__="profanOS";
#elif __linux__
    char* __os__="linux";
#else 
    char* __os__="UNKNOWN";
#endif

extern memory MEMORY;

int precision = 5;
int base_precision = 6;
int is_flushing = 0;

Object read_prompt(Object*o,int n_arg) {
    int capacity = 16;
    int length = 0;
    char* result = malloc(capacity * sizeof(char));
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        result[length++] = c;
        if (length == capacity) {
            capacity+=10;
            result = realloc(result, capacity * sizeof(char));
        }
    }
    result[length++]='\0';
    result=realloc(result, length * sizeof(char));
    return (Object){.type=Obj_string_t,.val.s=result};
}
 

Object print_prompt(Object*obj,int n_arg){
    if(n_arg>1){
        for(int i=0;i<n_arg-1;i++){
            print_prompt(&obj[i],1);
            printf(" ");
        }
        if (is_flushing){
            fflush(stdout);
        }
        return print_prompt(&obj[n_arg-1],1);

    }
    if(obj->type==Obj_string_t){
        printf("%s",obj->val.s);
    }
    else if(obj->type==Obj_boolean_t){
        printf(obj->val.b == 0 ? "0b" : "1b");
    }
    else if(obj->type==Obj_class_t){
        
    }
    else if(obj->type==Obj_complex_t){
        printf("%Lf + %Lfi",obj->val.c[0],obj->val.c[1]);
    }
    else if(obj->type==Obj_floap_t){
        char tmp[30];
        sprintf(tmp, "%%.%dLf", precision);
        printf(tmp, obj->val.f);
    }
    else if(obj->type==Obj_list_t){
        int len=obj->val.li->elements[0].val.i;
        if(len==0){
            printf("[]");
            if (is_flushing){
                fflush(stdout);
            }
            return nil_Obj;
        }
        printf("[");
        for(int i=1;i<len;i++){
            print_prompt(&obj->val.li->elements[i],1);
            printf(",");
        }
        print_prompt(&obj->val.li->elements[len],1);
        printf("]");

    }
    else if(obj->type==Obj_funcid_t){
        printf("function at :%p",obj->val.funcid);
    }
    else if(obj->type==Obj_nil_t){
        printf("nil");
    }
    else if(obj->type==Obj_Obj_t){
        //print_prompt(obj->val.o,);
    }
    else if(obj->type==Obj_ount_t){
        printf("%lld",obj->val.i);
    }
    else if(obj->type==Obj_typeid_t){
        printf("%s",obj->val.typeid);
    }
    else if(obj->type == obj_module_t){
        printf("{\n");
        for(int i=0;i<obj->val.module->MEM->len;i++){
            printf("%s : ",obj->val.module->MEM->keys[i]);
            println_prompt(&obj->val.module->MEM->values[i],1);
        }
        printf("\n}");
    }
    if (is_flushing){
        fflush(stdout);
    }
    return nil_Obj;

}

Object println_prompt(Object*obj,int n_arg){
    Object n=print_prompt(obj,n_arg);
    printf("\n");
    return n;
}

Object std_chr(Object* argv, int argc){
    if(argc == 1 && argv[0].type == Obj_list_t){
        return std_chr(&argv->val.li->elements[1], argv->val.li->elements[0].val.i);
    }
    if(argc <= 0){
        printf("ERROR std_chr takes at least one arg\n");
        exit(1);
    }
    for(int i=0; i < argc; i++){
        if(argv[i].type != Obj_ount_t){
            printf("ERROR std_chr only takes strings\n");
            exit(1);
        }
    }
    char* text = malloc(sizeof(char) * (1 + argc));
    for(int i=0; i < argc; i++){ // sataan bouche un coin

        text[i] = argv[i].val.i;
    }
    text[argc] = '\0';
    return new_string(text);
}

Object std_bool(Object*obj,int n_arg){
    if(n_arg!=1){
        printf("ERROR %s only 1 argument needed in bool call\n", n_arg>1 ? "too many arguments" : "too few arguments");
        exit(1);
    }
    if(obj->type==Obj_ount_t){
        return new_boolean(obj->val.i != 0);
    }
    if(obj->type==Obj_string_t){
        return new_boolean(strlen(obj->val.s));
    }
    if(obj->type==Obj_boolean_t){
        return new_boolean(obj->val.b);
    }
    if(obj->type == Obj_list_t){
        return new_boolean(obj->val.li->elements[0].val.i);
    }
    return new_boolean(0);
}

Object std_ount(Object*obj,int n_arg){
    if(obj->type==Obj_ount_t){
        return new_ount(obj->val.i);
    }

    if(obj->type==Obj_floap_t){
        return new_ount((long long int) obj->val.f);
    }

    if(obj->type==Obj_boolean_t){
        return new_ount((long long int) obj->val.b);

    }
    if(obj->type==Obj_string_t){
        return new_ount(str_to_llint(obj->val.s));
    }
    return nil_Obj;
}
Object std_floap(Object*obj,int n_arg){
    if(obj->type==Obj_floap_t){
        return new_floap(obj->val.f);
    }
    Object res;
    res.type=Obj_floap_t;

    if(obj->type==Obj_ount_t){
        res.val.f=(long double)(obj->val.i);
        return res;
    }

    if(obj->type==Obj_boolean_t){
        res.val.f=(long double)(obj->val.b);
        return res;
    }
    if(obj->type==Obj_string_t){
        res.val.f = atof(obj->val.s);
        return res;
    }
    return nil_Obj;

}

Object std_set_flush(Object* obj, int n_arg){
    if(n_arg != 1){
        printf("ERROR in __set_flush__ only takes one arg\n");
        exit(1);
    }
    Object x = std_bool(obj, 1);
    is_flushing = x.val.b;
    Obj_free_val(x);
    return nil_Obj;
}

Object std_list(Object*obj,int n_arg){
    if(n_arg==0){
        Object x;
        x.type=Obj_list_t;
        x.val.li=malloc(sizeof(list));
        x.val.li->elements=malloc(sizeof(Object));
        x.val.li->elements[0] = new_ount(0);
        add_count(x.val.li,Obj_list_t);
        return x;
    }
    Object x;
    x.type=Obj_list_t;
    x.val.li=malloc(sizeof(list));
    x.val.li->elements=malloc(sizeof(Object)*(n_arg+1));
    x.val.li->elements[0]= new_ount(n_arg);
    for(int i=1;i<=n_arg;i++){
        x.val.li->elements[i]=Obj_cpy(obj[i-1]);
    }
    add_count(x.val.li,Obj_list_t);

    return x;
}

Object current_timestamp(Object *obj,int n_arg) {
    Object o;
    o.type = Obj_ount_t;

    #ifdef __profanOS__
        o.val.i = c_timer_get_ms();
    #else
        struct timespec te;
        clock_gettime(CLOCK_REALTIME, &te);
        long long milliseconds = te.tv_sec * 1000LL + te.tv_nsec / 1000000LL;
        o.val.i = milliseconds;
    #endif

    return o;
}

Object sleep(Object *obj,int n_arg){
    long long int x = 0;
    Object o = std_ount(&obj[0], 1);
    if(o.type != Obj_ount_t){
        return nil_Obj;
    }
    x = o.val.i;
    long long start = current_timestamp(NULL, 0).val.i;
    
    while(current_timestamp(NULL, 0).val.i < start + x){}
    return nil_Obj;
}

Object var_exists(Object* args, int n_arg){
    if(n_arg == 0){
        printf("var_exists takes at least one arg");
        exit(1);
    }
    for(int i = 0; i < n_arg; i++){
        if(args[i].type != Obj_string_t){
            printf("var_exists takes only strings as arg");
            exit(1);
        }
    }
    for(int i=0; i < n_arg; i++){
        int found = 0;
        for(int k = 0; k < MEMORY.len; k++){
            if(!strcmp(MEMORY.keys[k], args[i].val.s)){
                found = 1;
                break;
            }
        }
        if(!found){
            return new_boolean(0);
        }
    }
    return new_boolean(1);
}

Object get(Object *obj,int n_arg){
    if (n_arg != 2){
        printf("ERROR get only takes 2 arguments\n");
        exit(1);
    }
    if(obj[0].type != Obj_string_t && obj[0].type != Obj_list_t){
        printf("ERROR get only take list or string as first argument\n");
        exit(1);
    }
    Object index = std_ount(&(obj[1]), 1);
    if (index.type == Obj_nil_t){
        printf("ERROR get only take a ount-convetible as second argument\n");
        exit(1);
    }
    int len = 0;
    if (obj[0].type == Obj_string_t){
        len = strlen(obj[0].val.s);
        if (index.val.i == -1){
            Object res;
            res.type = Obj_ount_t;
            res.val.s = malloc(sizeof(long long int));
            *res.val.s = len;
            return res;
        }
        if (index.val.i >= len || index.val.i < -1){
            printf("ERROR get out of range\n");
            exit(1);
        }
        Object res;
        res.type = Obj_string_t;
        res.val.s = malloc(sizeof(char)*2);
        res.val.s[0] = obj[0].val.s[index.val.i];
        res.val.s[1] = '\0';
        return res;
    }
    len = obj[0].val.li->elements[0].val.i;
    if (index.val.i >= len || index.val.i < -1){
        printf("ERROR get out of range\n");
        exit(1);
    }
    return obj[0].val.li->elements[index.val.i + 1];

}

Object set(Object *obj,int n_arg){
    if(n_arg != 3){
        printf("ERROR set only takes 3 args\n");
        exit(1);
    }
    if(obj[0].type != Obj_list_t && obj[0].type != Obj_string_t){
        printf("ERROR set only takes string or list as first arg\n");
        exit(1);
    }
    if(obj[1].type != Obj_ount_t && obj[1].type != Obj_floap_t){
        printf("ERROR set only take ount or floap as second arg\n");
        exit(1);
    }
    int index;
    if(obj[1].type == Obj_ount_t){
        index =1+ obj[1].val.i;
    }
    if(obj[1].type == Obj_floap_t){
        index = 1 + (int)obj[1].val.f;
    }
    if (index > obj[0].val.li->elements[0].val.i || index<1){
        printf("ERROR set out of range\n");
        exit(1);
    }
    obj[0].val.li->elements[index] = Obj_cpy(obj[2]);
    return nil_Obj;
}


Object std_comp(Object* argv, int argc){
    if(argc == 1){
        if(argv[0].type == Obj_ount_t){
            return new_complex(argv[0].val.i,0);
        }
        if(argv[0].type == Obj_floap_t){
            return new_complex(argv[0].val.f,0);
        }
        if(argv[0].type == Obj_boolean_t){
            return new_complex(argv[0].val.b,0);
        }
    }
    if(argc == 2){
        for(int i = 0; i < argc; i ++){
            if(!(argv[i].type == Obj_ount_t || argv[i].type == Obj_floap_t || argv[i].type == Obj_boolean_t)){
                printf("ERROR comp only takes ount, floap or boolean when 2 args\n");
                exit(1);
            }
        }
        Object re = std_floap(&argv[0],1);
        Object im = std_floap(&argv[1],1);
        Object c = new_complex(re.val.f, im.val.f);
        Obj_free_val(re);
        Obj_free_val(im);
        return im;
    }
    if(argc == 1){
        if(argv[0].type == Obj_complex_t){
            return Obj_cpy(argv[0]);
        }
    }
    printf("ERROR on comp()\n");
    exit(1);
}


Object type(Object* obj, int n_arg) {
    if (n_arg!=1){
        printf("ERROR type only take one arg\n");
        exit(1);
    }
    switch (obj[0].type){
        case Obj_ount_t:
            return new_string("ount");
        case Obj_floap_t:
            return new_string("floap");
        case Obj_funcid_t:
            return new_string("funcid");
        case Obj_string_t:
            return new_string("string");
        case Obj_boolean_t:
            return new_string("boolean");
        case Obj_list_t:
            return new_string("list");
        case Obj_complex_t:
            return new_string("complex");
        case Obj_nil_t:
            return new_string("nil");
        default:
            printf("%d ",obj->type);
            return new_string("unknow");
    }
}


Object set_precision(Object* args, int argc){
    if(argc!=1){
        printf("ERROR set_precision only takes one arg\n");
        exit(1);
    }
    if(args[0].type != Obj_ount_t){
        printf("ERROR set_precision only takes ount as arg\n");
        exit(1);
    }
    precision=args[0].val.i;
    return nil_Obj;
}

Object get_precision(Object* args, int argc){
    if(argc!=0){
        printf("ERROR get_precision doesnt take any arg\n");
        exit(1);
    }
    return new_ount(precision);
}

Object get_methods(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR methods only takes one arg\n");
        exit(1);
    }
    if(argv[0].type == Obj_string_t){
        return new_string(string_methods);
    }
    if(argv[0].type == Obj_funcid_t){
        return new_string(funccall_methods);
    }
    if(argv[0].type == Obj_list_t){
        return new_string(list_methods);
    }
    return new_string("");
}

Object pop(Object* argv, int argc){
    if(argv[0].val.li->elements[0].val.i == 0){
        printf("ERROR cannot pop on empty list\n");
        exit(1);
    }
    int len = get_list_len(argv[0]);
    Obj_free_val(argv[0].val.li->elements[len]);
    argv[0].val.li->elements[0].val.i--;
    argv[0].val.li->elements = realloc(argv[0].val.li->elements, sizeof(Object) * len);
}

Object std_asc_val(Object* argv, int argc){
    return new_ount((long long int)argv[0].val.s[0]);
}

Object std_rand(Object* argv, int argc){
    return new_floap((long double)rand()/RAND_MAX);
}

Object std_print_memory(Object* argv, int argc){
    if (argc){
        printf("ERROR print_memory doesn't take args\n");
        exit(1);
    }
    printf("\nMEMORY:%d\n",MEMORY.len);
    for(int i = 0; i < MEMORY.len; i++){
        printf("    %s: ",MEMORY.keys[i]);
        println_prompt(&MEMORY.values[i],1);
    }
    return nil_Obj;
}


memory init_std(memory MEMORY,char*path){
    add_object(&MEMORY,"nil",nil_Obj);
    add_object(&MEMORY,"_",nil_Obj);
    add_object(&MEMORY,"__",nil_Obj);
    add_object(&MEMORY,"___",nil_Obj);
    add_func(&MEMORY,"print",&print_prompt,"");
    add_func(&MEMORY,"println",&println_prompt,"");
    add_func(&MEMORY,"bool",&std_bool,"");
    add_func(&MEMORY,"input",&read_prompt,"");
    add_func(&MEMORY,"ount",&std_ount,"");
    add_func(&MEMORY,"floap",&std_floap,"");
    add_func(&MEMORY,"list",&std_list,"");
    add_func(&MEMORY,"time",&current_timestamp,"");
    add_func(&MEMORY,"sleep",&sleep,"");
    /*depracated*/
    add_func(&MEMORY,"get",&get,"");
    /*depracated*/
    add_func(&MEMORY,"set",&set,"");
    add_func(&MEMORY,"type",&type,"");
    add_func(&MEMORY,"__set_precision__",&set_precision,"");
    add_func(&MEMORY,"__get_precision__",&get_precision,"");
    char*path0=abs_path();
    back_slash_to_path(path0);
    char *d= uti_dirname(path0);
    add_obj_str(&MEMORY,"__path__",path);
    add_obj_str(&MEMORY,"__interpreter_path__",path0);
    add_obj_str(&MEMORY,"__dir_path__",d);
    add_obj_str(&MEMORY,"__os__",__os__);
    free(d);
    free(path0);
    add_object(&MEMORY,"__base_precision__",new_ount(base_precision));
    add_func(&MEMORY,"var_exists",&var_exists,"");
    add_func(&MEMORY,"chr",&std_chr,"");
    add_func(&MEMORY,"asc_val",&std_asc_val,"");
    add_func(&MEMORY,"methods",&get_methods,"");
    add_func(&MEMORY, "pop",&pop,"");

    srand(current_timestamp(NULL,0).val.i);
    add_func(&MEMORY, "rand",&std_rand,"");

    add_func(&MEMORY, "__print_memory__", &std_print_memory, "");
    add_func(&MEMORY, "__set_flush__", &std_set_flush, "");
    add_obj_str(&MEMORY,"__version__",VERSION);
    add_obj_str(&MEMORY,"__sub_version__",SUB_VERSION);
    add_obj_str(&MEMORY,"__complete_version__",COMPLETE_VERSION);

    return MEMORY;
}