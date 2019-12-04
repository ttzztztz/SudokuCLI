#ifndef SUDOKU_GAME_ALG_H
#define SUDOKU_GAME_ALG_H


#include "game_control.h"

class game_alg {
public:
    static bool can_place(const game_state &state, unsigned int i, unsigned int j, unsigned int number);

    static bool check_win(const game_state &state);

    static void generate(game_state& state, int difficulty);
};


#endif //SUDOKU_GAME_ALG_H
