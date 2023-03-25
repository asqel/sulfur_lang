#include "std.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"

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
        printf("%Lf",*obj->val.f);
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



memory init_std(memory MEMORY){
    add_object(&MEMORY,"nil",nil_Obj);
    add_func(&MEMORY,"print",&print_prompt,"");
    add_func(&MEMORY,"println",&println_prompt,"");
    add_func(&MEMORY,"bool",&std_bool,"");
    add_func(&MEMORY,"input",&read_prompt,"");
    add_func(&MEMORY,"ount",&std_ount,"");
    add_func(&MEMORY,"floap",&std_floap,"");
    add_func(&MEMORY,"list",&std_list,"");
    add_func(&MEMORY,"time",&current_timestamp,"");
    char*path0=abs_path();
    back_slash_to_path(path0);
    char *d=dirname(path0);
    char *filepath=str_cat_new(d,"/main.su");
    add_obj_str(&MEMORY,"__path__",filepath);
    add_obj_str(&MEMORY,"__interpreter_path__",path0);
    add_obj_str(&MEMORY,"__dir_path__",d);
    free(d);
    free(path0);

        
    
    return MEMORY;
}

