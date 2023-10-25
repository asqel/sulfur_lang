#ifndef ONE_FILE
    #include "../../../build/API/memlib.h"
#else
    #include "../../../include/memlib.h"
#endif
#include "graphic.h"

#ifndef ONE_FILE
    Sulfur_ctx context;
#else
    #include "../blt_libs/std.h"
#endif

#ifndef ONE_FILE
Object __loader(Sulfur_ctx ctx){
    context = ctx;
    Object mod = new_Module();

    add_func_Module(mod, "init_graphic", &init_graphic, "");
    add_func_Module(mod, "set_pixel", &set_pixel, "");
    add_func_Module(mod, "show_window", &show_window, "");
    add_func_Module(mod, "update_window", &update_window, "");
    add_func_Module(mod, "fill_rect", &fill_rect, "");
    add_func_Module(mod, "set_title", &set_title, "");
    add_func_Module(mod, "set_width", &set_width, "");
    add_func_Module(mod, "set_height", &set_height, "");
    add_func_Module(mod, "fill_window", &fill_window, "");
    add_func_Module(mod,  "update", &GetWindowEvents, "");
 
    return mod;

}
#else
Object __load_graphic(Sulfur_ctx ctx){
    Object mod=new_Module();
    (void)ctx;

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
