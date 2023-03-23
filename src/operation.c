#include "../include/memlib.h"
#include <stdlib.h>
#include "../include/utilities.h"


/*
ount | ount
string | string
floap | floap
boolean | boolean
*/
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
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=*a.val.f+*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b+*b.val.b;
        return x;

    }
    
    printf("ERROR : operation between 2 types not supported\n");
    exit(1);
}

/*
ount | ount
floap | floap
boolean | boolean
*/
Object sub(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=*a.val.i-*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=*a.val.f-*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b-*b.val.b;
        return x;

    }
    printf("ERROR : operation between 2 types not supported\n");
    exit(1);
}


Object mul(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=*a.val.i+*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=*a.val.f+*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b+*b.val.b;
        return x;

    }
    printf("ERROR : operation between 2 types not supported\n");
    exit(1);
}

Object less(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.i<*b.val.i;
        return x;
    }
}