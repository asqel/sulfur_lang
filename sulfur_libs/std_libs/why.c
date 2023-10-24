#ifndef ONE_FILE
    #include "../../build/API/memlib.h"
#else
    #include "../../include/memlib.h"
#endif
#include <stdio.h>
#include <stdlib.h>

#ifndef ONE_FILE
    Sulfur_ctx context;
#else
    #include "../blt_libs/std.h"
#endif

Object why_miracle_sort(Object *argv, int argc) {
    Object time_to_sleep = new_ount(1); 
    int is_sorted = 0;
    // we have to wait a little for the miracle dont be impatient
    while (!is_sorted){
        is_sorted = 1;
        sleep(&time_to_sleep, 1);
        for (int i = 1; i <= argv[0].val.li->elements[0].val.i - 1; i++) {
            if (argv[0].val.li->elements[i].type == argv[0].val.li->elements[i + 1].type && argv[0].val.li->elements[i].type == Obj_ount_t) {
                if (argv[0].val.li->elements[i].val.i <= argv[0].val.li->elements[i + 1].val.i) {
                    continue;
                }
            }
            is_sorted = 0;
            break;
        }   
        
    }
    
    return nil_Obj;
}

Object why_private_pop(Object obj, int index) {
    Obj_free_val(obj.val.li->elements[index]);
    for (int i = index + 1; i <= (obj.val.li->elements[0].val.i); i++) {
        obj.val.li->elements[i - 1] = obj.val.li->elements[i];
    }
    
    obj.val.li->elements = realloc(obj.val.li->elements, sizeof(Object) * obj.val.li->elements[0].val.i);
    obj.val.li->elements[0].val.i--;
    return obj;
}

#define why_element(X, i)  X.val.li->elements[i]
#define why_len(X) X.val.li->elements[0].val.i

Object why_our_sort(Object *argv, int argc) {
    if (argc != 1) {
        printf("ERROR why:our_sort only takes 1 arg");
        exit(1);
    }
    if (argv[0].type != Obj_list_t) {
        printf("ERROR why:our_sort only takes a list as arg");
        exit(1);
    }
    for (int i = 1; i <= why_len(argv[0]) - 1; i++) {
        if (why_element(argv[0], i).type == why_element(argv[0], i + 1).type && why_element(argv[0], i).type == Obj_ount_t) {
            if (why_element(argv[0], i).val.i > why_element(argv[0], i + 1).val.i) {
                Obj_free_val(why_element(argv[0], i + 1));
                for (int k = i + 1; k <= why_len(argv[0]) - 1; k++) {
                    why_element(argv[0], k) = why_element(argv[0], k + 1);
                }
                argv[0].val.li->elements = realloc(argv[0].val.li->elements,
                    sizeof(Object) * why_len(argv[0]));
                why_len(argv[0])--;
                i--;
            }
        }
    }   
    return nil_Obj;
}

#ifndef ONE_FILE
Object __loader(Sulfur_ctx ctx) {
    context = ctx;
    Object mod = new_Module();
    add_func_Module(mod, "miracle_sort", &why_miracle_sort, "");
    add_func_Module(mod, "our_sort", &why_our_sort, "");

    return mod;
}
#else
Object __load_why(Sulfur_ctx ctx) {
    Object mod = new_Module();
    (void)ctx;
    add_func_Module(mod, "miracle_sort", &why_miracle_sort, "");
    add_func_Module(mod, "our_sort", &why_our_sort, "");

    return mod;
}
#endif

