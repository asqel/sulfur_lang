#include "../../include/hash_map/linked_list.h"
#include <stdlib.h>


inline Node *node_get_next(Node *x) {
	return x->next ? x->next : NULL;
}

// if idx == 0, x will be returned
// return NULL if there is no node at idx and store in res the last node non NULL
Node *node_get_element(Node *x, int idx, int *res) {
	Node *current = x;
	for (int i = 0; i < idx; i++) {
		current = node_get_next(current);
		if (current == NULL) {
			if (res != NULL)
				*res = i - 1;
			return NULL;
		}
	}
	return current;
}