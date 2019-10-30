#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "afnd.h"

// Utilities for squared matrix calculations
void sq_mtx_mult(uint8_t **mtxA, uint8_t **mtxB, uint8_t **res, size_t size);
void sq_mtx_power(uint8_t **mtx, uint8_t **res, size_t size, size_t power);
uint8_t **sq_mtx_new(size_t sz);
void sq_mtx_delete(uint8_t **mtx, size_t sz);
void sq_mtx_cpy(uint8_t **dest, uint8_t **src, size_t sz);
int fprintf_sq_mtx(FILE *f, uint8_t **mtx, size_t sz);

//Utilities to translate from afnd.h inner structures to ours.
char **get_alphabet(AFND *afnd, size_t *arr_sz);
uint8_t ***get_first_transition_table(AFND *afnd);
void delete_first_transition_table(uint8_t ***trans_tb, size_t nstates, size_t nsym);
uint8_t **get_lambda_clausure(AFND *afnd, size_t *tb_sz);

//Utilities to translate from our structures to afnd.h structures
