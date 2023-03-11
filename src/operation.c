#include "../include/memlib.h"
#include <stdlib.h>

Object add(Object a,Object b){
    if(a.type==Obj_ount_t&&a.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=*a.val.i+*b.val.i;
        return x;
    }
}