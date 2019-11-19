/*******************************************************************************

    Authors:
        · Rafael Sanchez Sanchez - rafael.sanchezs@estudiante.uam.es
        · Alejandro Santorum Varela - alejandro.santorum@estudiante.uam.es
    File: transforma.c
    Project: First assignment AUTLEN
    Date: November 14, 2019

*******************************************************************************/

#include <stdbool.h>
#include "utils.h"
#include "linked_list.h"
#include "stack.h"
#include "afnd.h"
#include "current_state.h"
#include "transforma.h"

row *__get_dfa_table(uint8_t ***nfa_trans_tb, uint8_t **l_clausure, uint8_t *i_states, uint8_t *f_states, size_t alph_sz, size_t nstates, size_t *dfa_states);
cstate *__get_adj_states_by_sym_idx(cstate *c, size_t sym_j, uint8_t ***nfa_trans_tb, size_t nstates);
cstate **__get_dfa_transition_table_row(cstate *c, cstate **row_c, size_t alph_sz, uint8_t ***nfa_trans_tb, size_t nstates);
cstate *__add_clausure_to_cstate(cstate *c, uint8_t **l_clausure, size_t nstates);
cstate **__add_clausure_to_dfa_row(cstate **row, uint8_t **l_clausure, size_t alph_sz, size_t nstates);


/**
    AFNDTransforma
    Input:
        AFND *afnd: pointer to a ndfa object
    Returns:
        ndfa object, or NULL in error case
    Advanced description:
        It computes the equivalent DFA of the introduced NDFA.
*/
AFND *AFNDTransforma(AFND *afnd){
    if (!afnd){
        return NULL;
    }
    size_t nstates, alph_sz, dfa_states;
    uint8_t ***nfa_trans_tb = get_nfa_transition_table(afnd);
    uint8_t **l_clausure = get_lambda_clausure(afnd, &nstates);
    uint8_t *i_states = get_initial_states(afnd, &nstates);
    uint8_t *f_states = get_final_states(afnd, &nstates);
    char **alphabet = get_alphabet(afnd, &alph_sz);

    row *dfa_table = __get_dfa_table(nfa_trans_tb, l_clausure, i_states, f_states, alph_sz, nstates, &dfa_states);

    AFND *dfa = get_dfa_object(dfa_table, alphabet, alph_sz, dfa_states, nstates);

    for (size_t i = 0; i < dfa_states; i++) {
        for (size_t j = 0; j < alph_sz; j++) {
            cstate_remove(dfa_table[i].state_to[j]);
        }
        free(dfa_table[i].state_to);
        cstate_remove(dfa_table[i].state_from);
    }

    free(dfa_table);
    free(i_states);
    free(f_states);
    free(alphabet);
    sq_mtx_delete(l_clausure, nstates);
    delete_nfa_transition_table(nfa_trans_tb, nstates, alph_sz);
    return dfa;
}


/**
    __get_dfa_table (private function)
    Input:
        uint8_t ***nfa_trans_tb: NDFA transition table
        uint8_t **l_clausure: clausure lambda matrix
        uint8_t *i_states: array of initial states
        uint8_t *f_states: array of final states
        size_t alph_sz: alphabet size
        size_t nstates: NDFA number of states
        size_t *dfa_states: size_t pointer, where we return DFA num. of states
    Returns:
        ndfa object, or NULL in error case
    Advanced description:
        It computes the equivalent DFA table of the introduced NDFA.
*/
row *__get_dfa_table(uint8_t ***nfa_trans_tb, uint8_t **l_clausure, uint8_t *i_states, uint8_t *f_states, size_t alph_sz, size_t nstates, size_t *dfa_states){
    if (!nfa_trans_tb || !l_clausure){
        fprintf(stderr, "%s: NULL pointer at %s:%d\n", __func__, __FILE__, __LINE__);
        return NULL;
    }
    row* dfa_table = calloc(nstates, sizeof(row));
    size_t tb_sz = nstates;
    size_t tb_nstates = 0;
    stack *s = stack_init(stack_new(), nstates, cstate_size());
    cstate *cs = cstate_create(nstates);
    cstate *cs_aux;
    cstate **crow;
    llist *closed = llist_new();
    char buffer[255] = {0};

    cstate_init(cs, i_states);
    __add_clausure_to_cstate(cs, l_clausure, nstates);
    cstate_set_initial(cs);
    stack_push(s, cs);
    cstate_delete(cs);

    while(stack_top(s)){
        // Extract state from stack
        if (tb_nstates >= tb_sz){
            tb_sz *= 2;
            dfa_table = realloc(dfa_table, tb_sz * sizeof(row));
        }
        dfa_table[tb_nstates].state_from = cstate_new();
        cs = dfa_table[tb_nstates].state_from;
        stack_pop(s, cs);

        if (llist_in(closed, cs, cstate_cmp_v)){
            cstate_remove(cs);
            continue;
        }
        cstate_to_string(cs, buffer, 255);
        // Add to closed list
        llist_add(closed, cs);

        // Discover states that can be transversed to
        dfa_table[tb_nstates].state_to = calloc(alph_sz, sizeof(cstate *));
        __get_dfa_transition_table_row(cs, dfa_table[tb_nstates].state_to, alph_sz, nfa_trans_tb, nstates);
        __add_clausure_to_dfa_row(dfa_table[tb_nstates].state_to, l_clausure, alph_sz, nstates);
        crow = dfa_table[tb_nstates].state_to;

        if (cstate_is_final(cs, f_states)){
            cstate_set_final(cs);
        }

        // If theres a new state, push it to the stack
        for (size_t i = 0; i < alph_sz; i++) {
            if (cstate_is_valid(crow[i]) && !llist_in(closed, crow[i], cstate_cmp_v)){
                //stack_push(s, crow[i]);
                cs_aux = cstate_copy(crow[i]);
                cstate_to_string(cs_aux, buffer, 255);
                stack_push(s, cs_aux);
                cstate_delete(cs_aux);
            }
        }

        tb_nstates++;
    }

    stack_destroy(s);
    stack_delete(s);
    llist_destroy(closed);
    llist_delete(closed);
    *dfa_states = tb_nstates;
    return dfa_table;
}


