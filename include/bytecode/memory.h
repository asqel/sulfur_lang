#ifndef MEMORY_H
#define MEMORY_H

#include "objects.h"

void bc_destroy_obj(bc_object x);

void bc_remove_ref(bc_object x);

bc_object bc_get_surface_copy(bc_object x);

bc_object bc_new_ount(bc_arch_int x);
bc_object bc_new_bool(bc_arch_int x);
bc_object bc_new_floap(bc_arch_float x);
bc_object bc_new_string(char *str);
bc_object bc_new_complex(bc_arch_float re, bc_arch_float im);

#endif