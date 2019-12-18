/*******************************************************************************

    Authors:
        · Rafael Sanchez Sanchez - rafael.sanchezs@estudiante.uam.es
        · Alejandro Santorum Varela - alejandro.santorum@estudiante.uam.es
    File: minimiza.h
    Project: Second assignment AUTLEN
    Date: December 13, 2019

*******************************************************************************/

#ifndef MINIMIZA_H
#define MINIMIZA_H

#include <stdbool.h>
#include "linked_list.h"
#include "current_state.h"

/**
    struct _distinct
    Description:
        It represents a cell of the matrix which marks each pair if they are
        distinct.
        It also holds a list to know which states depends on self value to
        propagate the value.
    Members:
        uint8_t val
            1 if the pair is marked, 0 otherwise.
        llist *lst
            linked list containing the states to be marked is self is done so.
*/
typedef struct _distinct {
    uint8_t val;
    llist *lst;
} distinct;

/**
    AFNDMinimiza
    Input:
        AFND *afnd: pointer to a dfa object
    Returns:
        dfa object, or NULL in error case
    Advanced description:
        It returns the minimum expression of the DFA passed as argument

*/
AFND *AFNDMinimiza(AFND *afnd);
#endif