/**
    __get_adj_states_by_sym_idx (private function)
    Input:
        cstate *c: current NDFA state
        size_t sym_j: symbol index
        uint8_t ***nfa_trans_tb: NDFA transition table
        size_t nstates: NDFA number of states
    Returns:
        new current state object pointer, or NULL in error case
    Advanced description:
        It computes the adjacent states from the current state with the
        symbol passed by argument (it is passed its index in the alphabet)
*/
cstate *__get_adj_states_by_sym_idx(cstate *c, size_t sym_j, uint8_t ***nfa_trans_tb, size_t nstates) {
    if (!c){
        fprintf(stderr, "%s: NULL pointer at %s:%d\n", __func__, __FILE__, __LINE__);
        return NULL;
    }
    uint8_t *states = cstate_get_states(c);
    cstate *c_next = cstate_create(nstates);
    cstate *c_aux = cstate_create(nstates);

    for (size_t i = 0; i < nstates; i++) {
        if (states[i]) {
            cstate_init(c_aux, nfa_trans_tb[i][sym_j]);
            cstate_or_cstate(c_next, c_aux);
        }
    }

    cstate_remove(c_aux);

    return c_next;
}


/**
    __get_dfa_transition_table_row (private function)
    Input:
        cstate *c: current NDFA state
        cstate **row_c: an entire row of the transition table
        size_t alph_sz: alphabet size
        uint8_t ***nfa_trans_tb: NDFA transition table
        size_t nstates: NDFA number of states
    Returns:
        the transition table row of the given current state (array of cstates)
    Advanced description:
        It computes the transition table row of the given current state, i.e., for each
        alphabet symbol, this function computes its adjacent states and stores
        it in a current state object array (a transition table row).
*/
cstate **__get_dfa_transition_table_row(cstate *c, cstate **row_c, size_t alph_sz, uint8_t ***nfa_trans_tb, size_t nstates){
    if (!c || !row_c){
        fprintf(stderr, "%s: NULL pointer at %s:%d\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    for (size_t sym_i = 0; sym_i < alph_sz; sym_i++) {
        row_c[sym_i] = __get_adj_states_by_sym_idx(c, sym_i, nfa_trans_tb, nstates);
    }

    return row_c;
}


/**
    __add_clausure_to_cstate (private function)
    Input:
        cstate *c: pointer to a current state object
        uint8_t **l_clausure: clausure lambda matrix
        size_t nstates: NDFA number of states
    Returns:
        a state after being closed with lambda clausure matrix
    Advanced description:
        It computes the clausure to a given DFA state. So for the
        DFA state, the states that can be reached with a lambda symbol are added.
*/
cstate *__add_clausure_to_cstate(cstate *c, uint8_t **l_clausure, size_t nstates){
    if (!c ||!l_clausure){
        fprintf(stderr, "%s: NULL pointer at %s:%d\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    uint8_t *initial_states = calloc(nstates, sizeof(uint8_t));
    memcpy(initial_states, cstate_get_states(c), nstates * sizeof(uint8_t));
    cstate *c_aux = cstate_create(nstates);

    for (size_t q_i = 0; q_i < nstates; q_i++) {
        if (initial_states[q_i]){
            cstate_init(c_aux, l_clausure[q_i]);
            cstate_or_cstate(c, c_aux);
        }
    }

    free(initial_states);
    cstate_remove(c_aux);
    return c;
}


/**
    __add_clausure_to_dfa_row (private function)
    Input:
        cstate **row: array of pointers of current states (transition table row)
        uint8_t **l_clausure: clausure lambda matrix
        size_t alph_sz: alphabet size
        size_t nstates: NDFA number of states
    Returns:
        the transition table row after being closed with the lambda clausure matrix
    Advanced description:
        It computes the clausure to a given transition table row, so for the
        DFA state, that represents the current row, the states that can be
        reached with a lambda symbol are added.
*/
cstate **__add_clausure_to_dfa_row(cstate **row, uint8_t **l_clausure, size_t alph_sz, size_t nstates){
    if (!row || !l_clausure){
        fprintf(stderr, "%s: NULL pointer at %s:%d\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    for (size_t sym_i = 0; sym_i < alph_sz; sym_i++) {
        __add_clausure_to_cstate(row[sym_i], l_clausure, nstates);
    }

    return row;
}
