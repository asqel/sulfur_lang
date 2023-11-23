#ifndef GRAPHIC_GRAPHIC_H
#define GRAPHIC_GRAPHIC_H
#ifndef ONE_FILE
    #include "../../../build/API/memlib.h"
#else
    #include "../../../include/memlib.h"
#endif

#ifndef ONE_FILE
    extern Sulfur_ctx context;
#endif

extern int width;
extern int height;

Object init_graphic();

Object show_window();

Object update_window();

Object set_width(Object *argv, int argc);

Object set_height(Object *argv, int argc);

//args : x, y, r, g, b
Object set_pixel(Object *argv, int argc);

Object set_title(Object* argv, int argc);

Object fill_window(Object* argv,int argc);

//x y w h r g b 
Object fill_rect(Object* argv, int argc);


Object GetWindowEvents(Object * argv, int argc);


Object graphic_get_height(Object *argv, int argc);
Object graphic_get_width(Object *argv, int argc);

#ifndef ONE_FILE
Object __loader();
#else
Object __load_graphic();
#endif

#endif