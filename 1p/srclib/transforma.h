#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include "current_state.h"

typedef struct _row {
    cstate *state_from;
    cstate **state_to;
} row;

AFND *AFNDTransforma(AFND *afnd);

#endif
