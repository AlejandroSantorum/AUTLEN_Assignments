#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include "current_state.h"

#define STATE_NAME 'q'

struct _cstate{
    size_t nstates;
    uint8_t *states;
};


cstate *cstate_create(size_t nstates){
    cstate *cst=NULL;

    cst = (cstate*) calloc(1, sizeof(cstate));
    if (!cst) { /* Memory error */
        perror("Unable to allocate memory for cstate\n");
        return NULL;
    }

    cst->states = (uint8_t*) calloc(nstates, sizeof(uint8_t));
    if (!(cst->states)) {
        perror("Unable to allocate memory for cstate states\n");
        return NULL;
    }

    cst->nstates = nstates;
    return cst;
}


void cstate_remove(cstate *cst){
    if (cst) {
        if (cst->states){
            free(cst->states);
        }
        free(cst);
    }
}


cstate *cstate_add_state(cstate *cst, int index){
    if (!cst) {
        perror("cstate NULL pointer\n");
        return NULL;
    }
    if (index < 0 || index >= cst->nstates) {
        perror("Invalid index value\n");
        return NULL;
    }
    if (!(cst->states)) {
        perror("cstate states NULL pointer\n");
        return NULL;
    }

    /* Setting transition */
    cst->states[index] = 1;
    return cst;
}


void cstate_to_string(cstate *cst, char *string){
    size_t i;
    char *cst_str;

    if (!cst) {
        perror("cstate NULL pointer\n");
        return;
    }
    if (!string) {
        perror("string NULL pointer\n");
        return;
    }

    cst_str = string;
    for (i = 0; i < cst->nstates; i++) {
        if (cst->states[i]) {
            *cst_str = STATE_NAME;
            cst_str++;
            cst_str += sprintf(cst_str, "%lu", i);
        }
    }
}
