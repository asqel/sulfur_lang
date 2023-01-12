
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int root;
    struct node*right;
    struct node*left;
}node;

void list(node x){

}
node n_node(int ro,node r,node l){
    node x;
    x.left=malloc(sizeof(node));
    x.right=malloc(sizeof(node));
    *x.left=l;
    *x.right=r;
    return x;
}
node n_node(int ro,void* r,node l){
    node x;
    x.left=malloc(sizeof(node));
    *x.left=l;
    x.right=r;
    return x;
}
node n_node(int ro,node r,void* l){
    node x;
    x.right=malloc(sizeof(node));
    x.left=l;
    *x.right=r;
    return x;
}


node*n_node_ptr(int ro,node r,node l){
    
    
}







int main(){
    n_node(1,NULL,NULL);
}