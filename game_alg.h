#ifndef SUDOKU_GAME_ALG_H
#define SUDOKU_GAME_ALG_H


#include "game_control.h"

class game_alg {
public:
    static bool can_place(const game_state &state, int i, int j, int number);

    static bool check_win(const game_state &state);
};


#endif //SUDOKU_GAME_ALG_H
