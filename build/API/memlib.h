#ifndef MEMLIB_H
#define MEMLIB_H

struct Object;

typedef struct class{
    struct memory * MEM;
    char*class_name;
}class;

typedef struct Module{
    struct memory*MEM;
    char*filename;
}Module;

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
    char* s;//string {char,...}
    long long int i;//ount 
    long double f;//floap
    long double c[2];//complex {re,im}
    char b;//boolean 
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


extern Object end_Obj;
extern Object nil_Obj;
extern Object not_found_Obj;

typedef struct Sulfur_ctx{
    void *(**memlib_func)();
    void *(**std_func)();
    void *vars;
    memory *MEM;
    int *errno; // len = 4 [is_error,type,error_number,extra]
} Sulfur_ctx;

extern Sulfur_ctx context;

#define Obj_free_val        (*(int (*)(Object))context.memlib_func[0])
#define get_obj_pointer     (*(void *(*)(Object))context.memlib_func[1])
#define Objs_print          (*(void (*)(Object *, int))context.memlib_func[2])
#define Obj_print           (*(void (*)(Object))context.memlib_func[3])
#define new_blt_func        (*(Funcdef (*)(Object (*)(Object*, int), char *))context.memlib_func[4])
#define add_func            (*(memory *(*)(memory *, char *,Object (*func)(Object*, int),char *))context.memlib_func[5])
#define add_object          (*(memory *(*)(memory *, char *, Object))context.memlib_func[6])
#define add_obj_str         (*(memory *(*)(memory *, char *, char *))context.memlib_func[7])
#define new_Module          (*(Object (*)())context.memlib_func[8])
#define add_func_Module     (*(void (*)(Object, char *, Object (*)(Object *, int), char *))context.memlib_func[9])
#define add_Object_Module   (*(void (*)(Object, char *, Object))context.memlib_func[10])
#define new_ount            (*(Object (*)(long long int))context.memlib_func[11])
            
#define new_floap           (*(Object (*)(long double))context.memlib_func[12])
#define new_complex         (*(Object (*)(long double, long double))context.memlib_func[13])
#define new_string          (*(Object (*)(char *))context.memlib_func[14])
#define new_boolean         (*(Object (*)(int))context.memlib_func[15])
#define Obj_cpy             (*(Object (*)(Object))context.memlib_func[16])
#define is_muttable         (*(int (*)(Object))context.memlib_func[17])
#define get_Obj_mem         (*(Object (*)(memory, char *))context.memlib_func[18])
#define Obj_free_array      (*(void (*)(Object *, int))context.memlib_func[19])
#define get_list_len        (*(int (*)(Object))context.memlib_func[20])
#define add_count           (*(void (*)(void *, int))context.memlib_func[21])
#define remove_count        (*(void (*)(void *, int))context.memlib_func[22])


#endif