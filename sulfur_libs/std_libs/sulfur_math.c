#if !defined(ONE_FILE) || ONE_FILE == 0
    #include "../../build/API/memlib.h"
#else
    #include "../../include/memlib.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PRINT_ERR( ...) fprintf(stderr, __VA_ARGS__)

#if !defined(ONE_FILE) || ONE_FILE == 0
    Sulfur_ctx context;
#else
    #include "../blt_libs/std.h"
#endif

Object is_even(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR is_even only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t) {
        PRINT_ERR("ERROR is_even only take ount argument\n");
        exit(1);
    }
    return new_boolean(argv[0].val.i & 1);
}

Object is_odd(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR is_even only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t) {
        PRINT_ERR("ERROR is_even only take ount argument\n");
        exit(1);
    }
    return new_boolean(! (argv[0].val.i & 1));
}

Object _cos(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR cos only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR cos only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(cos(argv[0].type == Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}

Object _sin(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR sin only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR sin only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(sin(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}

Object _tan(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR sin only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR tan only take ounti or floap argument\n");
        exit(1);
    }
    return new_floap(tan(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}

Object _ln(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR ln only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR ln only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(log(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}
Object _log(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR log only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR log only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(log10(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}

Object _sqrt(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR sqrt only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR sqqrt only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(sqrt(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}

Object _ceil(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR ceil only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR ceil only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(ceil(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}

Object _floor(Object* argv, int argc) {
    if (argc != 1) {
        PRINT_ERR("ERROR floor only take 1 one argument\n");
        exit(1);
    }
    if (argv[0].type != Obj_ount_t && argv[0].type != Obj_floap_t) {
        PRINT_ERR("ERROR floor only take ount or floap argument\n");
        exit(1);
    }
    return new_floap(floor(argv[0].type==Obj_ount_t ? argv[0].val.i : argv[0].val.f));
}


#if !defined(ONE_FILE) || ONE_FILE == 0
Object __loader(Sulfur_ctx ctx) {
    context = ctx;
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
Object __load_math(Sulfur_ctx ctx) {
    Object mod = new_Module();
    (void)ctx;

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