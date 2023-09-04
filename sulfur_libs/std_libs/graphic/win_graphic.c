#ifdef _WIN32

#include "../../../include/memlib.h"
#include "graphic.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


int width=400;
int height=300;


HWND g_hWnd = NULL;
HWND hwndTitle;

Object init_graphic(){
	return nil_Obj;
}

Object show_window(){
	return nil_Obj;
}

Object update_window(){
	return nil_Obj;
}

Object set_width(Object *argv, int argc){
	return nil_Obj;
}

Object set_height(Object *argv, int argc){
	return nil_Obj;
}

//args : x, y, r, g, b
Object set_pixel(Object *argv, int argc){
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

#endif
