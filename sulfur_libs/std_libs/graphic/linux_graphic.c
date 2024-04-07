#if __profanOS__
#elif __APPLE__
#elif __linux__

#if !defined(ONE_FILE) || ONE_FILE == 0
    #include "../../../build/API/memlib.h"
#else
    #include "../../../include/memlib.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#define WIN_FLAGS ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask

int width = 400;
int height = 300;
int screen;

Display *display;
Window window;

Pixmap buffer;

void to_free_graphic() {
    XDestroyWindow(display, window);
    XFreePixmap(display, buffer);
    XCloseDisplay(display);
}


Object init_graphic(Object *argv, int *argc)
{
    add_to_call(&to_free_graphic);
    display = XOpenDisplay(NULL);
    if (NULL == display)
        return new_ount(-1);
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0,
                                 width, height, 0, 0, WhitePixel(display, screen));
    XSelectInput(display, window, WIN_FLAGS);
    buffer = XCreatePixmap(display, window, width, height, DefaultDepth(display, 0));
    return nil_Obj;
}

Object show_window()
{
    XSelectInput(display, window, StructureNotifyMask);
    XMapWindow(display, window);
    XEvent e;
    do {
        XNextEvent(display, &e);
    } while (e.type != MapNotify);
    XSelectInput(display, window, WIN_FLAGS);
    XFlush(display);
    return nil_Obj;
}

Object update_window()
{
    GC gc = XCreateGC(display, window, 0, NULL);
    XCopyArea(display, buffer, window, gc, 0, 0, width, height, 0, 0);
    XFreeGC(display, gc);
    return nil_Obj;
}

void graphic_handle_window_resize(int new_width, int new_height) {
    int old_width = width;
    int old_height = height;
    width = new_width;
    height = new_height;
    GC gc = XCreateGC(display, buffer, 0, NULL);
    Pixmap new_buffer = XCreatePixmap(display, window, width, height, DefaultDepth(display, 0));
    XCopyArea(display, buffer, new_buffer, gc, 0, 0, old_width, old_height, 0, 0);
    XFreeGC(display, gc);
    XFreePixmap(display, buffer);
    buffer = new_buffer;
}

Object set_width(Object *argv, int argc)
{
    if(argc != 1)
        return new_ount(-1);
    if(argv[0].type != Obj_ount_t)
        return new_ount(-1);
    int new_width = argv[0].val.i;
    Pixmap new_buffer = XCreatePixmap(display, window, new_width, height, DefaultDepth(display, 0));
    GC gc = XCreateGC(display, buffer, 0, NULL);
    XCopyArea(display, buffer, new_buffer, gc, 0, 0, new_width, height, 0, 0);
    XFreeGC(display, gc);
    XFreePixmap(display, buffer);
    buffer = new_buffer;
    width = new_width;
    XResizeWindow(display, window, new_width, height);
    return nil_Obj;
}

Object set_height(Object *argv, int argc)
{
    if(argc != 1)
        return new_ount(-1);
    if(argv[0].type != Obj_ount_t)
        return new_ount(-1);
    int new_height = argv[0].val.i;
    Pixmap new_buffer = XCreatePixmap(display, window, width, new_height, DefaultDepth(display, 0));
    GC gc = XCreateGC(display, buffer, 0, NULL);
    XCopyArea(display, buffer, new_buffer, gc, 0, 0, width, height, 0, 0);
    XFreeGC(display, gc);
    XFreePixmap(display, buffer);
    buffer = new_buffer;
    height = new_height;
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
    int x = argv[0].val.i;
    int y = argv[1].val.i;

    XColor color;
    color.red = argv[2].val.i * 65535 / 255;
    color.green = argv[3].val.i * 65535 / 255;
    color.blue = argv[4].val.i * 65535 / 255;
    color.flags = DoRed | DoGreen | DoBlue;

    GC gc_buffer = XCreateGC(display, buffer, 0, NULL);
    if (!XAllocColor(display, DefaultColormap(display, screen), &color)) {
        XFreeGC(display, gc_buffer);
        return new_ount(-1);
    }
    XSetForeground(display, gc_buffer, color.pixel);
    XDrawPoint(display, buffer, gc_buffer, x, y);

    return nil_Obj;
}

Object set_title(Object* argv, int argc){
    return nil_Obj;
}

Object fill_window(Object* argv, int argc) {
    if (argc != 3)
        return new_ount(-1);

    for (int i = 0; i < 3; i++) {
        if (argv[i].type != Obj_ount_t)
            return new_ount(-1);
    }

    XColor color;
    color.red = argv[0].val.i * 65535 / 255;
    color.green = argv[1].val.i * 65535 / 255;
    color.blue = argv[2].val.i * 65535 / 255;
    color.flags = DoRed | DoGreen | DoBlue;

    GC gc_buffer = XCreateGC(display, buffer, 0, NULL);
    if (!XAllocColor(display, DefaultColormap(display, screen), &color)) {
        XFreeGC(display, gc_buffer);
        return new_ount(-1);
    }
    XSetForeground(display, gc_buffer, color.pixel);
    XFillRectangle(display, buffer, gc_buffer, 0, 0, width, height);

    return nil_Obj;
}

//x y w h r g b 
Object fill_rect(Object* argv, int argc){
    if(argc != 7)
        return new_ount(-1);

    for(int i = 0; i < 7; i++){
        if(argv[i].type != Obj_ount_t)
            return new_ount(-1);
    }

    int x = argv[0].val.i;
    int y = argv[1].val.i;
    int w = argv[2].val.i;
    int h = argv[3].val.i;

    XColor color;
    color.red = argv[4].val.i * 65535 / 255;
    color.green = argv[5].val.i * 65535 / 255;
    color.blue = argv[6].val.i * 65535 / 255;
    color.flags = DoRed | DoGreen | DoBlue;

    GC gc_buffer = XCreateGC(display, buffer, 0, NULL);
    if (!XAllocColor(display, DefaultColormap(display, screen), &color)) {
        XFreeGC(display, gc_buffer);
        return new_ount(-1);
    }
    XSetForeground(display, gc_buffer, color.pixel);
    XFillRectangle(display, buffer, gc_buffer, x, y, w, h);
    return nil_Obj;
}



Object graphic_get_height(Object *argv, int argc) {
    return new_ount(height);
}
Object graphic_get_width(Object *argv, int argc) {
    return new_ount(width);
}

Object GetWindowEvents(Object * argv, int argc){
    XEvent event;

    XWindowAttributes windowAttributes;
    XGetWindowAttributes(display, window, &windowAttributes);

    if (windowAttributes.width != width || windowAttributes.height != height)
        graphic_handle_window_resize(windowAttributes.width, windowAttributes.height);
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
