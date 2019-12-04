#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <ctime>

#include "game_alg.h"

#define MAX_RANDOM_GENERATE_FUNCTION_CALL_TIME 220

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

// must input a empty state !!!
void game_alg::generate(game_state &state) {
    int call_time = 0;

    std::function<std::vector<int>()> generate_random_permutation = [&]() -> std::vector<int> {
        std::vector<int> seed = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::random_shuffle(seed.begin(), seed.end());
        call_time++;

        return seed;
    };

    for (int i = 0; i < 9; i++) {
        if (i == 0) {
            state = game_state();

            call_time = 0;
            std::vector<int> permutation = generate_random_permutation();
            for (int j = 0; j < 9; j++) {
                state.unsafe_place_number(i, j, permutation[j]);
            }
        } else {
            std::vector<int> permutation = generate_random_permutation();
            if (call_time > MAX_RANDOM_GENERATE_FUNCTION_CALL_TIME) {
                i = -1;
                continue;
            }

            for (int j = 0; j < 9; j++) {
                if (!game_alg::can_place(state, i, j, permutation[j])) {
                    for (int k = 0; k < j; k++) {
                        state.unsafe_remove_number(i, k);
                    }

                    j = 9, i -= 1;
                } else {
                    state.unsafe_place_number(i, j, permutation[j]);
                }
            }
        }
    }
}

