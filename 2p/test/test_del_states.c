#include "minimiza.h"

int main(int argc, char const *argv[]) {
    uint8_t trans_tb[5][2][5] = { { {0, 1, 0, 0, 0},  {0, 0, 1, 0, 0} },
                                  { {0, 0, 0, 0, 0},  {0, 0, 0, 0, 0} },
                                  { {0, 1, 0, 0, 0},  {0, 1, 0, 0, 0} },
                                  { {0, 0, 0, 0, 0},  {0, 0, 1, 0, 0} },
                                  { {0, 0, 0, 0, 0},  {0, 0, 0, 1, 0} } };

    uint8_t ***new_trans_tb;
    new_trans_tb = _delete_unacc_states(trans_tb, 5, 0, NULL, 2);

    return 0;
}
