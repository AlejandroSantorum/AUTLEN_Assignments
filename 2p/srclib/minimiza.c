/*******************************************************************************

    Authors:
        · Rafael Sanchez Sanchez - rafael.sanchezs@estudiante.uam.es
        · Alejandro Santorum Varela - alejandro.santorum@estudiante.uam.es
    File: minimiza.c
    Project: Second assignment AUTLEN
    Date: December 13, 2019

*******************************************************************************/

#include <stdbool.h>
#include "utils.h"
#include "linked_list.h"
#include "stack.h"
#include "afnd.h"
#include "current_state.h"
#include "minimiza.h"

// Functions to get the minimum deterministic finite automata
row * __get_min_dfa(uint8_t ***trans_tb, distinct **d_mtx, uint8_t *i_states, uint8_t *f_states, size_t nstates, size_t alph_sz, size_t *min_states);
row * __explore_states(distinct **d_mtx, uint8_t *i_states, uint8_t *f_states, size_t nstates, size_t *min_states);
void __set_transitions(row *min_dfa, uint8_t ***trans_tb, size_t nstates, size_t min_states, size_t alph_sz);
size_t __find_min_state(row *min_dfa, size_t min_states, size_t delta);

// Functions regarding the distinct matrix
distinct ** __calc_distinct_mtx(uint8_t ***trans_tb, uint8_t *fstates, size_t nstates, size_t alph_sz);
uint8_t __exists_marked_transition(uint8_t ***trans_tb, distinct ** d_mtx,size_t nstates, size_t alph_sz, size_t i, size_t j);
void __mark_recursively(distinct **d_mtx, llist *lst);

// Functions regarding the deletion of unaccesibles states
uint8_t *** __delete_unacc_states(uint8_t ***trans_tb, size_t nstates, uint8_t initial, size_t alph_sz, size_t *n_acc, uint8_t **acc);
void __update_acc(uint8_t ***trans_tb, size_t nstates, uint8_t state, size_t alph_sz, uint8_t *acc, stack *stack_acc);
void __update_metadata(uint8_t *acc, size_t nstates, size_t n_acc, uint8_t **i_states, uint8_t **f_states, char ***state_names);


/**
    AFNDMinimiza
    Input:
        AFND *afnd: pointer to a dfa object
    Returns:
        dfa object, or NULL in error case
    Advanced description:
        It returns the minimum expression of the DFA passed as argument

*/
AFND *AFNDMinimiza(AFND *afnd){
    if (!afnd){
        return NULL;
    }
    size_t nstates, alph_sz;
    tuple *t;

    // Fill our structures from the AFND object information
    //// Get the DFA transition table
    uint8_t ***trans_tb = get_nfa_transition_table(afnd);
    //// Get the initial state
    uint8_t *i_states = get_initial_states(afnd, &nstates);
    uint8_t init = 0;
    for (size_t j = 0; i_states[j] == 0; init = ++j);
    //// Get the set of final states
    uint8_t *f_states = get_final_states(afnd, &nstates);
    //// Get the names of the states
    char **state_names = get_states_names(afnd, &nstates);
    //// Get alphabet of the NDFA
    char **alphabet = get_alphabet(afnd, &alph_sz);

    // Delete unaccesibles states.
    size_t n_acc;
    uint8_t *acc;
    trans_tb = __delete_unacc_states(trans_tb, nstates, init, alph_sz, &n_acc, &acc);
    //// Update initials, finals and names (arrays can be shifted)
    __update_metadata(acc, nstates, n_acc, &i_states, &f_states, &state_names);

    // Calculate undistinguishable state matrix
    distinct **d_mtx = __calc_distinct_mtx(trans_tb, f_states, n_acc, alph_sz);

    // Build the transition table of the minimum DFA, merging equivalent states
    size_t dfa_states;
    row *min_dfa = __get_min_dfa(trans_tb, d_mtx, i_states, f_states, n_acc, alph_sz, &dfa_states);

    // Translate our table to the AFND object this function has to return
    AFND *dfa = get_dfa_object(min_dfa, alphabet, alph_sz, dfa_states, nstates, state_names);

    // Free's resources
    for (size_t i = 0; i < n_acc; i++) {
        for (size_t j = 0; j < n_acc; j++) {
            while( (t = llist_pop(d_mtx[i][j].lst)) ){
                free(t);
            }
            llist_destroy(d_mtx[i][j].lst);
            llist_delete(d_mtx[i][j].lst);
        }
        free(d_mtx[i]);
    }
    free(d_mtx);

    for (size_t i = 0; i < dfa_states; i++) {
        for (size_t j = 0; j < alph_sz; j++) {
            cstate_remove(min_dfa[i].state_to[j]);
        }
        free(min_dfa[i].state_to);
        cstate_remove(min_dfa[i].state_from);
    }

    free(acc);
    free(min_dfa);
    free(f_states);
    free(i_states);
    free(state_names);
    free(alphabet);
    delete_nfa_transition_table(trans_tb, n_acc, alph_sz);
    return dfa;
}


