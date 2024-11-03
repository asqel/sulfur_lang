#ifndef OBJECTS_H
#define OBJECTS_H

#include "memlib.h"

#include <stdint.h>

#ifndef SULFUR_32
	typedef int64_t     bc_arch_int;
	typedef uint64_t    bc_arch_uint;
	typedef long double bc_arch_float;
#else
	typedef int32_t  bc_arch_int;
	typedef uint32_t bc_arch_uint;
	typedef double   bc_arch_float;
#endif

typedef struct bc_object bc_object;

typedef struct {
	int        len;
	bc_object  *elements;
} bc_obj_list;

typedef struct {
	int len;
	bc_object *members;
	char *(*get_member_name)(int idx);
} bc_obj_module;

typedef struct {
	// TODO : implement that 
}bc_class;

typedef struct bc_obj_custom bc_obj_custom;

struct  bc_obj_custom{
	void *data;
	int len;
	bc_class *obj_class;
	void (*free_obj)(bc_obj_custom *self);
	bc_obj_custom *(*copy_obj)(bc_obj_custom *self);
};


struct bc_object{
	char type;
	int *ref_count;
	union {
		bc_arch_int   i;
		char         *s;
		bc_arch_float f;
		bc_arch_float c[2];
		char          b;
		bc_obj_list   *li;
		Funcdef       *fc;
		bc_obj_module *mod;
		bc_obj_custom *oc; // if ref_count == NULL its an immutable object
	}val;
};

enum bc_object_types {
	bc_obj_unset_t,
	bc_obj_nil_t,
	bc_obj_ount_t,
	bc_obj_bool_t,
	bc_obj_string_t,
	bc_obj_floap_t,
	bc_obj_complex_t,
	bc_obj_list_t,
	bc_obj_funcdef_t,
	bc_obj_module_t,
	bc_obj_custom_t,
};

typedef struct {
	bc_object *values;
	int len;
} bc_scope_t;

#define bc_nil ((bc_object){.type = bc_obj_nil_t, .val.b = 0, .ref_count = NULL})
#define bc_unset ((bc_object){0})

#endif