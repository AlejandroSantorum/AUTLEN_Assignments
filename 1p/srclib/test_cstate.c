#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "current_state.h"

int main(int argc, char const *argv[]) {
    cstate *cs;
    char buff[255]={0};
    cs = cstate_create(20);
    cstate_add_state(cs, 19);
    cstate_add_state(cs, 0);
    cstate_add_state(cs, 7);
    cstate_to_string(cs, &buff);
    printf("%s\n", buff);
    cstate_remove(cs);
    return 0;
}
