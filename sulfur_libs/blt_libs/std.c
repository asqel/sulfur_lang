#include "std.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"

#ifdef _WIN32
    char* __os__="windows";
#elif __APPLE__
    char* __os__="APPLE";
#elif __linux__
    char* __os__="linux";
#else 
    char* __os__="UNKNOWN";
#endif

int precision=5;
int base_precision=6;

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
        return print_prompt(&obj[n_arg-1],1);

    }
    if(obj->type==Obj_string_t){
        printf("%s",obj->val.s);
        return nil_Obj;
    }
    if(obj->type==Obj_boolean_t){
        printf(*obj->val.b==0?"0b":"1b");
        return nil_Obj;
    }
    if(obj->type==Obj_class_t){
        if(!str_ar_contains_str(obj->val.cl->keys,"__string__",obj->val.cl->key_len)){
            printf("instance at :%x",obj);
            return nil_Obj;
        }
        
    }
    if(obj->type==Obj_complex_t){
        printf("%Lf + %Lfi",obj->val.c[0],obj->val.c[1]);
        return nil_Obj;
    }
    if(obj->type==Obj_floap_t){
        printf("%.*Lf",precision,*obj->val.f);
        return nil_Obj;
    }
    if(obj->type==Obj_list_t){
        int len=*(obj->val.li->elements[0].val.i);
        if(len==0){
            printf("[]");
            return nil_Obj;
        }
        printf("[");
        for(int i=1;i<len;i++){
            print_prompt(&obj->val.li->elements[i],1);
            printf(",");
        }
        print_prompt(&obj->val.li->elements[len],1);
        printf("]");
        return nil_Obj;

    }
    if(obj->type==Obj_funcid_t){
        printf("function at :%x",obj->val.funcid);
        return nil_Obj;
    }
    if(obj->type==Obj_nil_t){
        printf("nil");
        return nil_Obj;
    }
    if(obj->type==Obj_Obj_t){
        //print_prompt(obj->val.o,);
        return nil_Obj;
    }
    if(obj->type==Obj_ount_t){
        printf("%lld",*obj->val.i);
        return nil_Obj;
    }
    if(obj->type==Obj_typeid_t){
        printf("%s",obj->val.typeid);
        return nil_Obj;
    }
    if(obj->type == obj_module_t){
        printf("{\n");
        for(int i=0;i<obj->val.module->MEM->len;i++){
            printf("%s : ",obj->val.module->MEM->keys[i]);
            println_prompt(&obj->val.module->MEM->values[i],1);
        }
        printf("\n}");
    }
    return nil_Obj;

}

Object println_prompt(Object*obj,int n_arg){
    Object n=print_prompt(obj,n_arg);
    printf("\n");
    return n;
}


Object std_bool(Object*obj,int n_arg){
    if(n_arg!=1){
        printf("ERROR %s only 1 argument needed in bool call",n_arg>1?"too many arguments":"too few arguments");
        exit(1);
    }
    Object res;
    res.type=Obj_boolean_t;
    res.val.b=malloc(sizeof(short int));
    *res.val.b=1;
    if(obj->type==Obj_ount_t){
        if(*obj->val.i==0){
            *res.val.b=0;
        }
        return res;
    }
    if(obj->type==Obj_string_t){
        if(obj->val.s[0]==0){
            *res.val.b=0;
        }
        return res;
    }
    if(obj->type==Obj_boolean_t){
        *res.val.b=*obj->val.b;
        return res;
    }
    return nil_Obj;
}

Object std_ount(Object*obj,int n_arg){
    if(obj->type==Obj_ount_t){
        return *obj;
    }
    Object res;
    res.type=Obj_ount_t;
    res.val.i=malloc(sizeof(long long int));

    if(obj->type==Obj_floap_t){
        *res.val.i=(int)*(obj->val.f);
        return res;
    }

    if(obj->type==Obj_boolean_t){
        *res.val.i=(int)*(obj->val.b);
        return res;
    }
    if(obj->type==Obj_string_t){
        *res.val.i=str_to_llint(obj->val.s);
        return res;
    }
    return nil_Obj;
}
Object std_floap(Object*obj,int n_arg){
    if(obj->type==Obj_floap_t){
        return *obj;
    }
    Object res;
    res.type=Obj_floap_t;
    res.val.f=malloc(sizeof(long long int));

    if(obj->type==Obj_ount_t){
        *res.val.f=(long double)*(obj->val.i);
        return res;
    }

    if(obj->type==Obj_boolean_t){
        *res.val.f=(long double)*(obj->val.b);
        return res;
    }
    if(obj->type==Obj_string_t){
        *res.val.f=atof(obj->val.s);
        return res;
    }
    return nil_Obj;

}

