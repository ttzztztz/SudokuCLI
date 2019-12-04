#include <random>
#include <vector>
#include <algorithm>
#include <functional>

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

// must input a empty state !!!
void game_alg::generate(game_state &state) {
    const int MAX_CALL_RANDOM_ARRAY_GENERATE_TIMES = 220;
    std::vector<std::vector<int>> current_state(9, std::vector<int>(9, 0));
    int current_call_times = 0;

    std::function<std::vector<int>(void)> random_array_generator = [&]() -> std::vector<int> {
        std::vector<int> result{1, 2, 3, 4, 5, 6, 7, 8, 9};
        current_call_times++;

        std::random_device random_device;
        std::mt19937 random_generator(random_device());
        std::shuffle(result.begin(), result.end(), random_generator);

        return result;
    };

    std::function<bool(int, int, std::vector<int>)> try_input_val = [&current_state]
            (int i, int j, const std::vector<int>& random_array) -> bool {
        std::function<bool(int, int)> check_valid = [&current_state](int i, int j) -> bool {
            std::function<bool(int, int)> check_conflict_row = [&current_state](int i, int j) -> bool {
                const int val = current_state[i][j];
                for (int k = 0; k < i; k++) {
                    if (current_state[k][j] == val) {
                        return false;
                    }
                }
                return true;
            };
            std::function<bool(int, int)> check_conflict_column = [&current_state](int i, int j) -> bool {
                const int val = current_state[i][j];
                for (int k = 0; k < j; k++) {
                    if (current_state[i][k] == val) {
                        return false;
                    }
                }
                return true;
            };
            std::function<bool(int, int)> check_conflict_block = [&current_state](int i, int j) -> bool {
                const int base_i = i / 3 * 3, base_j = j / 3 * 3, val = current_state[i][j];
                for (int a = 0; a < 3; a++) {
                    for (int b = 0; b < 3; b++) {
                        if (current_state[base_i + a][base_j + b] == 0 || (base_i + a == i && base_j + b == j)) {
                            continue;
                        }

                        if (current_state[base_i + a][base_j + b] == val) {
                            return false;
                        }
                    }
                }
                return true;
            };

            return check_conflict_block(i, j) && check_conflict_column(i, j) && check_conflict_row(i, j);
        };

        for (int k = 0; k < 9; k++) {
            current_state[i][j] = random_array[k];
            if (check_valid(i, j)) {
                return true;
            }
        }
        return false;
    };

    for (int i = 0; i < 9; i++) {
        if (i == 0) {
            current_call_times = 0;
            current_state[0] = random_array_generator();
        } else {
            std::vector<int> random_array = random_array_generator();

            for (int j = 0; j < 9; j++) {
                if (current_call_times >= MAX_CALL_RANDOM_ARRAY_GENERATE_TIMES) {
                    i = -1;
                    current_state = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
                    break;
                }

                if (!try_input_val(i, j, random_array)) {
                    current_state[i] = std::vector<int>(9, 0);
                    i--;
                    break;
                }
            }
        }
    }

    state = game_state();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            state.unsafe_place_initial_number(i, j, current_state[i][j]);
        }
    }
}

