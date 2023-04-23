#include "../../include/memlib.h"
#include <stdio.h>
#include <stdlib.h>

int width=400;
int height=300;

#ifdef _WIN32
#include <windows.h>
HWND g_hWnd = NULL;
HWND hwndTitle;


#elif __APPLE__
#elif __linux__
#include <X11/Xlib.h>
Display *display;
Window window;

#else
#endif




Object init_graphic(){
    #ifdef _WIN32
    HINSTANCE hDll = LoadLibrary("user32.dll");
    if (hDll == NULL) {
        printf("ERROR while loading windows API in graphic");
        exit(1);
    }

    // Initialiser la fenêtre
    
    g_hWnd = CreateWindowExW(0,
                            L"STATIC",
                            L"",
                            WS_OVERLAPPEDWINDOW, 
                            CW_USEDEFAULT, 
                            CW_USEDEFAULT, 
                            width, 
                            height, 
                            NULL, 
                            NULL, 
                            GetModuleHandle(NULL),
                            NULL);
    if (g_hWnd == NULL) {
        printf("ERROR while creating window ");
        exit(1);
    }

    #elif __APPLE__
    #elif __linux__
    
    // Ouvrir une connexion vers le serveur X
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir la connexion X\n");
        exit(1);
    }

    // Créer une fenêtre
    window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                                 0, 0, width, height, 0, 0, WhitePixel(display, 0));

    // Sélectionner les événements à surveiller
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XResizeWindow(display, window, 800, height);
    XFlush(display);
    XSync(display, False);
    #else
        printf("ERROR  your OS doesnt support an implentation of a graphic library\n");
        exit(1);
    #endif 
    return nil_Obj;
}

Object show_window(){
    #ifdef _WIN32

    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);
    return nil_Obj;

    #elif __linux__
    XMapWindow(display, window);
    
    #endif
}

Object update_window(){
    #ifdef _WIN32

    UpdateWindow(g_hWnd);
    #elif __linux__
        XFlush(display);
    XSync(display, False);
    #endif
    return nil_Obj;


}

Object set_width(Object *args, int argc){
    if(argc !=1){
        printf("ERROR set_width only takes one arg");
        exit(1);
    }
    if(args[0].type != Obj_ount_t && args[0].type != Obj_floap_t){
        printf("ERROR set_width only takes ount or floap as arg");
        exit(1);
    }
    #ifdef _WIN32

    width=args[0].type == Obj_ount_t ? *args[0].val.i : (int) *args[0].val.f;
    RECT rect;
    GetWindowRect(g_hWnd, &rect);
    MoveWindow(g_hWnd, rect.left, rect.top, width, height, 0);
    #elif __linux__
    XResizeWindow(display, window, width, height);
    XFlush(display); // Mettre à jour l'affichage
    #endif
    return nil_Obj;
}

Object set_height(Object *args, int argc){
    if(argc !=1){
        printf("ERROR set_height only takes one arg");
        exit(1);
    }
    if(args[0].type != Obj_ount_t && args[0].type != Obj_floap_t){
        printf("ERROR set_height only takes ount or floap as arg");
        exit(1);
    }
    #ifdef _WIN32
    height=args[0].type == Obj_ount_t ? *args[0].val.i : (int) *args[0].val.f;
    RECT rect;
    GetWindowRect(g_hWnd, &rect);
    MoveWindow(g_hWnd, rect.left, rect.top, width, height, 0);
    #elif __linux__
    XResizeWindow(display, window, width, height);
    XFlush(display); // Mettre à jour l'affichage
    #endif
    return nil_Obj;
}


//args : x, y, r, g, b
Object set_pixel(Object *args, int argc){
    
    if (argc != 5){
        printf("ERROR set_pixel only takes 5 args");
        exit(1);
    } 
    int x=0;
    int y=0;
    int r=0;
    int g=0;
    int b=0;
    for(int i=0;i<5;i++){
        if(args[i].type != Obj_ount_t && args[i].type != Obj_floap_t){
            printf("ERROR set_pixel only take ount or floap as %dth arg");
            exit(1);
        }
    }
    x=args[0].type == Obj_ount_t ? *args[0].val.i : *args[0].val.f;
    y=args[1].type == Obj_ount_t ? *args[1].val.i : *args[1].val.f;
    r=args[2].type == Obj_ount_t ? *args[2].val.i : *args[2].val.f;
    g=args[3].type == Obj_ount_t ? *args[3].val.i : *args[3].val.f;
    b=args[4].type == Obj_ount_t ? *args[4].val.i : *args[4].val.f;
    #ifdef _WIN32
    HDC hdc = GetDC(g_hWnd);
    SetPixel(hdc, x, y, RGB(r,g,b));
    ReleaseDC(g_hWnd, hdc);
    #elif __linux__
    XSetForeground(display, DefaultGC(display, 0), r<<18 | g<<8 | b);
    XDrawPoint(display, window, DefaultGC(display, 0), x, y);
    #endif 
    return nil_Obj;
}  

