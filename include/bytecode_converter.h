#ifndef BYTECODE_CONVERTER_H
#define BYTECODE_CONVERTER_H


/*
files start with 0x53 0x75

instruction separator 0x1
strings are ended by 0x0

for{
    0x2 var_loop_name start end
}
endfor{
    0x3 8 bytes for the for index
}
while{
    0x4 condition
}
endwhile{
    0x5 8 bytes for the while index
}


*/


typedef struct Bytecode_t{
    unsigned char* bytes;
    int len;

}Bytecode_t;

#endif