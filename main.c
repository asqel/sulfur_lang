#include <stdlib.h>
#include <stdio.h>
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include "include/func_interpreter.h"
#include "sulfur_libs/blt_libs/std.h"

#ifdef __profanOS__
#include <profan.h>
char *profan_get_current_dir();
#endif


char* VERSION = "2.3";
/*
args:
    *nothing*     - interactive shell
    *filepath*    - execute file
    -m *filepath* - show memory after execution
    -p *filepath* - show parse tree

    order of flags doesn't matter
*/

int show_mem=0;
int show_parse = 0;
char *filepath = NULL;

extern ref_count* REFS;
extern int REFS_len;

extern void parse_main_args(int argc, char** argv);
extern void instructions_print(Instruction* code, int code_len);
extern int interactive_shell();

int execute_file() {
    char *text = read_file(filepath);

    if (!text) {
        printf("File not found: %s\n",filepath);
        return 1;
    }
    
    Token *l = lexe(text);
    int len = token_len(l); 

    int instruction_len = 0;
    Instruction*code = parse(l, -1, -1, NULL, &instruction_len);

    if (show_parse) {
        instructions_print(code, instruction_len);
    }

    // parser copy values of tokens so
    // you can free tokens after parsing 
    
    init_memory();
    init_stack(); 
    init_libs(filepath);  

    execute(code, filepath, instruction_len);

    precision = base_precision;
    if (show_mem) {
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
        MEMORY.values[i].type == Obj_boolean_t ? free(MEMORY.values[i].val.b) : 0;
    }

    free(MEMORY.keys);
    free(MEMORY.values);
    free(REFS);

    return 0;
}

int main(int argc,char **argv){
    back_slash_to_path(argv[0]);
    parse_main_args(argc, argv);

    #ifdef __profanOS__

    if (filepath) {
        char *loc = profan_get_current_dir();
        char *new_file = malloc(strlen(filepath) + strlen(loc) + 2);
        assemble_path(loc, filepath, new_file);
        filepath = new_file;
    }

    #endif
    
    if (filepath) {
        return execute_file();
    } else {
        return interactive_shell();
    }
}
