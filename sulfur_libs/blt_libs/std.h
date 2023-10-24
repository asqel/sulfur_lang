#ifndef STD_H
#define STD_H

#include "../../include/memlib.h"

extern int precision;
extern int base_precision;
extern int is_flushing;

Object read_prompt(Object*o,int n_arg);

Object print_prompt(Object*obj,int n_arg);

Object println_prompt(Object*obj,int n_arg);

Object std_chr(Object* argv, int argc);

Object std_bool(Object*obj,int n_arg);

Object std_ount(Object*obj,int n_arg);

Object std_floap(Object*obj,int n_arg);

Object std_set_flush(Object* obj, int n_arg);

Object std_list(Object*obj,int n_arg);

Object current_timestamp(Object *obj,int n_arg);

Object sleep(Object *obj,int n_arg);

Object get(Object *obj,int n_arg);

Object set(Object *obj,int n_arg);

Object std_comp(Object* argv, int argc);

Object std_type(Object* obj, int n_arg);

Object set_precision(Object* args, int argc);

Object get_precision(Object* args, int argc);

Object get_methods(Object* argv, int argc);

Object pop(Object* argv, int argc);

Object std_asc_val(Object* argv, int argc);

Object std_rand(Object* argv, int argc);

memory init_std(memory MEMORY,char*path);
#endif 