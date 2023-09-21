#include "../include/memlib.h"
#include "../include/utilities.h"
#include "../sulfur_libs/blt_libs/std.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



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
                return obj1.val.i == obj2.val.i;
            case Obj_floap_t:
                return obj1.val.f == obj2.val.f;
            case Obj_complex_t:
                return *obj1.val.c == *obj2.val.c && obj1.val.c[1] == obj2.val.c[1];
            case Obj_boolean_t:
                obj1.val.b == obj2.val.b;
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


//return 1 if obj was a list else 0
int Obj_free_val(Object obj){
    switch (obj.type){
        case Obj_string_t:
            free(obj.val.s);
            break;
        case Obj_nil_t:
            break;
        case Obj_end_t:
            break;
        case Obj_list_t:
            remove_count(obj.val.li, Obj_list_t);
            return 1;
        case Obj_funcid_t:
            remove_count(obj.val.funcid, Obj_funcid_t);
            break;
        case Obj_typeid_t:
            free(obj.val.typeid);
            break;
        case Obj_class_t:
            break;
        default:
            break;
    }
    return 0;
}

void Objs_print(Object*obj,int len){
    for(int i=0;i<len;i++){
        Obj_print(obj[i]);
    }

}

void Obj_print(Object obj){
    switch (obj.type){
        case Obj_boolean_t:
            printf("Obj_b:[%s]", obj.val.b!=0?"1b":"0b");
            break;
        case Obj_complex_t:
            printf("Obj_c:[%Lf+%Lfi]",obj.val.c[0],obj.val.c[1]);
            break;
        case Obj_floap_t:
            printf("Obj_f:[%Lf]", obj.val.f);
            break;
        case Obj_ount_t:
            printf("Obj_i:[%lld]", obj.val.i);
            break;
        case Obj_funcid_t:
            printf("Obj_func[%p]",obj.val.funcid);
            break;
        default:
            break;
    }
}

void*get_obj_pointer(Object o){
    switch(o.type){
        case Obj_boolean_t:
            return NULL;
        case Obj_list_t:
            return o.val.li;
        case Obj_complex_t:
            return NULL;
        case Obj_floap_t:
            return NULL;
        case Obj_funcid_t:
            return o.val.funcid;
        case Obj_ount_t:
            return NULL;
        case Obj_string_t:
            return o.val.s;
        case Obj_typeid_t:
            return o.val.typeid;
    }
}

Funcdef new_blt_func(Object (*func)(Object*, int), char *desc){
    Funcdef f;
    f.arg_names=NULL;
    f.code=NULL;
    f.description=desc;
    f.func_p=func;
    f.is_builtin=1;
    f.nbr_of_args=-1;
    return f;
}

memory*add_func(memory *MEMORY, char *name,Object (*func)(Object*, int),char *desc){
    MEMORY->len += 1;
    MEMORY->keys = realloc_c(MEMORY->keys, sizeof(char*) * (MEMORY->len - 1), sizeof(char*) * MEMORY->len);

    MEMORY->keys[MEMORY->len-1] = malloc(sizeof(char) * (1 + strlen(name)));
    strcpy(MEMORY->keys[MEMORY->len - 1], name);  

    MEMORY->values = realloc_c(MEMORY->values,sizeof(Object) * (MEMORY->len - 1),sizeof(Object) * MEMORY->len);
    MEMORY->values[MEMORY->len - 1].type = Obj_funcid_t;
    MEMORY->values[MEMORY->len - 1].val.funcid = malloc(sizeof(Funcdef));
    *MEMORY->values[MEMORY->len - 1].val.funcid = new_blt_func(func,desc);
    add_count(MEMORY->values[MEMORY->len - 1].val.funcid, Obj_funcid_t);
    return MEMORY;
}

memory*add_object(memory *MEMORY, char *name, Object x){
    MEMORY->len++;
    MEMORY->keys = realloc(MEMORY->keys, sizeof(char*) * (MEMORY->len));

    MEMORY->keys[MEMORY->len-1] = malloc(sizeof(char)*(1+strlen(name)));
    strcpy(MEMORY->keys[MEMORY->len-1], name);  

    MEMORY->values = realloc(MEMORY->values, sizeof(Object) * MEMORY->len);
    MEMORY->values[MEMORY->len-1] = x;
    return MEMORY;

}

