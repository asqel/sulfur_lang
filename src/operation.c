#include "../include/memlib.h"
#include <stdlib.h>
#include "../include/utilities.h"


/*
ount | ount
string | string
floap | floap
boolean | boolean
*/
Object add(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        return new_ount(*a.val.i + *b.val.i);
    }
    if(a.type==Obj_string_t&&b.type==Obj_string_t){
        return new_string(str_cat_new(a.val.s, b.val.s));
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_floap(*a.val.f + *b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b + *b.val.b);

    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_floap(*a.val.f + *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_floap(*b.val.f + *a.val.i);
    }

    if(a.type==Obj_list_t && b.type==Obj_list_t){
        Object x;
        x.type=Obj_list_t;
        int len=*a.val.li->elements[0].val.i+*b.val.li->elements[0].val.i;
        x.val.li=malloc(sizeof(list));
        x.val.li->elements=malloc(sizeof(Object)*(len+1));
        x.val.li->elements[0].type=Obj_ount_t;
        x.val.li->elements[0].val.i=malloc(sizeof(long long int));
        *x.val.li->elements[0].val.i=len;
        int len1=*(a.val.li->elements[0].val.i);
        int len2=*(a.val.li->elements[0].val.i);
        for(int i=0;i<len1;i++){
            x.val.li->elements[i+1]=a.val.li->elements[i+1];
        }
        for(int i=0;i<len2;i++){
            x.val.li->elements[i+1+len1]=b.val.li->elements[i+1];
        }
        return x;
        

    }
    
    printf("ERROR : operation(+) between 2 types not supported\n");
    exit(1);
}

/*
ount | ount
floap | floap
boolean | boolean
*/
Object sub(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=*a.val.i-*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=*a.val.f-*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b-*b.val.b;
        return x;

    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_floap(*a.val.f - *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_floap(*a.val.i - *b.val.f );
    }
    printf("\n");
    Obj_print(a);
    Obj_print(b);
    printf("%lf\n",b.val.f);
    printf("ERROR : operation(-) between 2 types not supported %d %d\n",a.type,b.type);
    exit(1);
}


Object mul(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=*a.val.i*(*b.val.i);
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_floap((*a.val.f) * (*b.val.f));
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b*(*b.val.b);
        return x;

    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_floap(*a.val.f * *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_floap(*a.val.i * *b.val.f );
    }
    
    printf("ERROR : operation(*) between 2 types not supported\n");
    exit(1);
}

Object _div(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=(long double)*a.val.i/ (long double)*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_floap((*a.val.f) / (*b.val.f));
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b/ *b.val.b;
        return x;

    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_floap(*a.val.f / *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_floap(*a.val.i / *b.val.f );
    }
    printf("ERROR : operation(/) between 2 types not supported\n");
    exit(1);
}

Object _pow(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=pow(*a.val.i,*b.val.i);
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.f=malloc(sizeof(long double));
        *x.val.f=pow(*a.val.f,*b.val.f);
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=(int)pow(*a.val.b,*b.val.b);
        return x;

    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_floap(pow(*a.val.f, *b.val.i));
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_floap(pow(*a.val.i, *b.val.f));
    }
    printf("ERROR : operation(^) between 2 types not supported\n");
    exit(1);
}

Object less(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.i<*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.f<*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b<*b.val.b;
        return x;
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.f < *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.i < *b.val.f );
    }
    printf("ERROR : operation(<) between 2 types not supported\n");
    exit(1);
}

Object greater(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.i>*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.f>*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b>*b.val.b;
        return x;
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.f > *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.i > *b.val.f );
    }
    printf("ERROR : operation(>) between 2 types not supported\n");
    exit(1);
}

Object eq(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(*a.val.i==*b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(*a.val.f==*b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b == *b.val.b);
    }
    if(a.type == Obj_string_t && b.type == Obj_string_t){
        return new_boolean(!strcmp(a.val.s, b.val.s));
    }
    if(a.type == Obj_nil_t && b.type != Obj_nil_t){
        return new_boolean(0);
    }
    if(a.type != Obj_nil_t && b.type == Obj_nil_t){
        return new_boolean(0);
    }
    if(a.type == Obj_nil_t && b.type == Obj_nil_t){
        return new_boolean(1);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.f == *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.i == *b.val.f );
    }
    printf("ERROR : operation(==) between 2 types not supported\n");
    exit(1);
}

Object and(Object a,Object b){
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b && *b.val.b);
    }
    printf("ERROR : operation(&&) between 2 types not supported\n");
    exit(1);
}

Object negate(Object a){
    if(a.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=-*a.val.i;
        return x;
    }
    if(a.type==Obj_floap_t){
        Object x;
        x.type=Obj_floap_t;
        x.val.i=malloc(sizeof(long double));
        *x.val.i=-*a.val.f;
        return x;
    }
    if(a.type == Obj_boolean_t){
        return new_boolean(0 == *a.val.b);
    }
    printf("ERROR : operation(-(unary)) between 2 types not supported\n");
    exit(1);

}
Object not(Object a){
    if(a.type == Obj_boolean_t){
        return new_boolean(0 == *a.val.b);
    }
    printf("ERROR : operation(!(unary)) between 2 types not supported\n");
    exit(1);

}


Object mod(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=(*a.val.i%*b.val.i);
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_ount_t;
        x.val.i=malloc(sizeof(long long int));
        *x.val.i=(long long int)(*a.val.f)%(long long int)(*b.val.f);
        return x;
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_ount((long long int)*a.val.f % *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_ount(*a.val.i % (long long int)*b.val.f);
    }
    printf("ERROR : operation(^) between 2 types not supported\n");
    exit(1);
}



Object greater_eq(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.i>=*b.val.i;
        return x;
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.f>=*b.val.f;
        return x;
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        Object x;
        x.type=Obj_boolean_t;
        x.val.b=malloc(sizeof(short int));
        *x.val.b=*a.val.b>=*b.val.b;
        return x;
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.f >= *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.i >= *b.val.f );
    }
    printf("ERROR : operation(>) between 2 types not supported\n");
    exit(1);
}