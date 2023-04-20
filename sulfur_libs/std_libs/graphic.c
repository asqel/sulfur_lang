#include "../../include/memlib.h"

#ifdef _WIN32
#include <windows.h>

#elif __APPLE__
#elif __linux__
#else
#endif




Object init_graphic(){
    #ifdef _WIN32

    #elif __APPLE__
    #elif __linux__
    #else
        printf("ERROR  your OS doesnt support an implentation of a graphic library\n");
        exit(1);
    #endif 
    return nil_Obj;
}
//args : x, y, r, g, b
Object set_pixel(Object *args, int argc){
    if (argc != 5){
        printf("ERROR set_pixel only takes 5 args");
        exit(1);
    } 
    int x=0;
    int y=0;
    int r=0;
    int g=0;
    int b=0;
    for(int i=0;i<5;i++){
        if(args[i].type != Obj_ount_t && args[i].type != Obj_floap_t){
            printf("ERROR set_pixel only take ount or floap as %dth arg");
            exit(1);
        }
    }
    x=args[0].type == Obj_ount_t ? *args[0].val.i : *args[0].val.f;
    y=args[1].type == Obj_ount_t ? *args[1].val.i : *args[1].val.f;
    r=args[2].type == Obj_ount_t ? *args[2].val.i : *args[2].val.f;
    g=args[3].type == Obj_ount_t ? *args[3].val.i : *args[3].val.f;
    b=args[4].type == Obj_ount_t ? *args[4].val.i : *args[4].val.f;
    #ifdef _WIN32
    
    #endif 
}  


Object __loader(){
    Object mod=new_Module();
    add_func_Module(mod,"init_graphic",&init_graphic,"");

}