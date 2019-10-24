#include "tensor.h"

Tensor_dim1 *td1_create(int initial_sz){
    Tensor_dim1 *td1 = calloc(1, sizeof(Tensor_dim1));
    td1->re_sz = initial_sz;
    td1->list = calloc(initial_sz, sizeof(void *));
    return td1;
}

void td1_delete(Tensor_dim1 *td1);
Tensor_dim1 *td1_add(void * elem);


Tensor_dim2 *td2_create(int initial_sz, int fix_sz);
void td2_delete(Tensor_dim2 *td2);
Tensor_dim2 *td2_add(void **elem);
