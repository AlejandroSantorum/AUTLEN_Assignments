#include "utils.h"

void sq_mtx_mult(int **mtxA, int **mtxB, int **res, size_t size){
    if (!mtxA || !mtxB || !res){
        return;
    }
    int **res_tmp = sq_mtx_new(size);
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            for (size_t k = 0; k < size; k++) {
                res_tmp[i][j] += mtxA[i][k] * mtxB[k][j];
            }
        }
    }

    sq_mtx_cpy(res, res_tmp, size);
    sq_mtx_delete(res_tmp, size);

}

void sq_mtx_power (int **mtx, int **res, size_t size, size_t power) {
    if (!mtx || !res){
        return;
    }
    //Initialize res to identity matrix
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (i - j){
                res[i][j] = 0;
            }else{
                res[i][j] = 1;
            }
        }
    }
    //Multiply
    for (size_t i = 1; i <= power; i++) {
        sq_mtx_mult(mtx, res, res, size);
    }
}

int **sq_mtx_new(size_t sz){
    int **mtx;
    mtx = calloc(sz, sizeof(int *));
    for (size_t i = 0; i < sz; i++) {
        mtx[i] = calloc(sz, sizeof(int));
    }
    return mtx;
}

void sq_mtx_delete(int **mtx, size_t sz){
    if (mtx) {
        for (size_t i = 0; i < sz; i++) {
            if (mtx[i]){
                free(mtx[i]);
                mtx[i] = NULL;
            }
        }
        free(mtx);
        mtx = NULL;
    }
}

void sq_mtx_cpy(int **dest, int **src, size_t sz){
    if(!dest || !src){
        return;
    }
    for (size_t i = 0; i < sz; i++) {
        memcpy(dest[i], src[i], sz * sizeof(int));
    }
}

int fprintf_sq_mtx(FILE *f, int **mtx, size_t sz) {
    if (!f || !mtx) return 0;
    int chars = 0;
    chars += fprintf(f, "\n\n------- MTX --------\n");
    for (size_t i = 0; i < sz; i++) {
        for (size_t j = 0; j < sz; j++) {
            chars += fprintf(f, "%d\t", mtx[i][j]);
        }
        chars += fprintf(f, "\n");
    }
    chars += fprintf(f, "--------------------\n\n");

    return chars;
}
