#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 255

int load_fsm (char *filename, char ***table, int **finals, int *table_sz, int *finals_sz) {
    int nstates, nfinals;
    FILE *fp = fopen(filename, "r");
    char buffer[SIZE];

    fgets(buffer, SIZE, fp);
    nstates = atoi(buffer);

    *table = calloc(nstates, sizeof(char *));
    for (size_t i = 0; i < nstates; i++) {
        (*table)[i] = calloc(nstates, sizeof(char));
    }

    for (size_t i = 0; i < nstates; i++) {
        fgets((*table)[i], nstates+1, fp);
        fseek(fp, 1, SEEK_CUR);
    }

    fgets(buffer, SIZE, fp);
    nfinals = atoi(buffer);

    *finals = calloc(nfinals, sizeof(int));

    for (size_t i = 0; i < nfinals; i++) {
        fscanf(fp, "%d\n", *(finals+i));
    }
    *table_sz = nstates;
    *finals_sz = nfinals;
    return 0;
}

int main(int argc, char *argv[]) {
    /* char table[4][4] = {{0, "a", 0, "b"},
                        {"b", 0, "a", "c"},
                        {"a", "b", 0, "c"},
                        {"c", "b", "a", 0}};
    int finals[] = {3}; */
    char *string = argv[2];
    int state = 0;
    bool found = false;
    char **table;
    int *finals, table_sz, finals_sz;

    load_fsm(argv[1], &table, &finals, &table_sz, &finals_sz);

    while (*string) {
        printf("Estoy en q%d y voy a leer el caracter %c\n", state,  *string);
        for (size_t i = 0; i < table_sz; i++, found = false) {
            if (*string == table[state][i]) {
                //Change state
                state = i;
                found = true;
                break;
            }
        }
        if (!found) {
            printf("Palabra no reconocida por el autómata. (No existe transición).\n");
            return 0;
        }
        string++;
    }
    printf("He terminado la palabra, estoy en q%d\n", state);

    for (size_t i = 0; i < finals_sz; i++) {
        if (finals[i] == state){
            printf("Palabra reconocida por el autómata\n");
            return 0;
        }
    }

    printf("Palabra no reconocida por el autómata. (Estado no final).\n");
    return 0;
}
