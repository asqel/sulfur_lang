#include "../include/sulfur.h"
#include <stdlib.h>


#ifndef ONE_FILE
    #ifdef _WIN32
        #include <windows.h>
    #elif __profanOS__
        //nothing dont import dlfcn
    #elif __APPLE__ || __linux__
        #include <dlfcn.h>
    #endif
#endif


void **DYN_LIBS = NULL;
int DYN_LIBS_COUNT;

void init_dyn_libs() {
    DYN_LIBS = malloc(sizeof(void **));
    DYN_LIBS_COUNT = 0;
}

void add_dyn_lib(void *lib) {
    DYN_LIBS_COUNT++;
    DYN_LIBS = realloc(DYN_LIBS, sizeof(void**) * DYN_LIBS_COUNT);
    DYN_LIBS[DYN_LIBS_COUNT - 1] = lib;
}

void free_dyn_libs() {
    #ifndef ONE_FILE
        #ifdef _WIN32
            for (int i = 0; i < DYN_LIBS_COUNT; i++) {
                FreeLibrary(DYN_LIBS[i]);
            }
        #elif __APPLE || __linux__
            for (int i = 0; i < DYN_LIBS_COUNT; i++) {
                dlclose(DYN_LIBS[i]);
            }
            
        #endif
    #endif
    if (DYN_LIBS) {
        free(DYN_LIBS);
    }
}


void set_errno(int is_error, int type, int err_num, int extra) {
    CTX.errno[0] = is_error;
    CTX.errno[0] = type;
    CTX.errno[0] = err_num;  
    CTX.errno[0] = extra;
}