/**
    __get_min_dfa (private function)
    Input:
        uint8_t ***trans_tb: DFA transition table
        distinct **d_mtx: distinct matrix
        uint8_t *i_states: array of initial states
        uint8_t *f_states: array of final states
        size_t nstates: DFA number of states
        size_t alph_sz: alphabet size
        size_t *dfa_states: size_t pointer, where we return minimum DFA num. of states
    Returns:
        A representation of a DFA transition table, an array of rows.
        For example given this DFA:
            States: q0, q0q1
            Symbols: +, -
            Transitions:
                (q0q1, +) = q0q2
                (q0q1, -) = q0q1
                (q0q2, +) = q0q2
                (q0q2, -) = q0q1
            Then we would return an array of size 2:
                row[0] = {cstate *state_from = q0q1,
                          cstate *state_to = [q0q2, q0q1]}
                row[1] = {cstate *state_from = q0q2,
                          cstate *state_to = [q0q2, q0q1]}
    Advanced description:
        It computes the equivalent minimum DFA table of the introduced DFA. This
        runs the algorithm necessary to merge undistinguishable states presents
        in d_mtx and assigning the right transitions to them.
*/
row * __get_min_dfa(uint8_t ***trans_tb, distinct **d_mtx, uint8_t *i_states, uint8_t *f_states, size_t nstates, size_t alph_sz, size_t *min_states) {
    *min_states = 0;
    // Merge undistinguishable states and add it to the min_dfa array of rows, each
    // one in the field state_from of each array position.
    row *min_dfa = __explore_states(d_mtx, i_states, f_states, nstates, min_states);

    // Fill the state_to for each state looking into the original transition table
    // As it is a DFA it will copy each transition in trans_tb to our structure accordingly
    __set_transitions(min_dfa, trans_tb, nstates, *min_states, alph_sz);
    return min_dfa;
}


/**
    __explore_states (private function)
    Input:
        distinct **d_mtx: distinct matrix
        uint8_t *i_states: array of initial states
        uint8_t *f_states: array of final states
        size_t nstates: DFA number of states
        size_t *min_states: size_t pointer, where we return minimum DFA num. of states
    Returns:
        A representation of a DFA transition table, an array of rows, merging the
        undistinguishable ones.
        For example given this DFA:
            States: q0, q1, q2 (where q1 and q2 are undistinguishable)
            Transitions:
                (q0, +) = q1q2
                (q0, -) = q0
                (q1q2, +) = q1q2
                (q1q2, -) = q0
            Then we would return an array of size 2:
                row[0] = {cstate *state_from = q0,
                          cstate *state_to = NULL}
                row[1] = {cstate *state_from = q1q2,
                          cstate *state_to = NULL}
    Advanced description:
        It creates a row array and explore the distinguishable states in the
        automata using d_mtx to merge appropiately. Then it adds it to the
        state_from property of each row cell.
*/
row * __explore_states(distinct **d_mtx, uint8_t *i_states, uint8_t *f_states, size_t nstates, size_t *min_states){
    // Memory allocation
    //// Minimum Automata transition table
    row *min_dfa = calloc(nstates, sizeof(row));
    //// Holds a list of currently explored states
    uint8_t *explored = calloc(nstates, sizeof(uint8_t));
    //// Represents the possible union of states. For example, if we have states q0 q1 q2
    //// and q1 is equivalent to q2 then aux would be [0 1 1] when exploring either q1 or q2

    //// Note that only one of q1 or q2 will be explored
    uint8_t *aux = calloc(nstates, sizeof(uint8_t));
    uint8_t is_init, is_final;
    is_init = is_final = 0;

    // Do the actual iterative exploration
    for (size_t i = 0; i < nstates; i++) {
        // If state[i] has not been explored
        if (!explored[i]){
            // Create a new state in the table
            min_dfa[*min_states].state_from = cstate_create(nstates);
            // Initialise aux to the equivalent states of the one being explored and
            // set explored to true to each equivalent state so we dont explore it
            // again
            for (size_t j = 0; j < nstates; j++) {
                // In our implemenation, d_mtx[i][j] = 1 means distinct and 0
                // means equivalent, so we want to get: aux <-- !d_mtx[i]
                aux[j] = !d_mtx[i][j].val;
                explored[j] |= aux[j];
                if (aux[j]){
                    if (i_states[j]) is_init = 1;
                    if (f_states[j]) is_final = 1;
                }
            }
            // Initialise the set of equivalent states with aux array
            cstate_init(min_dfa[*min_states].state_from, aux);
            // Set inital and final if necessary
            if (is_init) cstate_set_initial(min_dfa[*min_states].state_from);
            if (is_final) cstate_set_final(min_dfa[*min_states].state_from);
            is_init = is_final = 0;
            (*min_states)++;
        }
    }
    free(aux);
    free(explored);
    return min_dfa;
}


