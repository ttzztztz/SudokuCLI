#include <cstring>

#include "game_alg.h"

bool game_alg::can_place(const game_state &state, int i, int j, int number) {
    // O(1)
    const int bit = 1 << (number - 1);
    return !((state.row[i] & bit) || (state.col[j] & bit) || (state.table[i][j] & bit));
}

bool game_alg::check_win(const game_state &state) {
    // O(N)
    const int mask = (1 << N) - 1;
    for (int i = 0; i < N; i++) {
        if (state.row[i] != mask) {
            return false;
        }
    }
    return true;
}

void game_alg::generate(game_state &state) {
    
}
