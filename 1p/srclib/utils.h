#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "afnd.h"

void sq_mtx_mult(int **mtxA, int **mtxB, int **res, size_t size);
void mtx_power (int **mtx, int **res, size_t size, size_t power);
