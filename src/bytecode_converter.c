#include "../include/bytecode_converter.h"
#include "../include/instruction.h"
#include <stdarg.h>


Bytecode_t *bytecode_append_char(Bytecode_t *code, int n, ...){
    va_list args;
    va_start(args, n);

    code->bytes = realloc(code->bytes, code->len + n);

    for (int i = 0; i < n; i++) {
        unsigned int arg = va_arg(args, unsigned int);
        code->bytes[code->len + i] = (unsigned char)arg;
    }

    va_end(args);
    code->len += n;
    return code;
}

Bytecode_t *bytecode_append_llint(Bytecode_t *code, int n, ...){
    va_list args;
    va_start(args, n);

    code->bytes = realloc(code->bytes, code->len + n);

    for (int i = 0; i < n; i++) {
        int arg = va_arg(args, long long int);
        memcpy(&code->bytes[code->len + i * sizeof(long long int)], &arg, sizeof(long long int));
    }

    va_end(args);
    code->len += sizeof(long long int) * n;
    return code;
}

Bytecode_t Ast_to_bytecode(Ast x){
    Bytecode_t code;
    code.bytes = malloc(1 * (8 * 2));
    code.len = 1 * (8 * 2);
    long long int stack_len = 0;
    long long int ast_len = 0;
    memcpy(code.bytes, &stack_len, 8);
    memcpy(&(code.bytes[8]), &ast_len, 8);
    //TODO finish this
}

Bytecode_t *bytecode_add_char_arr(Bytecode_t *x, unsigned char *bytes, int n){
    x->bytes = realloc(x->bytes, x->len + n);
    for (int i = 0; i < n; i++){
        x->bytes[x->len++] = bytes[i];
    }
    return x;
}

void free_bytecode(Bytecode_t x){
    free(x.bytes);
}

Bytecode_t Constants_to_bytecode(Object *argv, int argc){
    Bytecode_t code;
    code.bytes = malloc(1);
    code.len = 0;
    bytecode_append_llint(&code, 1, argc);
    for (int i = 0; i < argc; i++){
        if (argv[i].type == Obj_ount_t){
            bytecode_append_char(&code, 1, (char) 0x49);
            bytecode_append_llint(&code, 1, argv[i].val.i);
        }
        else if (argv[i].type == Obj_string_t){
            bytecode_append_char(&code, 1, (char) 0x53);
            bytecode_add_char_arr(&code, argv[i].val.s, strlen(argv[i].val.s) + 1);
        }
        else if (argv[i].type == Obj_floap_t){
            bytecode_append_char(&code, 1, (char) 0x46);
            double floap_val = (double)argv[i].val.f;
            memcpy(code.bytes + code.len, &floap_val, 8);
            code.len += 8;
        }
        else{
            printf("ERROR cant convert constant to bytecode type nbr %d arg %d\n", argv[i].type, i);
        }
    }
    bytecode_add_char_arr(&code, "\0\0\0\0\0\0", 6);
    return code;
}

Object *make_constants_ast(Ast x, int *constants_len, Object *constants){
    
    return constants;
}

// n : number of asts
Object *make_constants(Ast* asts, int n, int *constants_len){
    Object *constants = malloc(sizeof(Object));
    *constants_len = 0;
    for (int i = 0; i < n; i++) {
        constants = make_constants_ast(asts[i], constants_len, constants);
    }
    return constants;
}