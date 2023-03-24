#ifndef STD_H
#define STD_H

#include "../../include/memlib.h"


Object read_prompt(Object*o,int n_arg);
 
Object print_prompt(Object*obj,int n_arg);

Object println_prompt(Object*obj,int n_arg);

Object std_bool(Object*obj,int n_arg);

Object std_ount(Object*obj,int n_arg);

Object std_list(Object*obj,int n_arg);


memory init_std(memory MEMORY);

#endif 