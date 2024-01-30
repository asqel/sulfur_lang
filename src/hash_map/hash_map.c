#include "../../include/hash_map/hash_map.h"
#include "../../include/memlib.h"
#include "../../include/hash_map/linked_list.h"


#define hash_map_len 10000

// if .name == NULL it means the node isn't set
Node hash_map[hash_map_len] = {0};

S_sulfur_int hash_str(char *str) {
    S_sulfur_int res = 0;
    for(int i = 0; str[i] != '\0'; i++)
        res += str[i] * 31;
    return res % hash_map_len;
}



