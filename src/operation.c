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
    if(a.type == Obj_ount_t && b.type == Obj_ount_t)
        return new_ount(a.val.i + b.val.i);

    if (a.type == Obj_string_t && b.type == Obj_string_t) {
        char *s = str_cat_new(a.val.s, b.val.s);
        Object res = new_string(s);
        free(s);
        return res;
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t)
        return new_floap(a.val.f + b.val.f);

    if(a.type == Obj_boolean_t && b.type == Obj_boolean_t)
        return new_boolean(a.val.b + b.val.b);

    if(a.type == Obj_floap_t && b.type == Obj_ount_t)
        return new_floap(a.val.f + b.val.i);

    if(a.type == Obj_ount_t && b.type == Obj_floap_t)
        return new_floap(b.val.f + a.val.i);

    if(a.type == Obj_list_t && b.type == Obj_list_t){
        Object x;
        x.type=Obj_list_t;
        int len = a.val.li->elements[0].val.i + b.val.li->elements[0].val.i;
        x.val.li=malloc(sizeof(list));
        x.val.li->elements=malloc(sizeof(Object)*(len+1));
        x.val.li->elements[0] = new_ount(len);
        int len1 = a.val.li->elements[0].val.i;
        int len2 = a.val.li->elements[0].val.i;
        for(int i=0;i<len1;i++){
            x.val.li->elements[i+1]=a.val.li->elements[i+1];
        }
        for(int i=0;i<len2;i++){
            x.val.li->elements[i+1+len1]=b.val.li->elements[i+1];
        }
        return x;
        
        

    }
    //complex
    if (a.type == Obj_complex_t && b.type == Obj_ount_t)
        return new_complex(b.val.i + a.val.c[0], a.val.c[1]);

    if (b.type == Obj_complex_t && a.type == Obj_ount_t)
        return new_complex(a.val.i + b.val.c[0], b.val.c[1]);

    if (a.type == Obj_complex_t && b.type == Obj_floap_t)
        return new_complex(b.val.f + a.val.c[0], a.val.c[1]);

    if (b.type == Obj_complex_t && a.type == Obj_floap_t)
        return new_complex(a.val.f + b.val.c[0], b.val.c[1]);

    if (a.type == Obj_complex_t && b.type == Obj_complex_t)
        return new_complex(a.val.c[0] + b.val.c[1], a.val.c[1] + b.val.c[1]);

    
    printf("ERROR : operation(+) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

/*
ount | ount
floap | floap
boolean | boolean
*/
Object sub(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t)
        return new_ount(a.val.i - b.val.i);

    if(a.type==Obj_floap_t && b.type==Obj_floap_t)
        return new_floap(a.val.f - b.val.f);

    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t)
        return new_boolean(a.val.b - b.val.b);

    if(a.type == Obj_floap_t && b.type == Obj_ount_t)
        return new_floap(a.val.f - b.val.i);

    if(a.type == Obj_ount_t && b.type == Obj_floap_t)
        return new_floap(a.val.i - b.val.f );


    if (a.type == Obj_complex_t && b.type == Obj_ount_t)
        return new_complex(a.val.c[0] - b.val.i, a.val.c[1]);

    if (a.type == Obj_ount_t && b.type == Obj_complex_t)
        return new_complex(a.val.i - b.val.c[0], b.val.c[1]);

    if (a.type == Obj_complex_t && b.type == Obj_floap_t)
        return new_complex(a.val.c[0] - b.val.f, a.val.c[1]);

    if (a.type == Obj_floap_t && b.type == Obj_complex_t)
        return new_complex(a.val.f - b.val.c[0], b.val.c[1]);

    if (a.type == Obj_complex_t && b.type == Obj_complex_t)
        return new_complex(a.val.c[0] + b.val.c[1], a.val.c[1] + b.val.c[1]);


    printf("ERROR : operation(-) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}


Object mul(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t)
        return new_ount(a.val.i * b.val.i);

    if(a.type==Obj_floap_t && b.type==Obj_floap_t)
        return new_floap(a.val.f * b.val.f);

    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t)
        return new_boolean(a.val.b * b.val.b);

    if(a.type == Obj_floap_t && b.type == Obj_ount_t)
        return new_floap(a.val.f * b.val.i);

    if(a.type == Obj_ount_t && b.type == Obj_floap_t)
        return new_floap(a.val.i * b.val.f);


    if (a.type == Obj_complex_t && b.type == Obj_ount_t)
        return new_complex(a.val.c[0] * b.val.i, a.val.c[1] * b.val.i);

    if (a.type == Obj_ount_t && b.type == Obj_complex_t)
        return new_complex(a.val.i * b.val.c[0], a.val.i * b.val.c[1]);

    if (a.type == Obj_complex_t && b.type == Obj_floap_t)
        return new_complex(a.val.c[0] * b.val.f, a.val.c[1] * b.val.f);

    if (a.type == Obj_floap_t && b.type == Obj_complex_t)
        return new_complex(a.val.f * b.val.c[0], b.val.c[1] * a.val.f);

    if (a.type == Obj_complex_t && b.type == Obj_complex_t)
        return new_complex(
            a.val.c[0] * b.val.c[0] - a.val.c[1] * b.val.c[1],
            a.val.c[0] * b.val.c[1] + a.val.c[1] * b.val.c[0]
        );

    printf("ERROR : operation(*) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

Object _div(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        if(b.val.i == 0){
            printf("ERROR division by zero (ount)\n");
            exit(1);
        }
        return new_floap((long double)a.val.i / (long double)b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        if(b.val.f == 0){
            printf("ERROR division by zero (floap)\n");
            exit(1);
        }
        return new_floap(a.val.f / b.val.f);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(b.val.i == 0){
            printf("ERROR division by zero (ount)\n");
            exit(1);
        }
        return new_floap(a.val.f / b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if(b.val.f == 0){
            printf("ERROR division by zero (floap)\n");
            exit(1);
        }
        return new_floap(a.val.i / b.val.f );
    }


    if (a.type == Obj_complex_t && b.type == Obj_ount_t) {
        if(b.val.i == 0){
            printf("ERROR division by zero (ount)\n");
            exit(1);
        }
        return new_complex(a.val.c[0] * b.val.i, a.val.c[1] * b.val.i);
    }

    if (a.type == Obj_ount_t && b.type == Obj_complex_t) {
        if(b.val.c[0] == 0 && b.val.c[1] == 0){
            printf("ERROR division by zero (complex)\n");
            exit(1);
        }
        return new_complex(
            a.val.i * b.val.c[0] / (b.val.c[0] * b.val.c[0] + b.val.c[1] * b.val.c[1]),
            -a.val.i * b.val.c[1] / (b.val.c[0] * b.val.c[0] + b.val.c[1] * b.val.c[1])
        );
    }

    if (a.type == Obj_complex_t && b.type == Obj_floap_t) {
        return new_complex(a.val.c[0] / b.val.f, a.val.c[1] / b.val.f);
    }

    if (a.type == Obj_floap_t && b.type == Obj_complex_t) {
        if(b.val.c[0] == 0 && b.val.c[1] == 0){
            printf("ERROR division by zero (complex)\n");
            exit(1);
        }
        return new_complex(
            a.val.f * b.val.c[0] / (b.val.c[0] * b.val.c[0] + b.val.c[1] * b.val.c[1]),
            -a.val.f * b.val.c[1] / (b.val.c[0] * b.val.c[0] + b.val.c[1] * b.val.c[1])
        );
    }

    if (a.type == Obj_complex_t && b.type == Obj_complex_t) {
        float _a = a.val.c[0];
        float _b = a.val.c[1];
        float _c = b.val.c[0];
        float _d = b.val.c[1];
        if(_c == 0 && _d == 0){
            printf("ERROR division by zero (complex)\n");
            exit(1);
        }
        return new_complex(
            (_a*_c + _b*_d) / (_c*_c + _d*_d),
            (_b*_c - _a*_d) / (_c*_c + _d*_d)
        );
    }

    printf("ERROR : operation(/) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

Object _pow(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        if(a.val.i == 0 && b.val.i == 0){
            return new_floap(1);
        }
        return new_floap(sulfur_pow(a.val.i,b.val.i));
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        if(a.val.f == 0 && b.val.f == 0){
            return new_floap(1);
        }
        return new_floap(sulfur_pow(a.val.f,b.val.f));
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(a.val.f == 0 && b.val.i == 0){
            return new_floap(1);
        }
        return new_floap(sulfur_pow(a.val.f, b.val.i));
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if(a.val.i == 0 && b.val.f == 0){
            return new_floap(1);
        }
        return new_floap(sulfur_pow(a.val.i, b.val.f));
    }
    printf("ERROR : operation(^) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

Object less(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(a.val.i < b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(a.val.f < b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(a.val.b < b.val.b);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(a.val.f < b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(a.val.i < b.val.f );
    }
    printf("ERROR : operation(<) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

Object greater(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(a.val.i > b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(a.val.f > b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(a.val.b > b.val.b);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(a.val.f > b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(a.val.i > b.val.f );
    }
    printf("ERROR : operation(>) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

Object eq(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        return new_boolean(a.val.i == b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        return new_boolean(a.val.f == b.val.f);
    }
    if(a.type == Obj_boolean_t && b.type == Obj_boolean_t){
        return new_boolean(a.val.b == b.val.b);
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
        return new_boolean(a.val.f == b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(a.val.i == b.val.f );
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
            if(o.val.b == 0){
                Obj_free_val(o);
                return new_boolean(0);
            }
        }
        return new_boolean(1);
    }
    return new_boolean(0);
}

extern Object std_bool(Object* argv, int argc);

Object and(Object a,Object b){
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(a.val.b && b.val.b);
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
        return new_boolean(a.val.b || b.val.b);
    }
    a = std_bool(&a, 1);
    b = std_bool(&b, 1);
    Object o = or(a, b);
    Obj_free_val(a);
    Obj_free_val(b);
    return o;
}

Object lshift(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        return new_ount(a.val.i << b.val.i);
    }
    printf("ERROR : operation(<<) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}

Object rshift(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        return new_ount(a.val.i >> b.val.i);
    }
    printf("ERROR : operation(>>) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}



Object negate(Object a){
    if(a.type==Obj_ount_t){
        return new_ount(-a.val.i);
    }
    if(a.type==Obj_floap_t){
        return new_floap(-a.val.f);
    }
    if(a.type == Obj_boolean_t){
        return new_ount(-a.val.b);
    }
    printf("ERROR : operation(-(unary)) not supported for type %s", Obj_type_as_str(a.type));
    exit(1);
}
Object not(Object a){
    if(a.type == Obj_boolean_t){
        return new_boolean(!a.val.b);
    }
    Object o = std_bool(&a, 1);
    o.val.b = !o.val.b;
    return o;
}


Object mod(Object a,Object b){
    if(a.type==Obj_ount_t&&b.type==Obj_ount_t){
        if(b.val.i == 0){
            printf("ERROR modulo by 0 (ount)\n");
            exit(1);
        }
        return new_ount(a.val.i % b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        if(b.val.f == 0){
            printf("ERROR modulo by 0 (floap)\n");
            exit(1);
        }
        return new_floap((long long int)a.val.f % (long long int)b.val.f);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(b.val.i == 0){
            printf("ERROR modulo by 0 (ount)\n");
            exit(1);
        }
        return new_ount((long long int)a.val.f % b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if((long long int)b.val.f == 0){
            printf("ERROR modulo by 0 (floap)\n");
            exit(1);
        }
        return new_ount(a.val.i % (long long int)b.val.f);
    }
    printf("ERROR : operation(%) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}



Object greater_eq(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(a.val.i >= b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(a.val.f >= b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(a.val.b >= b.val.b);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(a.val.f >= b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(a.val.i >= b.val.f );
    }
    printf("ERROR : operation(>=) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}


Object less_eq(Object a,Object b){
    if(a.type==Obj_ount_t && b.type==Obj_ount_t){
        return new_boolean(a.val.i <= b.val.i);
    }
    if(a.type==Obj_floap_t && b.type==Obj_floap_t){
        return new_boolean(a.val.f <= b.val.f);
    }
    if(a.type==Obj_boolean_t && b.type==Obj_boolean_t){
        return new_boolean(a.val.b <= b.val.b);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        return new_boolean(a.val.f <= b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(a.val.i <= b.val.f );
    }
    printf("ERROR : operation(<=) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);
}


Object not_eq(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        return new_boolean(a.val.i != b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        return new_boolean(a.val.f != b.val.f);
    }
    if(a.type == Obj_boolean_t && b.type == Obj_boolean_t){
        return new_boolean(a.val.b != b.val.b);
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
        return new_boolean(a.val.f != b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        return new_boolean(a.val.i != b.val.f );
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
            if(o.val.b != 0){
                Obj_free_val(o);
                return new_boolean(1);
            }
        }
        return new_boolean(0);
    }
    return new_boolean(1);
}


Object fl_div(Object a,Object b){
    if(a.type == Obj_ount_t && b.type == Obj_ount_t){
        if(b.val.i == 0){
            printf("ERROR floor division by zero (ount)\n");
            exit(1);
        }
        return new_ount(a.val.i / b.val.i);
    }
    if(a.type == Obj_floap_t && b.type == Obj_floap_t){
        if(b.val.f == 0){
            printf("ERROR floor division by zero (floap)\n");
            exit(1);
        }
        return new_ount(a.val.f / b.val.f);
    }
    if(a.type == Obj_floap_t && b.type == Obj_ount_t){
        if(b.val.i == 0){
            printf("ERROR floor division by zero (ount)\n");
            exit(1);
        }
        return new_ount(a.val.f / b.val.i);
    }
    if(a.type == Obj_ount_t && b.type == Obj_floap_t){
        if(b.val.f == 0){
            printf("ERROR floor division by zero (floap)\n");
            exit(1);
        }
        return new_ount(a.val.i / b.val.f );
    }
    printf("ERROR : operation(\\) between 2 types not supported %s %s\n",
        Obj_type_as_str(a.type), Obj_type_as_str(b.type));
    exit(1);

}