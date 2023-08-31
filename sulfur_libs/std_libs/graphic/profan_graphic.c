#if __profanOS__

#include "../../../include/memlib.h"
#include <syscall.h>

int width = 400;
int height = 300;

Object init_graphic(Object *argv, int *argc)
{
    return nil_Obj;
}

Object show_window()
{
    return nil_Obj;
}

Object update_window()
{
    return nil_Obj;
}

Object set_width(Object *argv, int argc)
{
    if(argc != 1)
        return new_ount(-1);
    if(argv[0].type != Obj_ount_t)
        return new_ount(-1);
    width = *argv[0].val.i;
    return nil_Obj;
}

Object set_height(Object *argv, int argc)
{
    if(argc != 1)
        return new_ount(-1);
    if(argv[0].type != Obj_ount_t)
        return new_ount(-1);
    height = *argv[0].val.i;
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
    uint32_t x = *argv[0].val.i;
    uint32_t y = *argv[1].val.i;

    uint32_t color = (*argv[2].val.i << 16) | (*argv[3].val.i << 8) | *argv[4].val.i;

    c_vesa_set_pixel(x, y, color);

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

    uint32_t color = (*argv[0].val.i << 16) | (*argv[1].val.i << 8) | *argv[2].val.i;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            c_vesa_set_pixel(i, j, color);

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

    int x = *argv[0].val.i;
    int y = *argv[1].val.i;
    int w = *argv[2].val.i;
    int h = *argv[3].val.i;

    uint32_t color = (*argv[4].val.i << 16) | (*argv[5].val.i << 8) | *argv[6].val.i;

    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            c_vesa_set_pixel(i, j, color);

    return nil_Obj;
}


Object GetWindowEvents(Object * argv, int argc){
    return nil_Obj;
}
#endif
