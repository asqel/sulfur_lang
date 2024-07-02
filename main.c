#include <stdlib.h>
#include <stdio.h>

#include "include/make_include.h"
#include "include/sulfur.h"
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include "include/func_interpreter.h"
#include "include/make_context.h"
#include "include/make_jmp_links.h"

#include "sulfur_libs/blt_libs/std.h"

void show_help(char *name, int full) {
    printf("Usage: %s [options] [file]\n", name);
    if (!full) {
        printf("Try '%s --help' for more information.\n", name);
        return;
    }

    printf("Options:\n"
        "  -m, --show-mem      show memory after execution\n"
        "  -p, --show-parse    show parse tree\n"
        "  -l, --show-lexe     show tokens after lexing\n"
        "  -i,                 show tokens after making the includes\n"
        "  -h, --help          show this help message and exit\n"
        "  -v, --version       show sulfur version and exit\n"
        "      --bytecode      make the bytecode of the file\n"

        "If no file is given, sulfur will run in interactive mode.\n"
        "If file is given, sulfur will execute it and exit.\n"
    );
}

void show_version() {
    printf("Sulfur %s (%s), Under the GNU-GPL v3.0\nBy @asqel and other stupid people\n", VERSION, SUB_VERSION);
}

extern ref_count* REFS;
extern int REFS_len;

extern int precision;

extern void instructions_print(Instruction* code, int code_len);

char IS_SHELL = 0;

Instruction	*finish_instrcutions(Instruction *code, int *instruction_len);


int execute_file(sulfur_args_t *args) {
    char *text = read_file(args->filepath);
    make_context();

    if (!text) {
        printf("File not found: %s\n",args->filepath);
        return 1;
    }
    Token *l = lexe(text);
    int len = token_len(l);

    if (args->show_lexe)
        tokens_print(l, "\n");

    l = make_include(l, &len, args->filepath);
    
    if (args->show_lexe_include)
        tokens_print(l, "\n");

    int instruction_len = 0;
    Instruction *code = parse(l, -1, -1, NULL, &instruction_len);

    //if (args->show_parse) {
    //    instructions_print(code, instruction_len);
    //}

    if (CTX.requested_vars[0] != NULL) DEBUG("req vars left\n");
    for(int i = 0; CTX.requested_vars[i] != NULL; i++) {
        DEBUG("    %d : %s\n", i, CTX.requested_vars[i]);
    }
    if (CTX.requested_vars_right[0] != NULL) DEBUG("req vars right\n");
    for(int i = 0; CTX.requested_vars_right[i] != NULL; i++) {
        DEBUG("    %d : %s\n", i, CTX.requested_vars_right[i]);
    }
    code = make_jmp_links(code, instruction_len);
    code = finish_instrcutions(code, &instruction_len);

    if (args->show_parse) {
        instructions_print(code, instruction_len);
    }

    // parser copy values of tokens so
    // you can free tokens after parsing 
    
    init_memory();
    init_stack(); 
    init_libs(args->filepath);  

    //make bytecode
    //if (args->make_bytecode) {
    //    int len = strlen(args->filepath);
    //    char *new_path = malloc(len + 3);
    //    strcpy(new_path, args->filepath);
    //    strcat(new_path, "bc");
    //    FILE *f = fopen(new_path,"w");
    //    Bytecode_t b = make_bytecode(code, instruction_len);
    //    fwrite(b.bytes, 1, b.len, f);
    //    fclose(f);
    //    free_bytecode(b);
    //    free(new_path);
    //}

    execute(code, args->filepath, instruction_len);
    call_to_call_and_free();
    

    precision = base_precision;
    if (args->show_mem) {
        printf("\n\nMEMORY:%d\n",MEMORY.len);
        for(int i = 0; i < MEMORY.len; i++){
            printf("    %s: ",MEMORY.keys[i]);
            println_prompt(&MEMORY.values[i],1);
        }
        printf("Press enter to quit\n");
        getchar();
    }
    for(int i = 0; i < MEMORY.len; i++){
        free(MEMORY.keys[i]);
        Obj_free_val(MEMORY.values[i]);
    }
    for(int i = 0; i < len; i++){
        free_tok_val(l[i]);
    }
    instruction_free_array(code, instruction_len);
    free(l);

    free(MEMORY.keys);
    free(MEMORY.values);

    for (int i = 0; CTX.requested_vars[i] != NULL; i++) {
        free(CTX.requested_vars[i]);
    }
    free(CTX.requested_vars);
    for (int i = 0; CTX.requested_vars_right[i] != NULL; i++) {
        free(CTX.requested_vars_right[i]);
    }
    free(CTX.requested_vars_right);
    ///print refs
    //printf("            total %d\n", REFS_len);
    //for(int i = 0; i <  REFS_len; i++) {
    //    printf("        %p count %d type %d\n", REFS[i].address, REFS[i].count, REFS[i].type);
    //}
    
    free(REFS);
    free(text);

    return 0;
}


char *INTERPRETER_PATH = NULL;
char *INTERPRETER_DIR = NULL;
char *LIBS_DIR = NULL;
char *S_INCLUDE_DIR = NULL;

void init_paths(char *path) {
    if (NULL == INTERPRETER_PATH)
        INTERPRETER_PATH = uti_strdup(path);
    if (NULL == INTERPRETER_DIR)
        INTERPRETER_DIR = uti_dirname(path);
    if (NULL == LIBS_DIR)
        LIBS_DIR = str_cat_new(INTERPRETER_DIR, "/libs");
    if (NULL == S_INCLUDE_DIR)
        S_INCLUDE_DIR = str_cat_new(INTERPRETER_DIR, "/s_include");
}

void free_paths() {
    free(INTERPRETER_PATH);
    free(INTERPRETER_DIR);
    free(LIBS_DIR);
    free(S_INCLUDE_DIR);
}
#include <signal.h>

void segvHandler(int s) {
    PRINT_ERR("Segmentation Fault\n");
    exit(EXIT_FAILURE);
}

int main(int argc,char **argv) {
    signal(SIGSEGV, segvHandler);

    init_paths(argv[0]);

    init_dyn_libs();

    back_slash_to_path(argv[0]);
    
    sulfur_args_t *args = parse_main_args(argc, argv);
    int exit_code;

    init_to_call();

    if (args->help) {
        show_help(argv[0], args->help == 2);
        exit_code = args->help == 1;
        free(args);
        return exit_code;
    }

    if (args->version) {
        show_version();
        free(args);
        return 0;
    }
    
    if (args->filepath) {
        exit_code = execute_file(args);
    } else {
        exit_code = interactive_shell(args);
    }
    free(args);
    
    for (int i = 0; i < CTX.argc; i++) {
        free(CTX.argv[i]);
    }
    free(CTX.argv);

    free_dyn_libs();

    free_paths();

    return exit_code;
}