Object std_list(Object*obj,int n_arg){
    if(n_arg==0){
        Object x;
        x.type=Obj_list_t;
        x.val.li=malloc(sizeof(list));
        x.val.li->elements=malloc(sizeof(Object));
        x.val.li->elements[0].type=Obj_ount_t;
        x.val.li->elements[0].val.i=malloc(sizeof(long long int));
        *x.val.li->elements[0].val.i=0;
        return x;
    }
    Object x;
    x.type=Obj_list_t;
    x.val.li=malloc(sizeof(list));
    x.val.li->elements=malloc(sizeof(Object)*(n_arg+1));
    x.val.li->elements[0].type=Obj_ount_t;
    x.val.li->elements[0].val.i=malloc(sizeof(long long int));
    *x.val.li->elements[0].val.i=n_arg;
    for(int i=1;i<=n_arg;i++){
        x.val.li->elements[i]=obj[i-1];
    }

    return x;
}


Object current_timestamp(Object *obj,int n_arg) {
    struct timespec te;
    clock_gettime(CLOCK_REALTIME, &te);
    long long milliseconds = te.tv_sec*1000LL + te.tv_nsec/1000000LL;
    Object o;
    o.type=Obj_ount_t;
    o.val.i=malloc(sizeof(long long int));
    *o.val.i=milliseconds;
    return o;
}
Object sleep(Object *obj,int n_arg){
    long long int x=0;
    Object o=std_ount(&obj[0],1);
    if(o.type!=Obj_ount_t){
        return nil_Obj;
    }
    x=*o.val.i;
    long long start=*current_timestamp(NULL,0).val.i;
    while(*current_timestamp(NULL,0).val.i<start+x){}
    return nil_Obj;
}

Object get(Object *obj,int n_arg){
    if (n_arg!=2){
        printf("ERROR get only takes 2 arguments");
        exit(1);
    }
    if(obj[0].type != Obj_string_t && obj[0].type != Obj_list_t){
        printf("ERROR get only take list or string as first argument");
        exit(1);
    }
    Object index=std_ount(&(obj[1]),1);
    if (index.type == Obj_nil_t){
        printf("ERROR get only take a ount-convetible as second argument");
        exit(1);
    }
    int len=0;
    if (obj[0].type == Obj_string_t){
        len=strlen(obj[0].val.s);
        if (*index.val.i==-1){
            Object res;
            res.type=Obj_ount_t;
            res.val.s=malloc(sizeof(long long int));
            *res.val.s=len;
            return res;
        }
        if (*index.val.i >= len || *index.val.i<-1){
            printf("ERROR get out of range");
            exit(1);
        }
        Object res;
        res.type=Obj_string_t;
        res.val.s=malloc(sizeof(char)*2);
        res.val.s[0]=obj[0].val.s[*index.val.i];
        res.val.s[1]='\0';
        return res;
    }
    len=*(obj[0].val.li->elements[0].val.i);
    if (*index.val.i >= len || *index.val.i<-1){
        printf("ERROR get out of range");
        exit(1);
    }
    return obj[0].val.li->elements[*index.val.i+1];

}

