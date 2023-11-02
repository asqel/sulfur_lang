#include "../../include/bytecode_maker/make_constants.h"
#include "../../include/instruction.h"
#include "../../include/memlib.h"

Object *new_constants(S_sulfur_int *len){
    Object *res = malloc(sizeof(Object) * 3);
    *len = 3;
    res[0] = new_complex(0, 1);
    res[1] = new_boolean(0);
    res[2] = new_boolean(1);
    return res;
}

void init_constants() {
    constants = new_constants(&constants_len);
}

int add_constants(Object x){
    if (x.type != Obj_string_t && x.type != Obj_ount_t && x.type != Obj_floap_t) {
        printf("ERROR cannot add constant of type %s\n", Obj_type_as_str(x.type));
        printf("only string ount or floap ar allowed\n");
        exit(1);
    }
    for(int i = 0; i < constants_len; i++) {
        if (x.type == constants[i].type) {
            if (x.type == Obj_string_t)
                if (!strcmp(x.val.s, constants[i].val.s))
                    return i;
            if (x.type == Obj_ount_t)
                if (x.val.i == constants[i].val.i)
                    return i;
            if (x.type == Obj_floap_t)
                if (x.val.f == constants[i].val.f)
                    return i;
        }
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = Obj_cpy(x);
    return constants_len - 1;
}

int add_constant_str(char *s){
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type != Obj_string_t)
            continue;
        if (!strcmp(s, constants[i].val.s))
            return i;
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = new_string(s);
    return constants_len - 1;
}

int add_constant_int(S_ount_t value){
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type != Obj_ount_t)
            continue;
        if (value == constants[i].val.i)
            return i;
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = new_ount(value);
    return constants_len - 1;
}

int add_constant_float(S_floap_t value){
    for(int i = 0; i < constants_len; i++) {
        if (constants[i].type != Obj_floap_t)
            continue;
        if (value == constants[i].val.f)
            return i;
    }
    constants_len++;
    constants = realloc(constants, sizeof(Object) * constants_len);
    constants[constants_len - 1] = new_floap(value);
    return constants_len - 1;
}