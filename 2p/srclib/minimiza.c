/*******************************************************************************

    Authors:
        · Rafael Sanchez Sanchez - rafael.sanchezs@estudiante.uam.es
        · Alejandro Santorum Varela - alejandro.santorum@estudiante.uam.es
    File: minimiza.c
    Project: First assignment AUTLEN
    Date: November 14, 2019

*******************************************************************************/

#include <stdbool.h>
#include "utils.h"
#include "linked_list.h"
#include "stack.h"
#include "afnd.h"
#include "current_state.h"
#include "minimiza.h"

uint8_t *** _delete_unacc_states(uint8_t ***trans_tb, size_t nstates, uint8_t initial, uint8_t alph_sz);
void _update_acc(uint8_t ***trans_tb, size_t nstates, uint8_t state, uint8_t alph_sz, uint8_t *acc, stack *stack_acc);

/**
    AFNDMinimiza
    Input:
        AFND *afnd: pointer to a dfa object
    Returns:
        dfa object, or NULL in error case
    Advanced description:

*/
AFND *AFNDMinimiza(AFND *afnd){
    if (!afnd){
        return NULL;
    }
    size_t nstates, alph_sz, dfa_states;
    // Fill our structures from the AFND object information
    //// Get the DFA transition table
    uint8_t ***trans_tb = get_nfa_transition_table(afnd);
    //// Get the initial state
    uint8_t *i_states = get_initial_states(afnd, &nstates);
    //// Get the set of final states
    uint8_t *f_states = get_final_states(afnd, &nstates);
    //// Get alphabet of the NDFA
    char **alphabet = get_alphabet(afnd, &alph_sz);

    // Set all transitions regarding unacc states to -1
    trans_tb = _delete_unacc_states(trans_tb, nstates, i_states[0], alph_sz);

    

    // Translate our table to the AFND object this function has to return
    //AFND *dfa = get_dfa_object(min_table, alphabet, alph_sz, dfa_states, nstates);


    free(f_states);
    free(alphabet);
    delete_nfa_transition_table(trans_tb, nstates, alph_sz);
    /*return dfa;*/return NULL;
}


uint8_t *** _delete_unacc_states(uint8_t ***trans_tb, size_t nstates, uint8_t initial, uint8_t alph_sz){
    // List of accesible states
    uint8_t *acc = calloc(nstates, sizeof(uint8_t));
    // Stack of states to check its accesibility
    stack *stack_acc = stack_init(stack_new(), nstates, sizeof(uint8_t));

    // Inserting initial state into accesible list and stack
    acc[initial] = 1;
    stack_push(stack_acc, &initial);

    // Discover all accessibles states
    uint8_t state;
    while (stack_top(stack_acc)){
        stack_pop(stack_acc, &state);
        _update_acc(trans_tb, nstates, state, alph_sz, acc, stack_acc);
    }

    // Delete not accessible states
    for (size_t i = 0; i < nstates; i++) {
        if (!acc[i]){
            for (size_t j = 0; j < alph_sz; j++) {
                for (size_t k = 0; k < nstates; k++) {
                    trans_tb[k][j][i] = -1;
                    trans_tb[i][j][k] = -1;
                }
            }
        }
    }

    free(acc);
    stack_delete(stack_acc);
    stack_destroy(stack_acc);
    return trans_tb;
}

void _update_acc(uint8_t ***trans_tb, size_t nstates, uint8_t state, uint8_t alph_sz, uint8_t *acc, stack *stack_acc){
    for (size_t sym = 0; sym < alph_sz; sym++) {
        for (uint8_t j = 0; j < nstates; j++) {
            if(trans_tb[state][sym][j]){
                if (!acc[j]){
                    stack_push(stack_acc, &j);
                    acc[j] = 1;
                }
                break;
            }
        }
    }
}
