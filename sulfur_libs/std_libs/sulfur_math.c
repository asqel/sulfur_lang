#include "../../build/API/memlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Object is_even(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR is_even only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t) {
        printf("ERROR is_even only take ount argument");
        exit(1);
    }
    return new_boolean((*argv[0].val.i) & 1);
}

Object is_odd(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR is_even only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t) {
        printf("ERROR is_even only take ount argument");
        exit(1);
    }
    return new_boolean(! ((*argv[0].val.i) & 1));
}

Object _cos(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR cos only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR cos only take ount or floap argument");
        exit(1);
    }
    return new_floap(cos(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}

Object _sin(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR sin only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR sin only take ount or floap argument");
        exit(1);
    }
    return new_floap(sin(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}

Object _tan(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR sin only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR tan only take ounti or floap argument");
        exit(1);
    }
    return new_floap(tan(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}

Object _ln(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR ln only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR ln only take ount or floap argument");
        exit(1);
    }
    return new_floap(log(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}
Object _log(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR log only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR log only take ount or floap argument");
        exit(1);
    }
    return new_floap(log10(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}

Object _sqrt(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR sqrt only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR sqqrt only take ount or floap argument");
        exit(1);
    }
    return new_floap(sqrt(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}

Object _ceil(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR ceil only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR ceil only take ount or floap argument");
        exit(1);
    }
    return new_floap(ceil(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}

Object _floor(Object* argv, int argc) {
    if (argc != 1) {
        printf("ERROR floor only take 1 one argument");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        printf("ERROR floor only take ount or floap argument");
        exit(1);
    }
    return new_floap(floor(argv[0].type==Obj_ount_t ? *argv[0].val.i : *argv[0].val.f));
}


#ifndef ONE_FILE
Object __loader() {
    Object mod = new_Module();

    add_func_Module(mod, "is_even", &is_even, "");
    add_func_Module(mod, "is_odd", &is_odd, "");
    add_func_Module(mod, "cos", &_cos, "");
    add_func_Module(mod, "sin", &_sin, "");
    add_func_Module(mod, "tan", &_tan, "");
    add_func_Module(mod, "ln", &_ln, "");
    add_func_Module(mod, "log", &_log, "");
    add_func_Module(mod, "sqrt", &_sqrt, "");
    add_func_Module(mod, "ceil", &_ceil, "");
    add_func_Module(mod, "floor", &_floor, "");

    
    add_Object_Module(mod, "pi", new_floap(M_PI));
    add_Object_Module(mod, "e", new_floap(M_E));

    return mod;
}
#else
Object __load_math() {
    Object mod = new_Module();

    add_func_Module(mod, "is_even", &is_even, "");
    add_func_Module(mod, "is_odd", &is_odd, "");
    add_func_Module(mod, "cos", &_cos, "");
    add_func_Module(mod, "sin", &_sin, "");
    add_func_Module(mod, "tan", &_tan, "");
    add_func_Module(mod, "ln", &_ln, "");
    add_func_Module(mod, "log", &_log, "");
    add_func_Module(mod, "sqrt", &_sqrt, "");
    add_func_Module(mod, "ceil", &_ceil, "");
    add_func_Module(mod, "floor", &_floor, "");

    
    add_Object_Module(mod, "pi", new_floap(M_PI));
    add_Object_Module(mod, "e", new_floap(M_E));

    return mod;
}
#endif
