#include "../include/memlib.h"
#include <stdlib.h>
#include "../include/utilities.h"

Object add(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=*a.val.i+*b.val.i;
        return x;
    }
    if(a.type==Obj_string_t&&b.type==Obj_string_t){
        Object x;
        x.type=Obj_string_t;
        x.val.s=str_cat_new(a.val.s,b.val.s);
        return x;
    }
}