#ifndef CURRENTSTATE_H
#define CURRENTSTATE_H

typedef struct _cstate cstate;


cstate *cstate_create(size_t nstates);

void cstate_remove(cstate *cst);

cstate *cstate_add_state(cstate *cst, int index);

void cstate_to_string(cstate *cst, char **string);

#endif /* CURRENTSTATE_H */
