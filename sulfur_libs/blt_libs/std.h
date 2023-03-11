#ifndef STD_H
#define STD_H

#include "../../include/memlib.h"


Object read_prompt(Object*o);
 
Object print_prompt(Object*obj,int n_arg);

Object println_prompt(Object*obj,int n_arg);

Object std_bool(Object*obj,int n_arg);

#endif 