#include "../include/make_constants.h"
#include "../include/instruction.h"
#include "../include/memlib.h"

Object *constants = NULL;
int constants_len = 0;

void init_constants(){
    constants = malloc(sizeof(Object) * 3);
    constants_len = 3;
    constants[0] = new_complex(0, 1);
    constants[1] = new_boolean(0);
    constants[2] = new_boolean(1);
}

int add_constants(Object x){
    for(int i = 0; i < constants_len; i++) {
        if (x.type == constants[i].type) {
            if (x.type == Obj_string_t) {
                if (!strcmp(x.val.s, constants[i].val.s))
                    return i;
            }
            if (x.type == Obj_ount_t) {
                if (x.val.i == constants[i].val.i)
                    return i;
            }
            if (x.type == Obj_floap_t) {
                if (x.val.f == constants[i].val.f)
                    return i;
                
            }
        }
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = Obj_cpy(x);
    return constants_len - 1;
}

int add_constant_string(char *s){
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type != Obj_string_t)
            continue;
        if (!strcmp(s, constants[i].val.s))
            return i;
        constants_len++;
        constants = realloc(constants, sizeof(Object) * constants_len);
        constants[constants_len - 1] = new_string(s);
        return constants_len - 1;
            
    }
    return -1;
}


void make_constants_in_ast(Ast *x){

}