memory*add_obj_str(memory*MEMORY,char*name,char*val){
    Object x;
    x.type = Obj_string_t;
    x.val.s = malloc(sizeof(char) * (1 + strlen(val)));
    strcpy(x.val.s, val);
    return add_object(MEMORY, name, x);

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
    o.type = Obj_ount_t;
    o.val.i = value;
    return o;
}

Object new_floap(long double value){
    Object o;
    o.type = Obj_floap_t;
    o.val.f = value;
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
    o.val.b = value ? 1:0;
    return o;
}

Object Obj_cpy(Object o){
    Object res;
    switch (o.type){
        case Obj_ount_t:
            return new_ount(o.val.i);
        case Obj_floap_t:
            return new_floap(o.val.f);
        case Obj_string_t:
            return new_string(o.val.s);
        case Obj_boolean_t:
            return new_boolean(o.val.b);
        case Obj_list_t:
            res.type = Obj_list_t;
            res.val.li = o.val.li;
            add_count(o.val.li,Obj_list_t);
            return res;
        case Obj_funcid_t:
            res.type = Obj_funcid_t;
            res.val.funcid = o.val.funcid;
            add_count(o.val.funcid, Obj_funcid_t);
            return res;
        case obj_module_t:
            res.type=obj_module_t;
            res.val.module = o.val.module;
            return res;
        case Obj_complex_t:
            return new_complex(o.val.c[0], o.val.c[1]);
        default:
            return nil_Obj;
    }
}

Object new_complex(long double re, long double im){
    Object o;
    o.type = Obj_complex_t;
    o.val.c[0] = re;
    o.val.c[1] = im;
    sizeof(Object);
    return o;
}

int is_muttable(Object o){
    switch (o.type){
        case Obj_list_t:
        case Obj_funcid_t:
        case obj_module_t:
            return 1;

        default:
            return 0;
    }
}

//return the object with the corresponding name in the memory MEM
//if it isn't in the memory an Object of type Obj_not_found_t is returned
Object get_Obj_mem(memory MEMORY, char* name){
    for(int i = 0; i < MEMORY.len; i++){
        if(!strcmp(MEMORY.keys[i], name)){
            return MEMORY.values[i];
        }
    }
    return not_found_Obj;
}

void Obj_free_array(Object* objs, int len){
    for(int i = 0; i < len; i++){
        Obj_free_val(objs[i]);
    }
    free(objs);
}


int get_list_len(Object l){
    if(l.type == Obj_list_t){
        return l.val.li->elements[0].val.i;
    }
    return 0;
}



extern ref_count* REFS;
extern int REFS_len;

void add_count(void* address, int type){
    for(int i = 0; i < REFS_len; i++){
        if(REFS[i].address == address){
            REFS[i].count += 1;
            return ;
        }
    }
    REFS_len++;
    REFS = realloc(REFS, sizeof(ref_count) * REFS_len);
    REFS[REFS_len-1].address = address;
    REFS[REFS_len-1].count = 1;
    REFS[REFS_len-1].type = type;
}

void remove_count(void* address, int type){
    for(int i = 0; i < REFS_len; i++){
        if(REFS[i].address == address){
            REFS[i].count -= 1;
            if(REFS[i].count == 0){
                if(type == Obj_list_t){
                    Obj_free_array(((list*)address)->elements, ((list*)address)->elements[0].val.i);
                    free(address);
                }
                if(type == Obj_funcid_t){
                    Funcdef* f = ((Funcdef*)address);
                    if(f->is_builtin){
                        //free(f->description);
                        free(f);
                    }
                }
                for(int k = i + 1; k < REFS_len; k++){
                    REFS[k - 1] = REFS[k];
                }
                REFS_len--;
                REFS = realloc(REFS, sizeof(ref_count) * REFS_len);
            }
            return ;
        }
    }
}