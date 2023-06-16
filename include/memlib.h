#ifndef MEMLIB_H
#define MEMLIB_H
#include <string.h>

struct Object;

typedef struct class{
    struct memory * MEM;
    char*class_name;
}class;

typedef struct Module{
    struct memory*MEM;
    char*filename;
}Module;

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
    char**ret_type;// list of types that can be returned by the function
    int nbr_ret_type;//len of ret_type
    char**arg_types;
    char**arg_names;
    int nbr_of_args;
    int code_len;
    char is_builtin;
    struct Object (*func_p)(struct Object*,int);//pointer to the builtin function
    struct Instruction*code;
    char*description;//can be shown with help()
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

typedef union Obj_val{
    struct Object*o;
    char*s;//string {char,...}
    long long int*i;//ount 
    long double*f;//floap
    long double*c;//complex {re,im}
    char*b;//boolean 
    class*cl;//class
    class*cl_def;
    Funcdef *funcid;//contain a function identifier
    char*typeid;//contain a type
    Module*module;
    list*li;

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
    Obj_class_def_t
};


static Object end_Obj ={Obj_end_t,{.b=&(char){-1}}};
static Object nil_Obj ={.type = Obj_nil_t, .val.b=&(char){0}};
static Object not_found_Obj ={.type = Obj_not_found_t, .val.b=&(char){0}};

int Obj_free_val(Object obj);

void*get_obj_pointer(Object o);


void Objs_print(Object*obj,int len);
void Obj_print(Object obj);

//name will be copied
Funcdef new_blt_func(Object (*func)(Object*,int),char*desc);


memory*add_func(memory*MEMORY,char*name,Object (*func)(Object*,int),char*desc);

memory*add_object(memory*MEMORY,char*name,Object x);

memory*add_obj_str(memory*MEMORY,char*name,char*val);



Object new_Module();


void add_func_Module(Object mod, char *name, Object (*func)(Object *, int), char *desc);

void add_Object_Module(Object mod, char*name,Object x);


Object new_ount(long long int value);

Object new_floap(long double value);

//the string will be copied *
Object new_string(char * value);


Object new_boolean(int value);

Object Obj_cpy(Object o);

int is_muttable(Object o);

Object get_Obj_mem(memory MEMORY, char* name);


void Obj_free_array(Object* objs, int len);

int get_list_len(Object l);

#endif