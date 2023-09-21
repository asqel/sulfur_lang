#include <stdlib.h>
#include <stdio.h>

#include "include/sulfur.h"
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include "include/func_interpreter.h"
#include "include/make_context.h"

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
        "  -h, --help          show this help message and exit\n"
        "  -v, --version       show sulfur version and exit\n"

        "If no file is given, sulfur will run in interactive mode.\n"
        "If file is given, sulfur will execute it and exit.\n"
    );
}

void show_version() {
    printf("Sulfur %s, Under the GNU-GPL v3.0\nBy @asqel and other stupid people\n", VERSION);
}

extern ref_count* REFS;
extern int REFS_len;

extern int precision;

extern void instructions_print(Instruction* code, int code_len);

int execute_file(sulfur_args_t *args) {
    char *text = read_file(args->filepath);

    if (!text) {
        printf("File not found: %s\n",args->filepath);
        return 1;
    }
    Token *l = lexe(text);
    int len = token_len(l); 

    if (args->show_lexe)
        tokens_print(l, "\n");

    int instruction_len = 0;
    Instruction*code = parse(l, -1, -1, NULL, &instruction_len);

    if (args->show_parse) {
        instructions_print(code, instruction_len);
    }

    // parser copy values of tokens so
    // you can free tokens after parsing 
    
    init_memory();
    init_stack(); 
    init_libs(args->filepath);  
    make_context();

    execute(code, args->filepath, instruction_len);

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
    free(REFS);

    return 0;
}

int main(int argc,char **argv){
    back_slash_to_path(argv[0]);
    
    sulfur_args_t *args = parse_main_args(argc, argv);

    if (args->help) {
        show_help(argv[0], args->help == 2);
        return args->help == 1;
    }

    if (args->version) {
        show_version();
        return 0;
    }
    
    if (args->filepath) {
        return execute_file(args);
    } else {
        return interactive_shell(args);
    }
}