Object set_title(Object* argv, int argc){

    if(argc != 1){
        printf("ERROR set_title only take 1 arg");
        exit(1);
    }
    if(argv[0].type != Obj_string_t){
        printf("ERROR set_title only take a string arg");
        exit(1);
    }
    #ifdef _WIN32

    HDC hdc = GetDC(g_hWnd);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // Copier les pixels de la fenêtre dans le bitmap pour enregistrer l'état d'origine
    BitBlt(hdcMem, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    SetWindowText(g_hWnd,argv[0].val.s);
    BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

    // Libérer les ressources
    SelectObject(hdcMem, hBitmapOld);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(g_hWnd, hdc);
    #endif

    return nil_Obj;

}

Object fill_window(Object* argv,int argc){
    #ifdef _WIN32
    
    if (argc != 3){
        printf("ERROR fill_ only takes 3 args");
        exit(1);
    } 
    int r=0;
    int g=0;
    int b=0;
    for(int i=0;i<3;i++){
        if(argv[i].type != Obj_ount_t && argv[i].type != Obj_floap_t){
            printf("ERROR set_pixel only take ount or floap as %dth arg");
            exit(1);
        }
    }
    r=argv[0].type == Obj_ount_t ? *argv[0].val.i : *argv[0].val.f;
    g=argv[1].type == Obj_ount_t ? *argv[1].val.i : *argv[1].val.f;
    b=argv[2].type == Obj_ount_t ? *argv[2].val.i : *argv[2].val.f;

    long long unsigned int color=RGB(r,g,b);
    HDC hdc = GetDC(g_hWnd);
    RECT rect;
    rect.left=0;
    rect.top=0;
    rect.bottom=height-1;
    rect.right=width-1;
    HBRUSH hBrush = CreateSolidBrush(color);

    // Sélectionner la brosse dans le contexte de dispositif
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // Remplir le rectangle avec la brosse sélectionnée
    Rectangle(hdc, 0, 0, width, height);

    // Sélectionner l'ancienne brosse dans le contexte de dispositif
    SelectObject(hdc, hOldBrush);

    // Détruire la brosse
    DeleteObject(hBrush);
    #endif
    return nil_Obj;
}


//x y w h r g b 
Object fill_rect(Object* argv, int argc){
    if (argc != 7){
        printf("ERROR fill_rect only takes 7 args");
        exit(1);
    } 
    int x=0;
    int y=0;
    int w=0;
    int h=0;
    int r=0;
    int g=0;
    int b=0;
    for(int i=0;i<7;i++){
        if(argv[i].type != Obj_ount_t && argv[i].type != Obj_floap_t){
            printf("ERROR fill_rect only take ount or floap as %dth arg");
            exit(1);
        }
    }
    x=argv[0].type == Obj_ount_t ? *argv[0].val.i : (int) *argv[0].val.f;
    y=argv[1].type == Obj_ount_t ? *argv[1].val.i : (int) *argv[1].val.f;
    w=argv[2].type == Obj_ount_t ? *argv[2].val.i : (int) *argv[2].val.f;
    h=argv[3].type == Obj_ount_t ? *argv[3].val.i : (int) *argv[3].val.f;
    r=argv[4].type == Obj_ount_t ? *argv[4].val.i : (int) *argv[4].val.f;
    g=argv[5].type == Obj_ount_t ? *argv[5].val.i : (int) *argv[5].val.f;
    b=argv[6].type == Obj_ount_t ? *argv[6].val.i : (int) *argv[6].val.f;

    if( w<0 ){
        printf("ERROR width can not be negative");
        exit(1);
    }
    if( h<0 ){
        printf("ERROR height can not be negative");
        exit(1);
    }

    #ifdef _WIN32
    long long unsigned int color=RGB(r,g,b);
    HDC hdc = GetDC(g_hWnd);
    int x2=x+w;
    int y2=y+h;
    RECT rect;
    rect.left=x;
    rect.top=y;
    rect.bottom=y2;
    rect.right=x2;
    HBRUSH hBrush = CreateSolidBrush(color);

    // Sélectionner la brosse dans le contexte de dispositif
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // Remplir le rectangle avec la brosse sélectionnée
    Rectangle(hdc, x, y, x2, y2);

    // Sélectionner l'ancienne brosse dans le contexte de dispositif
    SelectObject(hdc, hOldBrush);

    // Détruire la brosse
    DeleteObject(hBrush);
    #endif

}



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

    return mod;

}