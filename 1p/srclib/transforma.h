/*******************************************************************************

    Authors:
        · Rafael Sanchez Sanchez - rafael.sanchezs@estudiante.uam.es
        · Alejandro Santorum Varela - alejandro.santorum@estudiante.uam.es
    File: transforma.h
    Project: First assignment AUTLEN
    Date: November 14, 2019

*******************************************************************************/

#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include "current_state.h"

typedef struct _row {
    cstate *state_from;
    cstate **state_to;
} row;


/**
    AFNDTransforma
    Input:
        AFND *afnd: pointer to a ndfa object
    Returns:
        ndfa object, or NULL in error case
    Advanced description:
        It computes the equivalent DFA of the introduced NDFA.
*/
AFND *AFNDTransforma(AFND *afnd);

#endif
