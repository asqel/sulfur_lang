#include "../include/make_context.h"
#include "../include/memlib.h"
#include "../include/sulfur.h"
#include "../sulfur_libs/blt_libs/std.h"
#include "../include/operation.h"

int	CTX_ERRNO[4];
extern memory MEMORY;

Sulfur_ctx CTX;

void *(*MEMLIB_FUNC[])() = {
	(void *(*)()) &Obj_free_val,
	(void *(*)()) &get_obj_pointer,
	(void *(*)()) &Objs_print,
	(void *(*)()) &Obj_print,
	(void *(*)()) &new_blt_func,
	(void *(*)()) &add_func,
	(void *(*)()) &add_object,
	(void *(*)()) &add_obj_str,
	(void *(*)()) &new_Module,
	(void *(*)()) &add_func_Module,
	(void *(*)()) &add_Object_Module,
	(void *(*)()) &new_ount,
	(void *(*)()) &new_floap,
	(void *(*)()) &new_complex,
	(void *(*)()) &new_string,
	(void *(*)()) &new_boolean,
	(void *(*)()) &Obj_cpy,
	(void *(*)()) &is_muttable,
	(void *(*)()) &get_Obj_mem,
	(void *(*)()) &Obj_free_array,
	(void *(*)()) &get_list_len,
	(void *(*)()) &add_count,
	(void *(*)()) &remove_count,
	(void *(*)()) &add_to_call,
	(void *(*)()) &add_Object_Module_cpy,
	(void *(*)()) NULL
};

Object (*STD_FUNC[])(Object *, int) = {
	&read_prompt,
	&print_prompt,
	&println_prompt,
	&std_chr,
	&std_bool,
	&std_ount,
	&std_floap,
	&std_set_flush,
	&std_list,
	&current_timestamp,
	&sleep,
	&get,
	&set,
	&std_comp,
	&std_type,
	&set_precision,
	&get_precision,
	&get_methods,
	&pop,
	&std_asc_val,
	&std_rand,
	NULL

};

void* (*OPERATIONS[])() = {
	(void *(*)()) &add,
	(void *(*)()) &sub,
	(void *(*)()) &mul,
	(void *(*)()) &_div,
	(void *(*)()) &fl_div,
	(void *(*)()) &mod,
	(void *(*)()) &_pow,
	(void *(*)()) &negate,
	(void *(*)()) &not,
	(void *(*)()) &and,
	(void *(*)()) &or,
	(void *(*)()) &eq,
	(void *(*)()) &not_eq,
	(void *(*)()) &less,
	(void *(*)()) &greater,
	(void *(*)()) &less_eq,
	(void *(*)()) &greater_eq,
	(void *(*)()) NULL

};

void make_context(){
	CTX._errno = CTX_ERRNO;
	CTX._errno[0] = 0;
	CTX.MEM = &MEMORY;
	CTX.memlib_func = MEMLIB_FUNC;
	CTX.std_func = STD_FUNC;
	CTX.vars = NULL;
	CTX.operations = OPERATIONS;
}
