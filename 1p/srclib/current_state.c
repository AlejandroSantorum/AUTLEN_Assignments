#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "current_state.h"
#include "afnd.h"

#define STATE_NAME 'q'

#pragma pack(1)
struct _cstate{
    size_t nstates;
    uint8_t is_final;
    uint8_t is_initial;
    uint8_t *states;
};

size_t cstate_size(){
    return sizeof(struct _cstate);
}

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

cstate *cstate_new() {
    return calloc(1, sizeof(cstate));
}

cstate *cstate_init(cstate *c, uint8_t *states) {
    if (!c || !states){
        return NULL;
    }
    memcpy(c->states, states, c->nstates * sizeof(uint8_t));
    return c;
}


void cstate_remove(cstate *cst){
    if (cst) {
        if (cst->states){
            free(cst->states);
        }
        free(cst);
    }
}

void cstate_delete(cstate *cst){
    if (cst) free(cst);
}

void cstate_destroy(cstate *cst){
    if (cst) free (cst->states);
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

uint8_t *cstate_get_states(cstate *cst){
    if (cst) return cst->states;
    return NULL;
}

cstate *cstate_or_cstate(cstate *dst, cstate *src){
    if (!dst || !src){
        perror("cstate_or_cstate: NULL pointer\n");
        return NULL;
    }
    if (!dst->states || !src->states){
        perror("cstate_or_cstate: No states");
        return NULL;
    }
    for (size_t i = 0; i < dst->nstates; dst->states[i] |= src->states[i], i++);
    return dst;
}


void cstate_to_string(cstate *cst, char *string, size_t str_sz){
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

    memset(string, 0, str_sz * sizeof(char));
    cst_str = string;
    for (i = 0; i < cst->nstates; i++) {
        if (cst->states[i]) {
            *cst_str = STATE_NAME;
            cst_str++;
            cst_str += sprintf(cst_str, "%lu", i);
        }
    }
}


int cstate_cmp(cstate *c1, cstate *c2){
    return memcmp(c1->states, c2->states, sizeof(uint8_t) * c1->nstates);
}


int cstate_cmp_v(void *c1_v, void *c2_v){
    cstate *c1 = (cstate *) c1_v;
    cstate *c2 = (cstate *) c2_v;
    return memcmp(c1->states, c2->states, sizeof(uint8_t) * c1->nstates);
}

uint8_t cstate_get_type(cstate *cst){
    if (!cst) return NORMAL;
    if (cst->is_final && cst->is_initial) return INICIAL_Y_FINAL;
    if (cst->is_final) return FINAL;
    if (cst->is_initial) return INICIAL;
    return NORMAL;
}

cstate *cstate_set_initial(cstate *cs){
    if (cs) {
        cs->is_initial = 1;
        return cs;
    }
    return NULL;
}

cstate *cstate_set_final(cstate *cs){
    if (cs) {
        cs->is_final = 1;
        return cs;
    }
    return NULL;
}

uint8_t cstate_is_final(cstate *cs, uint8_t *final_states){
    if (!cs || !final_states){
        return 0;
    }

    for (size_t i = 0; i < cs->nstates; i++) {
        if (cs->states[i] && final_states[i]){
            return 1;
        }
    }

    return 0;
}

uint8_t cstate_is_valid(cstate *cs){
    if(!cs) return 0;
    for (size_t i = 0; i < cs->nstates; i++) {
        if (cs->states[i]) return 1;
    }
    return 0;
}

cstate *cstate_copy(cstate *src){
    if (!src) return NULL;
    cstate *dst;
    dst = cstate_create(src->nstates);
    memcpy(dst->states, src->states, sizeof(uint8_t) * src->nstates);
    dst->nstates = src->nstates;
    dst->is_final = src->is_final;
    dst->is_initial = src->is_initial;
    return dst;
}
