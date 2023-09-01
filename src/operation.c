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
    if(a.type == Obj_ount_t&&b.type == Obj_ount_t)
        return new_ount(*a.val.i + *b.val.i);

    if(a.type == Obj_string_t&&b.type == Obj_string_t)
        return new_string(str_cat_new(a.val.s, b.val.s));

    if(a.type == Obj_floap_t && b.type == Obj_floap_t)
        return new_floap(*a.val.f + *b.val.f);

    if(a.type == Obj_boolean_t && b.type == Obj_boolean_t)
        return new_boolean(*a.val.b + *b.val.b);

    if(a.type == Obj_floap_t && b.type == Obj_ount_t)
        return new_floap(*a.val.f + *b.val.i);

    if(a.type == Obj_ount_t && b.type == Obj_floap_t)
        return new_floap(*b.val.f + *a.val.i);

    if(a.type == Obj_list_t && b.type == Obj_list_t){
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
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t)
        return new_ount(*a.val.i - *b.val.i);

    if(a.type==Obj_floap_t && b.type==Obj_floap_t)
        return new_floap(*a.val.f - *b.val.f);

    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t)
        return new_boolean(*a.val.b-*b.val.b);

    if(a.type == Obj_floap_t && b.type == Obj_ount_t)
        return new_floap(*a.val.f - *b.val.i);

    if(a.type == Obj_ount_t && b.type == Obj_floap_t)
        return new_floap(*a.val.i - *b.val.f );

    printf("ERROR : operation(-) between 2 types not supported %d %d\n",a.type,b.type);
    exit(1);
}


Object mul(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t)
        return new_ount(*a.val.i * (*b.val.i));

    if(a.type==Obj_floap_t && b.type==Obj_floap_t)
        return new_floap((*a.val.f) * (*b.val.f));

    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t)
        return new_boolean(*a.val.b * (*b.val.b));

    if(a.type == Obj_floap_t && b.type == Obj_ount_t)
        return new_floap(*a.val.f * (*b.val.i));

    if(a.type == Obj_ount_t && b.type == Obj_floap_t)
        return new_floap(*a.val.i * (*b.val.f));

    printf("ERROR : operation(*) between 2 types not supported\n");
    exit(1);
}

Object _div(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        if(*b.val.i == 0){
            printf("ERROR division by zero (ount)\n");
            exit(1);
        }
        return new_floap((long double)*a.val.i / (long double)*b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        if(*b.val.f == 0){
            printf("ERROR division by zero (floap)\n");
            exit(1);
        }
        return new_floap((*a.val.f) / (*b.val.f));
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(*b.val.i == 0){
            printf("ERROR division by zero (ount)\n");
            exit(1);
        }
        return new_floap(*a.val.f / *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if(*b.val.f == 0){
            printf("ERROR division by zero (floap)\n");
            exit(1);
        }
        return new_floap(*a.val.i / *b.val.f );
    }
    printf("ERROR : operation(/) between 2 types not supported\n");
    exit(1);
}

Object _pow(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        if(*a.val.i == 0 && *b.val.i == 0){
            return new_floap(1);
        }
        return new_floap(pow(*a.val.i,*b.val.i));
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        if(*a.val.f == 0 && *b.val.f == 0){
            return new_floap(1);
        }
        return new_floap(pow(*a.val.f,*b.val.f));
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(*a.val.f == 0 && *b.val.i == 0){
            return new_floap(1);
        }
        return new_floap(pow(*a.val.f, *b.val.i));
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if(*a.val.i == 0 && *b.val.f == 0){
            return new_floap(1);
        }
        return new_floap(pow(*a.val.i, *b.val.f));
    }
    printf("ERROR : operation(^) between 2 types not supported\n");
    exit(1);
}

Object less(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(*a.val.i < *b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(*a.val.f < *b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b < *b.val.b);
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
        return new_boolean(*a.val.i > *b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(*a.val.f > *b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b > *b.val.b);
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
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.i == *b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.f == *b.val.f);
    }
    if(a.type == Obj_boolean_t && b.type == Obj_boolean_t){
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
    if(a.type == Obj_list_t && b.type == Obj_list_t){
        if(a.val.li == b.val.li){
            return new_boolean(1);
        }
        int len_a = get_list_len(a);
        if(len_a != get_list_len(b)){
            return new_boolean(0);
        }
        int is_eq = 1;
        for(int i = 1; i <= len_a; i++){
            Object o = eq(a.val.li->elements[i], b.val.li->elements[i]);
            if(*o.val.b == 0){
                Obj_free_val(o);
                return new_boolean(0);
            }
        }
        return new_boolean(1);
    }
    return new_boolean(0);
    printf("ERROR : operation(==) between 2 types not supported\n");
    exit(1);
}

extern Object std_bool(Object* argv, int argc);

Object and(Object a,Object b){
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b && *b.val.b);
    }
    a = std_bool(&a, 1);
    b = std_bool(&b, 1);
    Object o = and(a, b);
    Obj_free_val(a);
    Obj_free_val(b);
    return o;
}

