#ifndef SUDOKU_GAME_ALG_H
#define SUDOKU_GAME_ALG_H


#include "game_control.h"

class game_alg {
public:
    static bool can_place(const game_state &state, int i, int j, int number);

    static bool check_win(const game_state &state);

    static void generate(game_state& state);

private:
    static bool __generate_dfs(game_state& state, int baseI, int baseJ, int i, int j);
};


#endif //SUDOKU_GAME_ALG_H