Object set(Object *obj,int n_arg){
    if(n_arg != 3){
        printf("ERROR set only takes 3 args");
        exit(1);
    }
    if(obj[0].type != Obj_list_t && obj[0].type != Obj_string_t){
        printf("ERROR set only takes string or list as first arg");
        exit(1);
    }
    if(obj[1].type != Obj_ount_t && obj[1].type != Obj_floap_t){
        printf("ERROR set only take ount or floap as second arg");
        exit(1);
    }
    int index;
    if(obj[1].type == Obj_ount_t){
        index =1+ * obj[1].val.i;
    }
    if(obj[1].type == Obj_floap_t){
        index = 1+(int)*obj[1].val.f;
    }
    if (index > *obj[0].val.li->elements[0].val.i || index<1){
        printf("ERROR set out of range");
        exit(1);
    }
    obj[0].val.li->elements[index]=Obj_cpy(obj[2]);
    return nil_Obj;

}

Object append(Object*obj,int n_arg){
    int len =*(obj[0].val.li->elements[0].val.i);
    obj[0].val.li->elements=realloc(obj->val.li->elements,(len+n_arg)*sizeof(Object));
    for(int i=1;i<n_arg;i++){
        if(obj[i].type==Obj_ount_t){
            obj[0].val.li->elements[len+i].type=Obj_ount_t;
            obj[0].val.li->elements[len+i].val.i=malloc(sizeof(long long int));
            *(obj[0].val.li->elements[len+i].val.i)=*obj[i].val.i;
        }
        else if(obj[i].type==Obj_floap_t){
            obj[0].val.li->elements[len+i].type=Obj_floap_t;
            obj[0].val.li->elements[len+i].val.f=malloc(sizeof(long double));
            *(obj[0].val.li->elements[len+i].val.f)=*obj[i].val.f;
        }
        else if(obj[i].type==Obj_boolean_t){
            obj[0].val.li->elements[len+i].type=Obj_boolean_t;
            obj[0].val.li->elements[len+i].val.b=malloc(sizeof(short int));
            *(obj[0].val.li->elements[len+i].val.b)=*obj[i].val.b;
        }
        else{
            obj[0].val.li->elements[len+i]=obj[i];
        }
    }
    *(obj[0].val.li->elements[0].val.i)+=n_arg-1;
    return obj[0];
}

Object type(Object* obj, int n_arg) {
    if (n_arg!=1){
        printf("ERROR type only take one arg");
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
        default:
            return new_string("unknow");
    }
}


Object set_precision(Object* args, int argc){
    if(argc!=1){
        printf("ERROR set_precision only takes one arg");
        exit(1);
    }
    if(args[0].type != Obj_ount_t){
        printf("ERROR set_precision only takes ount as arg");
        exit(1);
    }
    precision=*args[0].val.i;
    return nil_Obj;
}

Object get_precision(Object* args, int argc){
    if(argc!=0){
        printf("ERROR get_precision doesnt take any arg");
        exit(1);
    }
    return new_ount(precision);
}

memory init_std(memory MEMORY,char*path){
    add_object(&MEMORY,"nil",nil_Obj);
    add_func(&MEMORY,"print",&print_prompt,"");
    add_func(&MEMORY,"println",&println_prompt,"");
    add_func(&MEMORY,"bool",&std_bool,"");
    add_func(&MEMORY,"input",&read_prompt,"");
    add_func(&MEMORY,"ount",&std_ount,"");
    add_func(&MEMORY,"floap",&std_floap,"");
    add_func(&MEMORY,"list",&std_list,"");
    add_func(&MEMORY,"time",&current_timestamp,"");
    add_func(&MEMORY,"sleep",&sleep,"");
    add_func(&MEMORY,"get",&get,"");
    add_func(&MEMORY,"set",&set,"");
    add_func(&MEMORY,"append",&append,"");
    add_func(&MEMORY,"type",&type,"");
    add_func(&MEMORY,"set_precision",&set_precision,"");
    add_func(&MEMORY,"get_precision",&get_precision,"");
    char*path0=abs_path();
    back_slash_to_path(path0);
    char *d=dirname(path0);
    add_obj_str(&MEMORY,"__path__",path);
    add_obj_str(&MEMORY,"__interpreter_path__",path0);
    add_obj_str(&MEMORY,"__dir_path__",d);
    add_obj_str(&MEMORY,"__os__",__os__);
    free(d);
    free(path0);
    add_object(&MEMORY,"__base_precision__",new_ount(base_precision));


        
    
    return MEMORY;
}