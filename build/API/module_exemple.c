#include "memlib.h"


Object add_nums(Object* argv, int argc){
    //only take  2 arguments
    if(argc != 2){
        printf("ERROR in malloc");
        exit(1);
    }
    //only take args of type ount
    if(argv[0].type != Obj_ount_t || argv[1].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    int res = *argv[0].val.i + *argv[1].val.i;
    return new_ount(res);
}


Object __loader() {
    Object mod = new_Module();

    add_func_Module(mod, "add_nums", &add_nums, "");
    add_Object_Module(mod, "a_var", new_string("hi !"));

    return mod;
}