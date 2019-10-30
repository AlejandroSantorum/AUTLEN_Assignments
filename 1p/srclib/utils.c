#include "utils.h"

void sq_mtx_mult(int **mtxA, int **mtxB, int **res, size_t size){
    if (!mtxA || !mtxB || !res){
        return;
    }
    for (size_t i = 0; i < size; i++) {
        memset(res[i], 0, size * sizeof(int));
        for (size_t j = 0; j < size; j++) {
            res[i][j] += mtxA[i][j] * mtxB[j][i];
        }
    }
}

void mtx_power (int **mtx, int **res, size_t size, size_t power) {
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
        sq_mtx_mult(mtx, mtx, res, size);
    }
}
