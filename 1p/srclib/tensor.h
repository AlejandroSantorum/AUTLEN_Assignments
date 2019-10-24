#ifndef TENSOR
#define TENSOR

typedef struct _tensor_dim1 {
    int re_sz;
    void **list;
} Tensor_dim1;

typedef struct _tensor_dim2 {
    int re_sz;
    int fix_sz;
    void ***mtx;
} Tensor_dim2;

Tensor_dim1 *td1_create(int initial_sz);
void td1_delete(Tensor_dim1 *td1);
Tensor_dim1 *td1_add(void * elem);


Tensor_dim2 *td2_create(int initial_sz, int fix_sz);
void td2_delete(Tensor_dim2 *td2);
Tensor_dim2 *td2_add(void **elem);

#endif
