#include "../../include/memlib.h"
#include <stdio.h>
#include <stdlib.h>

int width=400;
int height=300;

#ifdef _WIN32
#include <windows.h>
HWND g_hWnd = NULL;
HWND hwndTitle;


#elif __profanOS__
#elif __APPLE__
#elif __linux__
#include <X11/Xlib.h>
Display *display;
Window window;

#else
#endif




Object init_graphic(){
    return nil_Obj;
}

Object show_window(){
    return nil_Obj;
}

Object update_window(){
    return nil_Obj;
}

Object set_width(Object *args, int argc){
    return nil_Obj;
}

Object set_height(Object *args, int argc){
    return nil_Obj;
}


//args : x, y, r, g, b
Object set_pixel(Object *args, int argc){
    return nil_Obj;
}  

Object set_title(Object* argv, int argc){
    return nil_Obj;
}

Object fill_window(Object* argv,int argc){
    return nil_Obj;
}


//x y w h r g b 
Object fill_rect(Object* argv, int argc){
    return nil_Obj;
}


Object GetWindowEvents(Object * argv, int argc){
    return nil_Obj;
}

#ifndef ONE_FILE
Object __loader(){
    Object mod=new_Module();
    add_func_Module(mod,"init_graphic",&init_graphic,"");
    add_func_Module(mod,"set_pixel",&set_pixel,"");
    add_func_Module(mod,"show_window",&show_window,"");
    add_func_Module(mod,"update_window",&update_window,"");
    add_func_Module(mod,"fill_rect",&fill_rect,"");
    add_func_Module(mod,"set_title",&set_title,"");
    add_func_Module(mod,"set_width",&set_width,"");
    add_func_Module(mod,"set_height",&set_height,"");
    add_func_Module(mod,"fill_window",&fill_window,"");
    add_func_Module(mod, "update", &GetWindowEvents, "");

    return mod;

}
#else
Object __load_graphic(){
    Object mod=new_Module();
    add_func_Module(mod,"init_graphic",&init_graphic,"");
    add_func_Module(mod,"set_pixel",&set_pixel,"");
    add_func_Module(mod,"show_window",&show_window,"");
    add_func_Module(mod,"update_window",&update_window,"");
    add_func_Module(mod,"fill_rect",&fill_rect,"");
    add_func_Module(mod,"set_title",&set_title,"");
    add_func_Module(mod,"set_width",&set_width,"");
    add_func_Module(mod,"set_height",&set_height,"");
    add_func_Module(mod,"fill_window",&fill_window,"");
    add_func_Module(mod, "update", &GetWindowEvents, "");

    return mod;

}
#endif