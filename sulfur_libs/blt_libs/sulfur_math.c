#include "sulfur_math.h"
#include "../../build/API/memlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

Object is_even(Object *argv,int argc){
    if(argc != 1){
        printf("ERROR iseven only take 1 one argument");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERROR iseven only take ount argument");
        exit(1);
    }
    return new_boolean((*argv[0].val.i)&1);
}

Object _cos(Object *argv, int argc){
    if(argc != 1){
        printf("ERROR cos only take 1 one argument");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERROR cos only take ount argument");
        exit(1);
    }
    return new_floap(cos(*argv[0].val.i));
}

Object _sin(Object *argv, int argc){
    if(argc != 1){
        printf("ERROR sin only take 1 one argument");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERROR sin only take ount argument");
        exit(1);
    }
    return new_floap(sin(*argv[0].val.i));
}


Object __loader(){
    Object mod=new_Module();
    add_func_Module(mod,"is_even",&is_even,"");
    add_func_Module(mod,"cos",&_cos,"");
    add_func_Module(mod,"sin",&_sin,"");
    add_Object_Module(mod,"PI",new_floap(M_PI));
    return mod;
}