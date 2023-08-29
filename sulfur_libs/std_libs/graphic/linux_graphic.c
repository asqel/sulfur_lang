#if __profanOS__
#elif __APPLE__
#elif __linux__

#include "../../../include/memlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

int width = 400;
int height = 300;
int screen;

Display *display;
Window window;


Object init_graphic(Object *argv, int *argc)
{
    Display *display = XOpenDisplay(NULL);
    if(NULL == display)
        return new_ount(-1);
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0,
                                 width, height, 0, 0, WhitePixel(display, screen));
    long int flags = ExposureMask | KeyPressMask |
                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    XSelectInput(display, window, flags);
    return nil_Obj;
}

Object show_window()
{
    XMapWindow(display, window);
    return nil_Obj;
}

Object update_window()
{
    XFlush(display);
    return nil_Obj;
}

Object set_width(Object *argv, int argc)
{
    if(argc != 1)
        return new_ount(-1);
    if(argv[0].type != Obj_ount_t)
        return new_ount(-1);
    width = *argv[0].val.i;
    XResizeWindow(display, window, width, height);
    return nil_Obj;
}

Object set_height(Object *argv, int argc)
{
    if(argc != 1)
        return new_ount(-1);
    if(argv[0].type != Obj_ount_t)
        return new_ount(-1);
    height = *argv[0].val.i;
    XResizeWindow(display, window, width, height);
    return nil_Obj;
}


//args : x, y, r, g, b
Object set_pixel(Object *argv, int argc){
    if(argc != 5)
        return new_ount(-1);
    for(int i = 0; i < 5; i++){
        if(argv[i].type != Obj_ount_t)
            return new_ount(-1);
    }
    XColor color;
    Colormap colormap;
    GC gc;

    colormap = DefaultColormap(display, DefaultScreen(display));
    XAllocColor(display, colormap, &color);

    color.red = *argv[2].val.i * 256;
    color.green = *argv[3].val.i * 256;
    color.blue = *argv[4].val.i * 256;

    gc = XCreateGC(display, window, 0, NULL);

    XSetForeground(display, gc, color.pixel);

    XDrawPoint(display, window, gc, *argv[0].val.i, *argv[1].val.i);

    XFreeGC(display, gc);
    XFreeColors(display, colormap, &color.pixel, 1, 0);
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
    XEvent event;

    while (XPending(display)) {
        XNextEvent(display, &event);

        /* Gestion des événements */
        switch (event.type) {
            case Expose:
                /* Redessiner la fenêtre */
                break;
            case KeyPress:
                /* Traitement des touches */
                break;
            case ButtonPress:
                /* Traitement des clics de souris */
                break;
            /* ... autres types d'événements ... */
        }
    }
    return nil_Obj;
}
#endif
