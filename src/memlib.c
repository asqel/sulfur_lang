#include "../include/memlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
            break;
        case Obj_complex_t:
            return o.val.c;
            break;
        case Obj_floap_t:
            return o.val.f;
            break;
        case Obj_funcid_t:
            return o.val.funcid;
            break;
        case Obj_ount_t:
            return o.val.i;
            break;
        case Obj_string_t:
            return o.val.s;
            break;
        case Obj_typeid_t:
            return o.val.typeid;
            break;
    }
}