/**
    __set_transitions (private function)
    Input:
        row *min_dfa: minimum automata table (states already discovered)
        uint8_t ***trans_tb: original automata table
        size_t nstates: number of states in trans_tb
        size_t min_states: number of states in min_dfa
        size_t alph_sz: size of the alphabet

    Returns:
        void

    Advanced description:
        It fills the transitions in the (row *) array, alloc'ing the state_to field
        in each cell of min_dfa and filling it up accordingly with the information
        in trans_tb
*/
void __set_transitions(row *min_dfa, uint8_t ***trans_tb, size_t nstates, size_t min_states, size_t alph_sz){

    // for each state in min_dfa
    for (size_t i = 0; i < min_states; i++) {
        uint8_t *state = cstate_get_states(min_dfa[i].state_from);
        // alloc memory for the transition array
        min_dfa[i].state_to = calloc(alph_sz, sizeof(cstate *));

        // find first single state present in the state of the min_dfa
        size_t from = 0;
        for (size_t j = 0; state[j] == 0; from = ++j);

        // for each symbol in the alphabet
        for (size_t sym = 0; sym < alph_sz; sym++) {
            // find state index in the original table that we transverse to, from first state found
            size_t delta = dfa_delta(trans_tb, nstates, from, sym);
            // find state index for such state in the minimum dfa table
            size_t idx = __find_min_state(min_dfa, min_states, delta);
            // sets a copy of such state in the corresponding cell of min_dfa transition array
            min_dfa[i].state_to[sym] = cstate_copy(min_dfa[idx].state_from);
        }
    }
}


/**
    __find_min_state (private function)
    Input:
        row *min_dfa: minimum automata table (states already discovered)
        size_t min_states: number of states in min_dfa
        size_t delta: original index of state in dfa table

    Returns:
        Index of delta state in min_dfa table.

    Advanced description:
        It searches for the state in min_dfa table that contains the state
        with index delta from the original table.
*/
size_t __find_min_state(row *min_dfa, size_t min_states, size_t delta){
    // for each state in min_states
    for (size_t i = 0; i < min_states; i++) {
        uint8_t *states = cstate_get_states(min_dfa[i].state_from);
        // if delta in states[i]
        if (states[delta]){
            return i;
        }
    }
    return -1;
}


