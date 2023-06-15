#include <stdlib.h>
#include <stdio.h>
#include "../include/lexer.h"
#include "../include/memlib.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/func_interpreter.h"
#include "../sulfur_libs/blt_libs/std.h"


extern int show_parse;
extern int show_mem;
extern char* VERSION;


extern void instructions_print(Instruction* code, int code_len);

int does_code_is_good(char *code) {
    // return 0 if code is incomplete
    // return 1 if code is good
    // return 2 if code is 'q'

    int nub = 0;
    int len = strlen(code);

    while (code[len - 1] == ' ' || code[len - 1] == '\n') {
        len--;
    }

    if (len == 0) return 1;

    // check if all '(' are closed
    for (int i = 0; i < len; i++) {
        if (code[i] == '(') nub++;
        if (code[i] == ')') nub--;
    }
    if (nub != 0) return 0;

    // check if all '{' are closed
    for (int i = 0; i < len; i++) {
        if (code[i] == '{') nub++;
        if (code[i] == '}') nub--;
    }
    if (nub != 0) return 0;

    // check if all '[' are closed
    for (int i = 0; i < len; i++) {
        if (code[i] == '[') nub++;
        if (code[i] == ']') nub--;
    }
    if (nub != 0) return 0;

    if (len == 1 && code[0] == 'q') return 2;

    // check if the line ends with ';', '}'
    if (code[len - 1] != ';' && code[len - 1] != '}') {
        return 0;
    }
    return 1;
}

int interactive_shell() {
    printf("Welcome to sulfur v%s interactive shell!\n", VERSION);
    printf("Type 'q' or destroy your computer to exit\n");

    // init
    init_memory();
    init_stack();
    init_libs("*shell*");

    // main loop
    char *code = calloc(1000, sizeof(char));
    int decl = 0;
    int exit_code;
    while (1) {
        // read line
        printf("sulfur > ");
        while (1) {
            char *line = fgets(code + decl, 1000 - decl, stdin);
            if (!line) break;
            decl += strlen(line);
            exit_code = does_code_is_good(code);
            if (exit_code) break;
            printf("       > ");
        }

        if (exit_code == 2) break;

        if (code[0] == '\n') {
            decl = 0;
            continue;
        }

        // tokenize
        Token *l = lexe(code);
        int len = token_len(l);

        // parse
        int instruction_len = 0;
        Instruction *insts = parse(l, -1, -1, NULL, &instruction_len);

        if (show_parse) {
            instructions_print(insts, instruction_len);
        }

        // execute
        execute(insts, "*shell*", instruction_len);

        // clean up
        decl = 0;
    }

    // clean memory
    free(code);

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
}
