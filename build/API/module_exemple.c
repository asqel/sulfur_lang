#include "memlib.h"

Sulfur_ctx context;

Object add_nums(Object* argv, int argc){
    //only take  2 arguments
    if(argc != 2){
        PRINT_ERR("ERROR in add_nums\n");
        exit(1);
    }
    //only take args of type ount
    if(argv[0].type != Obj_ount_t || argv[1].type != Obj_ount_t){
        PRINT_ERR("ERROR in add_nums\n");
        exit(1);
    }
    int res = argv[0].val.i + argv[1].val.i;
    return new_ount(res);
}

Object sub_nums(Object *argv, int argc) {
    //only take  2 arguments
    if(argc != 2){
        PRINT_ERR("ERROR in sub_nums\n");
        exit(1);
    }
    //only take args of type ount
    if(argv[0].type != Obj_ount_t || argv[1].type != Obj_ount_t){
        PRINT_ERR("ERROR in sub_nums\n");
        exit(1);
    }
    return OP_SUB(argv[0], argv[1]); // you can also use the operations

}

Object __loader(Sulfur_ctx ctx) {
    context = ctx;

    Object mod = new_Module();


    add_func_Module(mod, "add_nums", &add_nums, "you can get this message with .help()");
    add_func_Module(mod, "sub_nums", &sub_nums, "this function substract 2 numbers");
    add_Object_Module(mod, "a_var", new_string("hi !"));

    return mod;
}