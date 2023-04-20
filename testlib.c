#include "build/api/memlib.h"
#include <math.h>


Object qzπ(Object* arg, int argc){
    return new_floap(M_PI);
}




Object __loader(){
    Object mod=new_Module();
    add_func_Module(mod,"qzπ",&qzπ,"");
    return mod;
}