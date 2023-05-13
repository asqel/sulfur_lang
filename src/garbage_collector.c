#include <stdio.h>
#include "../include/garbage_collector.h"
#include "../include/memlib.h"

ref_counter* REF_COUNT;

void init_garbage(){
    REF_COUNT = malloc(sizeof(ref_counter));
    REF_COUNT->len = 0;
    REF_COUNT->REFS = malloc(sizeof(ref));
}

int add_ref(Object obj){
    if(is_muttable(obj)){
        for(int i=0; i < REF_COUNT->len; i++){
            if(REF_COUNT->REFS[i].obj_val_p == get_obj_pointer(obj)){
                REF_COUNT->REFS[i].count++; 
                if(obj.type == Obj_list_t){
                    for(int i = 1; i <= *obj.val.li->elements[0].val.i; i++){
                        add_ref(obj.val.li->elements[i]);
                    }
                }
               return 1;
            }
        }
        (REF_COUNT->len)++;
        REF_COUNT->REFS = realloc(REF_COUNT->REFS, sizeof(ref) * REF_COUNT->len);
        REF_COUNT->REFS[REF_COUNT->len - 1].count = 1;
        REF_COUNT->REFS[REF_COUNT->len - 1].obj_val_p = get_obj_pointer(obj);
        REF_COUNT->REFS[REF_COUNT->len - 1].type = obj.type;
        if(obj.type == Obj_list_t){
            for(int i = 1; i <= *obj.val.li->elements[0].val.i; i++){
                add_ref(obj.val.li->elements[i]);
            }
        }
        return 0;
    }
    return -1;
}

int remove_ref(Object obj){
    if(is_muttable(obj)){
        for(int i=0; i < REF_COUNT->len; i++){
            if(REF_COUNT->REFS[i].obj_val_p == get_obj_pointer(obj)){
                REF_COUNT->REFS[i].count--; 
                if(obj.type == Obj_list_t){
                    for(int i = 1; i <= *obj.val.li->elements[0].val.i; i++){
                        remove_ref(obj.val.li->elements[i]);
                    }
                }
               return 1;
            }
        }
    }
    return -1;
}

int temp_ref(Object obj){
    if(is_muttable(obj)){
        for(int i=0; i < REF_COUNT->len; i++){
            if(REF_COUNT->REFS[i].obj_val_p == get_obj_pointer(obj)){
                if(obj.type == Obj_list_t){
                    for(int i = 1; i <= *obj.val.li->elements[0].val.i; i++){
                        temp_ref(obj.val.li->elements[i]);
                    }
                }
                return 1;
            }
        }
        (REF_COUNT->len)++;
        REF_COUNT->REFS = realloc(REF_COUNT->REFS, sizeof(ref) * REF_COUNT->len);
        REF_COUNT->REFS[REF_COUNT->len - 1].count = 0;
        REF_COUNT->REFS[REF_COUNT->len - 1].obj_val_p = get_obj_pointer(obj);
        REF_COUNT->REFS[REF_COUNT->len - 1].type = obj.type;
        if(obj.type == Obj_list_t){
            for(int i = 1; i <= *obj.val.li->elements[0].val.i; i++){
                temp_ref(obj.val.li->elements[i]);
            }
        }
        return 0;
    }
    return -1;
}

int update_ref(){
    for(int i = 0; i < REF_COUNT->len; i++){
        if(REF_COUNT->REFS[i].count <= 0){
            if(REF_COUNT->REFS[i].type == Obj_list_t){
                for(int i = 1; i <= ((list *)REF_COUNT->REFS[i].obj_val_p)->elements[0].val.i; i++){
                    //free(((list *)REF_COUNT->REFS[i].obj_val_p)->elements[i]);
                    // free obj_val if not muttable
            }
            }
            for(int k = i + 1; k < REF_COUNT->len; k++){
            
            }
        }
    }
}