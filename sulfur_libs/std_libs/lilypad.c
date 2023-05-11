//this is the lib to manage memory
#include "../../include/memlib.h"
#include <stdlib.h>
#include <stdio.h>


Object std_malloc(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    return new_ount(malloc(*argv[0].val.i));
}
Object std_free(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    free(*argv[0].val.i);
    return nil_Obj;
}

Object std_get_val_byte(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    return new_ount(*((unsigned char *)*argv[0].val.i));
}
Object std_set_val_byte(Object* argv, int argc){
    if(argc != 2){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t, argv[1].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    *((unsigned char *)*argv[0].val.i) = *argv[1].val.i;
    return nil_Obj;
}

Object donut(Object* argv, int argc){
         int k;float A=
       0,B=0,i,j,z[1760];char b[
     1760];printf("\x1b[2J");while(1 
  ){memset(b,32,1760);memset(z,0,7040)
  ;for(j=0;6.28>j;j+=0.07)for(i=0;6.28
 >i;i+=0.02){float c=sin(i),d=cos(j),e=
 sin(A),f=sin(j),g=cos(A),h=d+2,D=1/(c*
 h*e+f*g+5),l=cos      (i),m=cos(B),n=s\
in(B),t=c*h*g-f*        e;int x=40+30*D*
(l*h*m-t*n),y=            12+15*D*(l*h*n
+t*m),o=x+80*y,          N=8*((f*e-c*d*g
 )*m-c*d*e-f*g-l        *d*n);if(22>y&&
 y>0&&x>0&&80>x&&D>z[o]){z[o]=D;;;b[o]=
 ".,-~:;=!*#$@"[N>0?N:0];}}/*#****!!-*/
  printf("\x1b[H");for(k=0;1761>k;k++)
   putchar(k%80?b[k]:10);A+=0.04;B+=
     0.02;}/*****####*******!!=;:~
       ~::==!!!**********!!!==::-
         .,~~;;;========;;;:~-.
             ..,--------,*/
    return nil_Obj;
}


Object __loader() {
    Object mod = new_Module();

    add_func_Module(mod, "free", &std_free, "");
    add_func_Module(mod, "malloc", &std_malloc, "");
    add_func_Module(mod,"get_byte", &std_get_val_byte,"");
    add_func_Module(mod,"set_byte", &std_set_val_byte,"");
    add_func_Module(mod,"donut", &donut,"");

    return mod;
}