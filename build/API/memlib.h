#ifndef MEMLIB_H
#define MEMLIB_H

struct Object;

typedef char*   S_string_t;
typedef char    S_boolean_t;
#ifndef SULFUR_32
    typedef long long int   S_ount_t;
    typedef long long int   S_sulfur_int;
    typedef long double     S_floap_t;
#else
    typedef long int    S_ount_t;
    typedef long int    S_sulfur_int;
    typedef double      S_floap_t;
#endif

typedef struct class{
    struct memory * MEM;
    char*class_name;
}class;

typedef struct Module{
    struct memory*MEM;
    char*filename;
}Module;

typedef struct Funcdef{
    //for builtin
    struct Object (*func_p)(struct Object*,int);//pointer to the builtin function
    //common
    char*description;//can be shown with help()
    char is_builtin;
    char*name;
    //for sulfuric function
    char** args;
    int args_len;
    char args_mod;  // 'o' = only args_len,  '+' = args_len or more
    struct Instruction* code;
    int code_len;

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
    S_floap_t     c[2];//complex {re,im}
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
    Obj_class_def_t
};


static Object end_Obj ={.type = Obj_end_t, .val.b = -1};
static Object nil_Obj ={.type = Obj_nil_t, .val.b = 0};
static Object not_found_Obj ={.type = Obj_not_found_t, .val.b = 0};

typedef struct Sulfur_ctx{
    void *(**memlib_func)();
    void *(**std_func)();
    void *(**operations)();
    void *vars;
    memory *MEM;
    int *_errno; // len = 4 [is_error,type,error_number,extra]
} Sulfur_ctx;

extern Sulfur_ctx context;

#define SULFUR_MEMLIB_FUNC
    #define Obj_free_val        (*(int (*)(Object))context.memlib_func[0])
    #define get_obj_pointer     (*(void *(*)(Object))context.memlib_func[1])
    #define Objs_print          (*(void (*)(Object *, int))context.memlib_func[2])
    #define Obj_print           (*(void (*)(Object))context.memlib_func[3])
    #define new_blt_func        (*(Funcdef (*)(Object (*)(Object*, int), char *, char *))context.memlib_func[4])
    #define add_func            (*(memory *(*)(memory *, char *,Object (*func)(Object*, int),char *))context.memlib_func[5])
    #define add_object          (*(memory *(*)(memory *, char *, Object))context.memlib_func[6])
    #define add_obj_str         (*(memory *(*)(memory *, char *, char *))context.memlib_func[7])
    #define new_Module          (*(Object (*)())context.memlib_func[8])
    #define add_func_Module     (*(void (*)(Object, char *, Object (*)(Object *, int), char *))context.memlib_func[9])
    #define add_Object_Module   (*(void (*)(Object, char *, Object))context.memlib_func[10])
    #define new_ount            (*(Object (*)(S_ount_t))context.memlib_func[11])
    #define new_floap           (*(Object (*)(S_floap_t))context.memlib_func[12])
    #define new_complex         (*(Object (*)(S_floap_t, S_floap_t))context.memlib_func[13])
    #define new_string          (*(Object (*)(S_string_t))context.memlib_func[14])
    #define new_boolean         (*(Object (*)(S_ount_t))context.memlib_func[15])
    #define Obj_cpy             (*(Object (*)(Object))context.memlib_func[16])
    #define is_muttable         (*(int (*)(Object))context.memlib_func[17])
    #define get_Obj_mem         (*(Object (*)(memory, char *))context.memlib_func[18])
    #define Obj_free_array      (*(void (*)(Object *, int))context.memlib_func[19])
    #define get_list_len        (*(int (*)(Object))context.memlib_func[20])
    #define add_count           (*(void (*)(void *, int))context.memlib_func[21])
    #define remove_count        (*(void (*)(void *, int))context.memlib_func[22])
    #define add_to_call         (*(void (*)(void (*)()))context.memlib_func[23])

#define SULFUR_OPERATIONS 
    #define OP_ADD        (*(Object (*)(Object, Object)) context.operations[0])
    #define OP_SUB        (*(Object (*)(Object, Object)) context.operations[1])
    #define OP_MUL        (*(Object (*)(Object, Object)) context.operations[2])
    #define OP_DIV        (*(Object (*)(Object, Object)) context.operations[3])
    #define OP_FL_DIV     (*(Object (*)(Object, Object)) context.operations[4])
    #define OP_MOD        (*(Object (*)(Object, Object)) context.operations[5])
    #define OP_POW        (*(Object (*)(Object, Object)) context.operations[6])
    #define OP_NEGATE     (*(Object (*)(Object)) context.operations[7])
    #define OP_NOT        (*(Object (*)(Object)) context.operations[8])
    #define OP_AND        (*(Object (*)(Object, Object)) context.operations[9])
    #define OP_OR         (*(Object (*)(Object, Object)) context.operations[10])
    #define OP_EQ         (*(Object (*)(Object, Object)) context.operations[11])
    #define OP_NOT_EQ     (*(Object (*)(Object, Object)) context.operations[12])
    #define OP_LESS       (*(Object (*)(Object, Object)) context.operations[13])
    #define OP_GREATER    (*(Object (*)(Object, Object)) context.operations[14])
    #define OP_LESS_EQ    (*(Object (*)(Object, Object)) context.operations[15])
    #define OP_GREATER_EQ (*(Object (*)(Object, Object)) context.operations[16])

#define SULFUR_STD_FUNCS
    #define read_prompt         (*(Object (*)(Object *, int)) context.std_func[0])
	#define print_prompt        (*(Object (*)(Object *, int)) context.std_func[1])
	#define println_prompt      (*(Object (*)(Object *, int)) context.std_func[2])
	#define std_chr             (*(Object (*)(Object *, int)) context.std_func[2])
	#define std_bool            (*(Object (*)(Object *, int)) context.std_func[3])
	#define std_ount            (*(Object (*)(Object *, int)) context.std_func[4])
	#define std_floap           (*(Object (*)(Object *, int)) context.std_func[5])
	#define std_set_flush       (*(Object (*)(Object *, int)) context.std_func[6])
	#define std_list            (*(Object (*)(Object *, int)) context.std_func[7])
	#define current_timestamp   (*(Object (*)(Object *, int)) context.std_func[8])
	#define sleep               (*(Object (*)(Object *, int)) context.std_func[9])
	#define get                (*(Object (*)(Object *, int)) context.std_func[10])
	#define set                (*(Object (*)(Object *, int)) context.std_func[11])
	#define std_comp           (*(Object (*)(Object *, int)) context.std_func[12])
	#define std_type           (*(Object (*)(Object *, int)) context.std_func[13])
	#define set_precision      (*(Object (*)(Object *, int)) context.std_func[14])
	#define get_precision      (*(Object (*)(Object *, int)) context.std_func[15])
	#define get_methods        (*(Object (*)(Object *, int)) context.std_func[16])
	#define pop                (*(Object (*)(Object *, int)) context.std_func[17])
	#define std_asc_val        (*(Object (*)(Object *, int)) context.std_func[18])
	#define std_rand           (*(Object (*)(Object *, int)) context.std_func[19])

#endif