#include "utils.h"



/*
 *
 * Utilities to make some operations with squared matrix
 *
 */

void sq_mtx_mult(uint8_t **mtxA, uint8_t **mtxB, uint8_t **res, size_t size){
    if (!mtxA || !mtxB || !res){
        return;
    }
    uint8_t **res_tmp = sq_mtx_new(size);
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

void sq_mtx_power (uint8_t **mtx, uint8_t **res, size_t size, size_t power) {
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

uint8_t **sq_mtx_new(size_t sz){
    uint8_t **mtx;
    mtx = calloc(sz, sizeof(uint8_t *));
    for (size_t i = 0; i < sz; i++) {
        mtx[i] = calloc(sz, sizeof(uint8_t));
    }
    return mtx;
}

void sq_mtx_delete(uint8_t **mtx, size_t sz){
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

void sq_mtx_cpy(uint8_t **dest, uint8_t **src, size_t sz){
    if(!dest || !src){
        return;
    }
    for (size_t i = 0; i < sz; i++) {
        memcpy(dest[i], src[i], sz * sizeof(uint8_t));
    }
}

int fprintf_sq_mtx(FILE *f, uint8_t **mtx, size_t sz) {
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


/*
 *
 * Utilities to translate from the structures in afnd.h to a more simplified
 * structure that we know better.
 *
 */

char **get_alphabet(AFND *afnd, size_t *arr_sz){
    if (!afnd) {
        return NULL;
    }
    *arr_sz = AFNDNumSimbolos(afnd);
    char **alphabet = calloc(*arr_sz, sizeof(char *));
    for (size_t i = 0; i < *arr_sz; i++) {
        alphabet[i] = AFNDSimboloEn(afnd, i);
    }
    return alphabet;
}

uint8_t ***get_first_transition_table(AFND *afnd){
    size_t nstates = AFNDNumEstados(afnd);
    size_t nsym = AFNDNumSimbolos(afnd);

    uint8_t ***trans_tb = calloc(nstates, sizeof(uint8_t **));
    for (size_t i = 0; i < nstates; i++) {
        trans_tb[i] = calloc(nsym, sizeof(uint8_t *));
        for (size_t j = 0; j < nsym; j++) {
            trans_tb[i][j] = calloc(nstates, sizeof(uint8_t));
        }
    }

    for (size_t i = 0; i < nstates; i++) {
        for (size_t j = 0; j < nsym; j++) {
            for (size_t k = 0; k < nstates; k++) {
                trans_tb[i][j][k] = AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k);
            }
        }
    }

    return trans_tb;
}

void delete_first_transition_table(uint8_t ***trans_tb, size_t nstates, size_t nsym){
    if(!trans_tb) {
        return;
    }
    for (size_t i = 0; i < nstates; i++) {
        if (!trans_tb[i]){
            continue;
        }
        for (size_t j = 0; j < nsym; j++) {
            if (trans_tb[i][j]) {
                free(trans_tb[i][j]);
                trans_tb[i][j] = NULL;
            }
        }
        free(trans_tb[i]);
        trans_tb[i] = NULL;
    }
    free(trans_tb);
    trans_tb = NULL;
}

uint8_t **get_lambda_clausure(AFND *afnd, size_t *tb_sz){
    AFND *c_afnd = AFNDCierraLTransicion(afnd);
    size_t nstates = AFNDNumEstados(c_afnd);
    *tb_sz = nstates;
    uint8_t **lambda_cl = sq_mtx_new(nstates);

    for (size_t i = 0; i < nstates; i++) {
        for (size_t j = 0; j < nstates; j++) {
            lambda_cl[i][j] = AFNDCierreLTransicionIJ(afnd, i, j);
        }
    }

    return lambda_cl;
}
