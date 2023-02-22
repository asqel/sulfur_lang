#include "../include/instruction.h"


Lib*LIBS;
long long int LIBS_len;

void init_libs(){
    LIBS=malloc(sizeof(Lib));
    LIBS_len=0;
}
