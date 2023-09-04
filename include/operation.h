#ifndef OPERATION_H
#define OPERATION_H
#include "memlib.h"

#include "../include/memlib.h"
#include <stdlib.h>
#include "../include/utilities.h"


/*
ount | ount
string | string
floap | floap
boolean | boolean
*/
Object add(Object a,Object b);

/*
ount | ount
floap | floap
boolean | boolean
*/
Object sub(Object a,Object b);

Object mul(Object a,Object b);

Object _div(Object a,Object b);

Object _pow(Object a,Object b);

Object less(Object a,Object b);

Object greater(Object a,Object b);

Object eq(Object a,Object b);

Object not_eq(Object a,Object b);

Object and(Object a,Object b);

Object negate(Object a);
Object not(Object a);

Object mod(Object a,Object b);

Object greater_eq(Object a,Object b);

Object or(Object a,Object b);

Object less_eq(Object a,Object b);
Object fl_div(Object a,Object b);

Object lshift(Object a,Object b);

Object rshift(Object a,Object b);

#endif