Object or(Object a,Object b){
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b || *b.val.b);
    }
    a = std_bool(&a, 1);
    b = std_bool(&b, 1);
    Object o = or(a, b);
    Obj_free_val(a);
    Obj_free_val(b);
    return o;
}

Object negate(Object a){
    if(a.type==Obj_ount_t){
        return new_ount(- *a.val.i);
    }
    if(a.type==Obj_floap_t){
        return new_floap(- *a.val.f);
    }
    if(a.type == Obj_boolean_t){
        return new_ount(- *a.val.b);
    }
    printf("ERROR : operation(-(unary)) between 2 types not supported\n");
    exit(1);
}
Object not(Object a){
    if(a.type == Obj_boolean_t){
        return new_boolean(0 == *a.val.b);
    }
    Object o = std_bool(&a, 1);
    o = not(o);
    return o;
}


Object mod(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        if(*b.val.i == 0){
            printf("ERROR modulo by 0 (ount)");
            exit(1);
        }
        return new_ount(*a.val.i%*b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        if(*b.val.f == 0){
            printf("ERROR modulo by 0 (floap)");
            exit(1);
        }
        return new_floap((long long int)(*a.val.f)%(long long int)(*b.val.f));
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(*b.val.i == 0){
            printf("ERROR modulo by 0 (ount)");
            exit(1);
        }
        return new_ount((long long int)*a.val.f % *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if((long long int)*b.val.f == 0){
            printf("ERROR modulo by 0 (floap)");
            exit(1);
        }
        return new_ount(*a.val.i % (long long int)*b.val.f);
    }
    printf("ERROR : operation(^) between 2 types not supported\n");
    exit(1);
}



Object greater_eq(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(*a.val.i >= *b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(*a.val.f >= *b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b >= *b.val.b);
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


Object less_eq(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(*a.val.i <= *b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(*a.val.f <= *b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(*a.val.b <= *b.val.b);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.f <= *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.i <= *b.val.f );
    }
    printf("ERROR : operation(>) between 2 types not supported\n");
    exit(1);
}


Object not_eq(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.i != *b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.f != *b.val.f);
    }
    if(a.type == Obj_boolean_t && b.type == Obj_boolean_t){
        return new_boolean(*a.val.b != *b.val.b);
    }
    if(a.type == Obj_string_t && b.type == Obj_string_t){
        return new_boolean(strcmp(a.val.s, b.val.s));
    }
    if(a.type == Obj_nil_t && b.type != Obj_nil_t){
        return new_boolean(1);
    }
    if(a.type != Obj_nil_t && b.type == Obj_nil_t){
        return new_boolean(1);
    }
    if(a.type == Obj_nil_t && b.type == Obj_nil_t){
        return new_boolean(0);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(*a.val.f != *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(*a.val.i != *b.val.f );
    }
    if(a.type == Obj_list_t && b.type == Obj_list_t){
        if(a.val.li == b.val.li){
            return new_boolean(0);
        }
        int len_a = get_list_len(a);
        if(len_a != get_list_len(b)){
            return new_boolean(1);
        }
        int is_eq = 1;
        for(int i = 1; i <= len_a; i++){
            Object o = eq(a.val.li->elements[i], b.val.li->elements[i]);
            if(*o.val.b != 0){
                Obj_free_val(o);
                return new_boolean(1);
            }
        }
        return new_boolean(0);
    }
    return new_boolean(0);
    printf("ERROR : operation(!=) between 2 types not supported\n");
    exit(1);
}


Object fl_div(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        if(*b.val.i == 0){
            printf("ERROR floor division by zero (ount)\n");
            exit(1);
        }
        return new_ount(*a.val.i / *b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        if(*b.val.f == 0){
            printf("ERROR floor division by zero (floap)\n");
            exit(1);
        }
        return new_ount((*a.val.f) / (*b.val.f));
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(*b.val.i == 0){
            printf("ERROR floor division by zero (ount)\n");
            exit(1);
        }
        return new_ount(*a.val.f / *b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if(*b.val.f == 0){
            printf("ERROR floor division by zero (floap)\n");
            exit(1);
        }
        return new_ount(*a.val.i / *b.val.f );
    }
    printf("ERROR : operation(\\) between 2 types not supported\n");
    exit(1);

}