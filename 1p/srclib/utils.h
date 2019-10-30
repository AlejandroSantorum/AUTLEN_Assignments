#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "afnd.h"

void sq_mtx_mult(int **mtxA, int **mtxB, int **res, size_t size);
void sq_mtx_power(int **mtx, int **res, size_t size, size_t power);
int **sq_mtx_new(size_t sz);
void sq_mtx_delete(int **mtx, size_t sz);
void sq_mtx_cpy(int **dest, int **src, size_t sz);
int fprintf_sq_mtx(FILE *f, int **mtx, size_t sz);
