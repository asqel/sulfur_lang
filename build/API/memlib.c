#include "memlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

Object end_Obj ={Obj_end_t,{.b=&(short int){-1}}};
Object nil_Obj ={Obj_nil_t,{.b=&(short int){0}}};

//return 1 if they are the same
//else 0
//they are the same if they have same type and same value pointed by the .value
int Obj_cmp(Object obj1,Object obj2){
    if(obj1.type!=obj2.type){
        return 0;
    }
    else{
        switch(obj1.type){
            case Obj_Obj_t:
                return Obj_cmp(*obj1.val.o,*obj2.val.o);
            case Obj_string_t:
                return !strcmp(obj1.val.s,obj2.val.s);
            case Obj_ount_t:
                return *obj1.val.i==*obj2.val.i;
            case Obj_floap_t:
                return *obj1.val.f==*obj2.val.f;
            case Obj_complex_t:
                return *obj1.val.c==*obj2.val.c&&obj1.val.c[1]==obj2.val.c[1];
            case Obj_boolean_t:
                *obj1.val.b==*obj2.val.b;
            default:
                //ERROR
                break;
        }
    }
    return 0;
}

int Obj_len(Object*obj){
    int x=0;
    for(int i=0;!Obj_cmp(obj[i],end_Obj);i++){
        x++;
    }
    return x;
}

void Obj_free_val(Object obj){
    int len;
    switch (obj.type){
        case Obj_Obj_t:
            len=Obj_len(obj.val.o);
            for(int i=0;i<len;i++){
                Obj_free_val(obj.val.o[i]);
            }
            break;
        case Obj_string_t:
            free(obj.val.s);
            break;
        case Obj_ount_t:
            free(obj.val.o);
            break;
        case Obj_floap_t:
            free(obj.val.f);
            break;
        case Obj_complex_t:
            free(obj.val.c);
            break;
        case Obj_boolean_t:
            free(obj.val.b);
            break;
        case Obj_nil_t:
            free(obj.val.i);
            break;
        case Obj_end_t:
            free(obj.val.i);
            break;
        case Obj_list_t:
            free(obj.val.o);   
            break;
        case Obj_funcid_t:
            free(obj.val.funcid);
            break;
        case Obj_typeid_t:
            free(obj.val.typeid);
            break;
        case Obj_class_t:
            for(int i=0;i<obj.val.cl->key_len;i++){
                free(obj.val.cl->keys[i]);
            }  
            free(obj.val.cl->keys);
            free(obj.val.cl->class_name);
            for(int i=0;i<obj.val.cl->key_len;i++){
                Obj_free_val(obj.val.cl->values[i]);
            }
            free(obj.val.cl->values);
            free(obj.val.cl);
            break;

        default:
            break;
    }
}

void Objs_print(Object*obj,int len){
    for(int i=0;i<len;i++){
        Obj_print(obj[i]);
    }

}

void Obj_print(Object obj){
    switch (obj.type){
        case Obj_boolean_t:
            printf("Obj_b:[%s]",*obj.val.b!=0?"1b":"0b");
            break;
        case Obj_complex_t:
            printf("Obj_c:[%Lf+%Lfi]",obj.val.c[0],obj.val.c[1]);
            break;
        case Obj_floap_t:
            printf("Obj_f:[%Lf]",*obj.val.f);
            break;
        case Obj_ount_t:
            printf("Obj_i:[%lld]",*obj.val.i);
    
        default:
            break;
    }
}

void*get_obj_pointer(Object o){
    switch(o.type){
        case Obj_boolean_t:
            return o.val.b;
        case Obj_list_t:
            return o.val.li;
        case Obj_complex_t:
            return o.val.c;
        case Obj_floap_t:
            return o.val.f;
        case Obj_funcid_t:
            return o.val.funcid;
        case Obj_ount_t:
            return o.val.i;
        case Obj_string_t:
            return o.val.s;
        case Obj_typeid_t:
            return o.val.typeid;
    }
}

Funcdef new_blt_func(Object (*func)(Object*,int),char*desc){
    Funcdef f;
    f.arg_names=NULL;
    f.arg_types=NULL;
    f.code=NULL;
    f.description=desc;
    f.func_p=func;
    f.is_builtin=1;
    f.nbr_of_args=-1;
    f.nbr_ret_type=-1;
    f.ret_type=NULL;
    return f;
}

memory*add_func(memory*MEMORY,char*name,Object (*func)(Object*,int),char*desc){
    //add print
    MEMORY->len+=1;
    MEMORY->keys=realloc_c(MEMORY->keys,sizeof(char*)*(MEMORY->len-1),sizeof(char*)*MEMORY->len);

    MEMORY->keys[MEMORY->len-1]=malloc(sizeof(char)*(1+strlen(name)));
    strcpy(MEMORY->keys[MEMORY->len-1],name);  

    MEMORY->values=realloc_c(MEMORY->values,sizeof(Object)*(MEMORY->len-1),sizeof(Object)*MEMORY->len);
    MEMORY->values[MEMORY->len-1].type=Obj_funcid_t;
    MEMORY->values[MEMORY->len-1].val.funcid=malloc(sizeof(Funcdef));
    *MEMORY->values[MEMORY->len-1].val.funcid=new_blt_func(func,desc);
    return MEMORY;
}

memory*add_object(memory*MEMORY,char*name,Object x){
    MEMORY->len++;
    MEMORY->keys=realloc_c(MEMORY->keys,sizeof(char*)*(MEMORY->len-1),sizeof(char*)*MEMORY->len);

    MEMORY->keys[MEMORY->len-1]=malloc(sizeof(char)*(1+strlen(name)));
    strcpy(MEMORY->keys[MEMORY->len-1],name);  

    MEMORY->values=realloc_c(MEMORY->values,sizeof(Object)*(MEMORY->len-1),sizeof(Object)*MEMORY->len);
    MEMORY->values[MEMORY->len-1]=x;
    return MEMORY;

}

memory*add_obj_str(memory*MEMORY,char*name,char*val){
    Object x;
    x.type=Obj_string_t;
    x.val.s=malloc(sizeof(char)*(1+strlen(val)));
    strcpy(x.val.s,val);
    return add_object(MEMORY,name,x);

}


Object new_Module(){
    Module m;
    m.MEM=malloc(sizeof(memory));
    m.MEM->keys=malloc(sizeof(char*));
    m.MEM->len=0;
    m.MEM->values=malloc(sizeof(Object));

    Object o;
    o.type=obj_module_t;
    o.val.module=malloc(sizeof(Module));
    *o.val.module=m;
    return o;
}


void add_func_Module(Object mod, char *name, Object (*func)(Object *, int), char *desc){
    add_func(mod.val.module->MEM,name,func,desc);

}

void add_Object_Module(Object mod, char*name,Object x){
    add_object(mod.val.module->MEM,name,x);
}


Object new_ount(long long int value){
    Object o;
    o.type=Obj_ount_t;
    o.val.i=malloc(sizeof(long long int));
    *o.val.i=value;
    return o;
}

Object new_floap(long double value){
    Object o;
    o.type=Obj_floap_t;
    o.val.f=malloc(sizeof(long double));
    *o.val.f=value;
    return o;
}

//the string will be copied *
Object new_string(char * value){
    Object o;
    o.type=Obj_string_t;
    o.val.s=malloc(sizeof(char)*(1+strlen(value)));
    strcpy(o.val.s,value);
    return o;
}


Object new_boolean(int value){
    Object o;
    o.type=Obj_boolean_t;
    o.val.b=malloc(sizeof(short int));
    *o.val.b= value ? 1:0;
    return o;
}