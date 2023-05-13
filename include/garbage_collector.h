


typedef struct ref{
    short int type;
    int count;
    void* obj_val_p;
}ref;   


typedef struct ref_counter{
    int len;
    ref* REFS;
}ref_counter;

extern ref_counter* REF_COUNT;

void init_garbage();