/**
    __delete_unacc_states (private function)
    Input:
        uint8_t ***trans_tb: original dfa transition table
        size_t nstates: number of states in trans_tb
        uint8_t initial: dfa initial state index
        size_t alph_sz: alphabet size
        size_t *n_acc_states: number of accesible states (this will be a value to return)
        uint8_t **acess: accesible states indexes array (this will be a value to return)

    Returns:
        Updated transition table without unaccesibles states.

    Advanced description:
        It finds the unaccesibles states from trans_tb, then allocs a new trans_tb
        and copy the original to it without those states.
        n_acc_states will contain the new number of states in trans_tb and
        acess will be the array of 0s and 1s that represent whether the original
        state is accesible (1) or not (0). This will be used to update metadata.
*/
uint8_t *** __delete_unacc_states(uint8_t ***trans_tb, size_t nstates, uint8_t initial, size_t alph_sz, size_t *n_acc_states, uint8_t **access){
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
        // Push to the stack all the states than can be transversed from popped state
        __update_acc(trans_tb, nstates, state, alph_sz, acc, stack_acc);
    }

    // Create new adjacency matrix
    size_t n_acc = 0;
    for (size_t i = 0; i < nstates; n_acc += acc[i++]);
    uint8_t ***new_trans_tb = calloc(n_acc, sizeof(uint8_t **));
    for (size_t i = 0; i < n_acc; i++) {
        new_trans_tb[i] = calloc(alph_sz, sizeof(uint8_t *));
        for (size_t j = 0; j < alph_sz; j++) {
            new_trans_tb[i][j] = calloc(n_acc, sizeof(uint8_t));
        }
    }

    //// Copy trans_tb to new adjacency matrix
    for (size_t i = 0, new_i = 0; i < nstates; i++) {
        if (acc[i]){
            for (size_t j = 0; j < alph_sz; j++) {
                for (size_t k = 0, new_k = 0; k < nstates; k++) {
                    if (acc[k]){
                        new_trans_tb[new_i][j][new_k] = trans_tb[i][j][k];
                        new_k++;
                    }
                }
            }
            new_i++;
        }
    }

    // Free resources
    delete_nfa_transition_table(trans_tb, nstates, alph_sz);

    *access = acc;
    stack_destroy(stack_acc);
    stack_delete(stack_acc);
    *n_acc_states = n_acc;
    return new_trans_tb;
}


/**
    __update_acc (private function)
    Input:
        uint8_t ***trans_tb: original dfa transition table
        size_t nstates: number of states in trans_tb
        uint8_t state: current state to update accesibility
        size_t alph_sz: alphabet size
        uint8_t *acc: array of accesible states (indexes)
        stack *stack_acc: stack which we'll pop states from, in order to explore them

    Returns:
        void

    Advanced description:
        Pushes to the stack all the states that can be transversed from -state-,
        if they have not been pushed before.
*/
void __update_acc(uint8_t ***trans_tb, size_t nstates, uint8_t state, size_t alph_sz, uint8_t *acc, stack *stack_acc){
    for (size_t sym = 0; sym < alph_sz; sym++) {
        for (uint8_t j = 0; j < nstates; j++) {
            if(trans_tb[state][sym][j]){
                // There is a transition from current state to state j with symbol sym
                if (!acc[j]){
                    // Setting state j as accesible
                    acc[j] = 1;
                    // Since its accesible, it is inserted into the stack
                    // to explore it later
                    stack_push(stack_acc, &j);
                }
                break;
            }
        }
    }
}


/**
    __update_metadata (private function)
    Input:
        uint8_t *acc: list of accesible states
        uint8_t nstates: number of dfa states
        size_t nacc: DFA number of accesible states
        uint8_t **i_states: new list of initial states
        uint8_t **f_states: new list of final state
        char ***state_names: new list of state names
    Returns:
        The function itself returns nothing. However:
        It returns new structures for initial and final states,
        along with new state names.
    Advanced description:
        With the information of the trully accesible states, we compute
        the new list of initial states, the new list of final states, and
        the new names of the states, returning them by reference.
*/
void __update_metadata(uint8_t *acc, size_t nstates, size_t nacc, uint8_t **i_states, uint8_t **f_states, char ***state_names){
    // Allocating memory for new structures
    uint8_t *finals = calloc(nacc, sizeof(uint8_t));
    uint8_t *initials = calloc(nacc, sizeof(uint8_t));
    char **names = calloc(nacc, sizeof(char *));
    size_t new_i = 0;
    // Setting initial and final states, along withnew state names
    for (size_t i = 0; i < nstates; i++) {
        if (acc[i]){
            initials[new_i] = (*i_states)[i];
            finals[new_i] = (*f_states)[i];
            names[new_i] = (*state_names)[i];
            new_i++;
        }
    }
    // Deleting old structures
    free(*f_states);
    free(*i_states);
    free(*state_names);
    // Returning it by reference
    *f_states = finals;
    *i_states = initials;
    *state_names = names;
}


