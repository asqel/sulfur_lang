#ifndef MEMLIB_H
#define MEMLIB_H
#include <string.h>


typedef char*   S_string_t;
typedef char    S_boolean_t;
#ifndef SULFUR_32
    typedef long long int   S_ount_t;
    typedef long long int   S_sulfur_int;
    typedef long double     S_floap_t;
    typedef long double     S_float_t;
    typedef double     S_short_float_t;
    #define sulfur_int_abs  llabs
    #define sulfur_float_abs fabsl
    #define sulfur_sqrt     sqrtl
    #define sulfur_pow      pow
#else
    typedef long int    S_ount_t;
    typedef long int    S_sulfur_int;
    typedef double      S_floap_t;
    typedef double      S_float_t;
    typedef float     S_short_float_t;
    #define sulfur_int_abs      labs
    #define sulfur_float_abs    fabs
    #define sulfur_sqrt     sqrt
    #define sulfur_pow      powf
#endif

typedef struct Object Object;


typedef struct class{
    struct memory * MEM;
    char*class_name;
}class;

typedef struct Module{
    struct memory*MEM;
    char*filename;
}Module;

typedef struct {
    char** args;
    int args_len;
    char args_mod;  // 'o' = only args_len,  '+' = args_len or more
    struct Instruction* code;
    int code_len;
}sulfur_func;

/*every blt function should be definied like this but inn theory it's not important
{
    ret_type: NULL
    nbr_ret_type: 0
    arg_types: NULL
    arg_names: NULL
    nbr_of_args: -1
    is_builtin: 1
    func_p: pointer to the builtin function
    code: NULL
    description: a string for the description of the function if there is one
    
}
or you can just set is_builtin to 1 and not set the others

the builtin function written in c has to to take 2 argument: an array of Object and the length of this array
if the function number of args=0 a NULL pointer is passed
return type must be Object
*/
typedef struct Funcdef{
    //for builtin
    struct Object (*func_p)(struct Object*,int);//pointer to the builtin function
    //common
    char*description;//can be shown with help()
    char is_builtin;
    char*name;
    //for sulfuric function
    sulfur_func *defs;
    int defs_len;
}Funcdef;

typedef struct list{
    struct Object*elements;// elements[0] is the length stored in an object
}list;

typedef struct memory{
    struct Object*values;
    char**keys;
    int len;
}memory;

typedef struct stack{
    memory* MEM;
    int len;
}stack;

typedef struct Custom_obj {
    char *type_name;
    void *data;
}Custom_obj;

typedef union Obj_val{
    struct Object *o;
    S_string_t    s;//string {char,...}
    S_ount_t      i;//ount 
    S_floap_t     f;//floap
    S_short_float_t     c[2];//complex {re,im}
    S_boolean_t   b;//boolean 
    class       *cl;//class
    class       *cl_def;
    Funcdef     *funcid;//contain a function identifier
    char        *typeid;//contain a type
    Module      *module;
    list *      li;
    Custom_obj  *cst_obj;
}Obj_val;

typedef struct Object{
    short int type;
    Obj_val val;
}Object;

enum Obj_Type{
    Obj_not_found_t,
    Obj_Obj_t,
    Obj_string_t,
    Obj_ount_t,
    Obj_floap_t,
    Obj_complex_t,
    Obj_boolean_t,
    Obj_nil_t,//value is stored in .b not .i like in tokens
    Obj_list_t,// list starts  with Object that has value of len of the list
    Obj_end_t,// can be used to make list withput putting the len at index 0
    Obj_funcid_t,//contains addres of function not name
    Obj_typeid_t,
    Obj_class_t,
    obj_module_t,
    Obj_class_def_t,
    Obj_class_instance_t,
    Obj_custom_obj_t
};


static Object end_Obj ={.type = Obj_end_t, .val.b = -1};
static Object nil_Obj ={.type = Obj_nil_t, .val.b = 0};
static Object not_found_Obj ={.type = Obj_not_found_t, .val.b = 0};

int Obj_free_val(Object obj);

void*get_obj_pointer(Object o);


void Objs_print(Object*obj,int len);
void Obj_print(Object obj);

//name will be copied
Funcdef new_blt_func(Object (*func)(Object*, int), char *desc, char *name);


memory *add_func(memory *MEMORY, char *name, Object (*func)(Object *, int), char *desc);

memory *add_object(memory *MEMORY, char *name, Object x);

memory *add_obj_str(memory *MEMORY, char *name, S_string_t val);



Object new_Module();


void add_func_Module(Object mod, char *name, Object (*func)(Object *, int), char *desc);

void add_Object_Module(Object mod, char*name,Object x);


Object new_ount(S_ount_t value);

Object new_floap(S_floap_t value);

Object new_complex(S_floap_t re, S_floap_t im);

//the string will be copied *
Object new_string(S_string_t value);


Object new_boolean(S_ount_t value);

Object Obj_cpy(Object o);

int is_muttable(Object o);

Object get_Obj_mem(memory MEMORY, char* name);


void Obj_free_array(Object* objs, int len);

int get_list_len(Object l);

typedef struct ref_count{
    int count;
    void* address;
    int type;
}ref_count;

void add_count(void* address, int type);

void remove_count(void* address, int type);


typedef struct Sulfur_ctx{
    void *(**memlib_func)();
    void *(**operations)();
    Object (**std_func)(Object *, int);
    void *vars;
    memory *MEM;
    int *_errno; // len = 4 [is_error,type,error_number,extra]
    char **argv; // args passed after filename can be NULL
    int argc;

    char **requested_vars; // used by the bytecode and interpreter and libs
    char **requested_vars_right; // same but for the right side of the dot operator

} Sulfur_ctx;

char *Obj_type_as_str(short int type);

memory*add_object_cpy(memory *MEMORY, char *name, Object x);


extern void (**TO_CALL)();
extern int TO_CALL_LEN;

extern void **protected_refs;
extern int protected_refs_len;

void add_protected(void *address);

void  init_to_call();

void add_to_call(void (*func)());

void call_to_call_and_free();

void add_Object_Module_cpy(Object mod, char*name,Object x);

S_sulfur_int hash_str(char *str);

#endif
