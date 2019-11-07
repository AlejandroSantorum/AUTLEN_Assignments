#ifndef CURRENTSTATE_H
#define CURRENTSTATE_H

#include <stdint.h>

typedef struct _cstate cstate;

size_t cstate_size();

cstate *cstate_create(size_t nstates);

cstate *cstate_new();

cstate *cstate_init(cstate *c, uint8_t *states);

void cstate_remove(cstate *cst);

void cstate_delete(cstate *cst);

void cstate_destroy(cstate *cst);

cstate *cstate_copy(cstate *src);

cstate *cstate_add_state(cstate *cst, int index);

cstate *cstate_or_cstate(cstate *dst, cstate *src);

uint8_t *cstate_get_states(cstate *cst);

void cstate_to_string(cstate *cst, char *string, size_t str_sz);

int cstate_cmp(cstate *c1, cstate *c2);

int cstate_cmp_v(void *c1_v, void *c2_v);

cstate *cstate_set_initial(cstate *cs);

cstate *cstate_set_final(cstate *cs);

uint8_t cstate_get_type(cstate *cst);

uint8_t cstate_is_final(cstate *cs, uint8_t *final_states);

uint8_t cstate_is_valid(cstate *cs);
#endif /* CURRENTSTATE_H */
