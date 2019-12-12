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

uint8_t *** _delete_unacc_states(uint8_t ***trans_tb, size_t nstates, uint8_t initial, size_t alph_sz);
void _update_acc(uint8_t ***trans_tb, size_t nstates, uint8_t state, size_t alph_sz, uint8_t *acc, stack *stack_acc);
distinct ** _calc_distinct_mtx(uint8_t ***trans_tb, uint8_t *fstates, size_t nstates, size_t alph_sz);
uint8_t _exists_marked_transition(uint8_t ***trans_tb, distinct ** d_mtx,size_t nstates, size_t alph_sz, size_t i, size_t j);
void _mark_recursively(distinct **d_mtx, llist *lst);
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
    distinct **d_mtx = _calc_distinct_mtx(trans_tb, f_states, nstates, alph_sz);


    // Translate our table to the AFND object this function has to return
    //AFND *dfa = get_dfa_object(min_table, alphabet, alph_sz, dfa_states, nstates);


    free(f_states);
    free(alphabet);
    delete_nfa_transition_table(trans_tb, nstates, alph_sz);
    /*return dfa;*/return NULL;
}


uint8_t *** _delete_unacc_states(uint8_t ***trans_tb, size_t nstates, uint8_t initial, size_t alph_sz){
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

void _update_acc(uint8_t ***trans_tb, size_t nstates, uint8_t state, size_t alph_sz, uint8_t *acc, stack *stack_acc){
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

distinct ** _calc_distinct_mtx(uint8_t ***trans_tb, uint8_t *fstates, size_t nstates, size_t alph_sz){
    // TODO: error handling
    distinct ** d_mtx = calloc(nstates, sizeof(distinct *));
    for (size_t i = 0; i < nstates; i++) {
        d_mtx[i] = calloc(nstates, sizeof(distinct));
    }

    // Mark distinct states between finals and not finals
    for (size_t i = 0; i < nstates; i++) {
        if (fstates[i]){
            for (size_t j = 0; j < nstates; j++) { // TODO: Podría ser a partir de j=i+1
                if (!fstates[j]){
                    d_mtx[i][j].val = 1;
                }
            }
        }
    }

    size_t di, dj;
    for (size_t i = 0; i < nstates; i++) {
        for (size_t j = i+1; j < nstates; j++) { // TODO: This might explode the algorithm
            if (_exists_marked_transition(trans_tb, d_mtx ,nstates, alph_sz, i, j)) {
                _mark_recursively(d_mtx, d_mtx[i][j].lst);
            } else {
                for (size_t sym = 0; sym < alph_sz; sym++) {
                    di = dfa_delta(trans_tb, nstates, i, sym);
                    dj = dfa_delta(trans_tb, nstates, j, sym);
                    if (di == dj){
                        continue;
                    }
                    if (!d_mtx[di][dj].lst) {
                        d_mtx[di][dj].lst = llist_new();
                    }
                    // TODO: Check if its better to do this outside of this loop
                    tuple *t = calloc(1, sizeof(tuple));
                    t->a = i; t->b = j;
                    llist_add(d_mtx[di][dj].lst, t);
                }
            }
        }
    }
    return d_mtx;
}


uint8_t _exists_marked_transition(uint8_t ***trans_tb, distinct ** d_mtx,size_t nstates, size_t alph_sz, size_t i, size_t j){
    size_t di, dj;
    for (size_t sym = 0; sym < alph_sz; sym++) {
        di = dfa_delta(trans_tb, nstates, i, sym);
        dj = dfa_delta(trans_tb, nstates, j, sym);
        if (d_mtx[di][dj].val){
            return 1;
        }
    }
    return 0;
}

void _mark_recursively(distinct **d_mtx, llist *lst){
    tuple *t;
    while( (t = llist_pop(lst)) ){
        d_mtx[t->a][t->b].val = 1;
        _mark_recursively(d_mtx, d_mtx[t->a][t->b].lst);
        free(t);
    }
}