/**
    __calc_distinct_mtx (private function)
    Input:
        uint8_t ***trans_tb: DFA transition table
        uint8_t *fstates: array of final states
        size_t nstates: DFA number of states
        size_t alph_sz: alphabet size
    Returns:
        It returns a squared matrix (of size nstates) of uint8_t.
    Advanced description:
        If the coordinate (i,j) of the matrix is represented as 1, it
        means that state i is distinct than state j.
        By default it is a null matrix (matrix of zeros).
*/
distinct ** __calc_distinct_mtx(uint8_t ***trans_tb, uint8_t *fstates, size_t nstates, size_t alph_sz){
    distinct ** d_mtx = calloc(nstates, sizeof(distinct *));
    for (size_t i = 0; i < nstates; i++) {
        d_mtx[i] = calloc(nstates, sizeof(distinct));
    }

    // Mark distinct states between finals and not finals
    for (size_t i = 0; i < nstates; i++) {
        if (fstates[i]){
            for (size_t j = 0; j < nstates; j++) {
                if (!fstates[j]){
                    d_mtx[i][j].val = 1;
                }
            }
        } else {
            for (size_t j = 0; j < nstates; j++) {
                if (fstates[j]){
                    d_mtx[i][j].val = 1;
                }
            }
        }
    }

    // Markingrest of states recursively, sice we already know finals and
    // not finals are distinct
    size_t di, dj;
    for (size_t i = 0; i < nstates; i++) {
        for (size_t j = 0; j < nstates; j++) {
            if (__exists_marked_transition(trans_tb, d_mtx ,nstates, alph_sz, i, j)) {
                // If exists a marked transition from state i to state j (with any symbol)
                // We mark it
                d_mtx[i][j].val = 1;
                // And for all the states in its assoc list (recursively)
                __mark_recursively(d_mtx, d_mtx[i][j].lst);
            } else {
                // Otherwise, we create the assoc list
                for (size_t sym = 0; sym < alph_sz; sym++) {
                    di = dfa_delta(trans_tb, nstates, i, sym);
                    dj = dfa_delta(trans_tb, nstates, j, sym);
                    if (di == dj){
                        continue;
                    }
                    if (!d_mtx[di][dj].lst) {
                        d_mtx[di][dj].lst = llist_new();
                    }

                    tuple *t = calloc(1, sizeof(tuple));
                    t->a = i; t->b = j;
                    llist_add(d_mtx[di][dj].lst, t);
                }
            }
        }
    }
    return d_mtx;
}


/**
    __exists_marked_transition (private function)
    Input:
        uint8_t ***trans_tb: DFA transition table
        distinct **d_mtx: distinct matrix
        size_t nstates: DFA number of states
        size_t alph_sz: alphabet size
        size_t i: index of first state
        size_t j: index of second state
    Returns:
        1 (true) if exists a transition between state i and state j.
        0 (false) otherwise
    Advanced description:
        It checks if there is marked a transition between state i and
        state j.
*/
uint8_t __exists_marked_transition(uint8_t ***trans_tb, distinct ** d_mtx, size_t nstates, size_t alph_sz, size_t i, size_t j){
    size_t di, dj;
    for (size_t sym = 0; sym < alph_sz; sym++) {
        // getting transition from state i with symbol sym
        di = dfa_delta(trans_tb, nstates, i, sym);
        // getting transition from state j with symbol sym
        dj = dfa_delta(trans_tb, nstates, j, sym);
        // checking if marked
        if (d_mtx[di][dj].val){
            return 1;
        }
    }
    return 0;
}


/**
    __mark_recursively (private function)
    Input:
        distinct **d_mtx: distinct matrix
        llist *lst: linked list of states
    Returns:
        Void
    Advanced description:
        Marks all pairs in lst as distinguishable and propagate it
        to each pair in the list.
*/
void __mark_recursively(distinct **d_mtx, llist *lst){
    tuple *t;
    while( (t = llist_pop(lst)) ){
        // marking pair
        d_mtx[t->a][t->b].val = 1;
        // propagating it recursively
        __mark_recursively(d_mtx, d_mtx[t->a][t->b].lst);
        free(t);
    }
}
