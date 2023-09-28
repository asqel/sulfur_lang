#include "memlib.h"

Object end_Obj = {Obj_end_t, .val.b = -1};
Object nil_Obj = {Obj_nil_t, .val.b = 0};
Object not_found_Obj = {Obj_not_found_t, .val.b = 0};