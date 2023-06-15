#include <stdlib.h>
#include <stdio.h>
#include "../include/lexer.h"
#include "../include/memlib.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/func_interpreter.h"

extern int show_parse;

extern void instructions_print(Instruction* code, int code_len);

int does_code_is_good(char *code) {
    int nub = 0;
    int len = strlen(code);

    while (code[len - 1] == ' ' || code[len - 1] == '\n') {
        len--;
    }

    // check if all '(' are closed
    for (int i = 0; i < len; i++) {
        if (code[i] == '(') nub++;
        if (code[i] == ')') nub--;
    }
    if (nub != 0) return 1;

    // check if all '{' are closed
    for (int i = 0; i < len; i++) {
        if (code[i] == '{') nub++;
        if (code[i] == '}') nub--;
    }
    if (nub != 0) return 1;

    // check if all '[' are closed
    for (int i = 0; i < len; i++) {
        if (code[i] == '[') nub++;
        if (code[i] == ']') nub--;
    }
    if (nub != 0) return 1;

    // check if the line ends with ';', '}'
    if (code[len - 1] != ';' && code[len - 1] != '}') {
        printf("last char is '%c'\n", code[len - 1]);
        return 1;
    }
    return 0;
}

int interactive_shell() {
    printf("welcome to sulfur interactive shell\n");

    // init
    init_memory();
    init_stack();
    init_libs("*shell*");

    // main loop
    char *code = calloc(1000, sizeof(char));
    int decl = 0;
    while (1) {
        // read line
        printf("sulfur > ");
        while (1) {
            char *line = fgets(code + decl, 1000 - decl, stdin);
            if (!line) break;
            decl += strlen(line);
            if (!does_code_is_good(code)) break;
            printf("       > ");
        }

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
}
