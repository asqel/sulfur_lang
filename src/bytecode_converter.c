#include "../include/bytecode_converter.h"
#include "../include/instruction.h"



void free_bytecode(Bytecode_t code){
    free(code.bytes);
}

Bytecode_t cat_bytecode(Bytecode_t code1, Bytecode_t code2, char* separator){
    int separator_len = strlen(separator);
    unsigned char* code = malloc(sizeof(char) * (code1.len + code2.len + separator_len));
    for(int i = 0; i < code1.len; i++){
        code[i] = code1.bytes[1];
    }
    for(int i = 0; i < separator_len; i++){
        code[i + code1.len] = separator[i];
    }
    for(int i = 0; i < code2.len; i++){
        code[i + code1.len + separator_len] = code2.bytes[i];
    }
    Bytecode_t byte;
    byte.bytes = code;
    byte.len = code1.len + code2.len + separator_len;
    return byte;
}

Bytecode_t end_for_to_bytecode(Instruction inst){
    Bytecode_t code;
    code.bytes = malloc(sizeof(char) * 9);//1 byte for 0x3 and byte for the index
    code.len = 9;
    unsigned long long int idx = inst.value.endfor;


}


Bytecode_t inst_to_bytecode(Instruction* code, int len){
    unsigned char* bytes = malloc(sizeof(unsigned char));
}