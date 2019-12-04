#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <array>

#include "game_alg.h"

bool game_alg::can_place(const game_state &state, unsigned int i, unsigned int j, unsigned int number) {
    // O(1)
    const unsigned int bit = 1u << (number - 1u);
    return !((state.row[i] & bit) || (state.col[j] & bit) || (state.table[i][j] & bit));
}

bool game_alg::check_win(const game_state &state) {
    // O(N)
    const unsigned int mask = (1u << N) - 1u;
    for (unsigned int i : state.row) {
        if (i != mask) {
            return false;
        }
    }
    return true;
}

// must input a empty state !!!
void game_alg::generate(game_state &state, const int difficulty) {
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
            (int i, int j, const std::vector<int> &random_array) -> bool {
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

    std::function<int(int, int)> generate_one_random_number = [](int lower_bound, int upper_bound) -> int {
        std::random_device random_device;
        std::default_random_engine random_engine(random_device());
        std::uniform_int_distribution random_distribution(lower_bound, upper_bound);
        return random_distribution(random_engine);
    };

    std::array<std::function<void(game_state &)>, 3> difficult_remove_block_number{
            [&generate_one_random_number, &current_state](game_state &state) -> void {
                std::array<std::pair<int, int>, 9> remove_per_block{};

                int block_count = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        remove_per_block[block_count++] = {generate_one_random_number(i * 3 + 0, i * 3 + 2),
                                                           generate_one_random_number(j * 3 + 0, j * 3 + 2)};
                    }
                }

                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        if (std::find(remove_per_block.begin(), remove_per_block.end(), std::make_pair(i, j)) ==
                            remove_per_block.end()) {
                            state.unsafe_place_initial_number(i, j, current_state[i][j]);
                        }
                    }
                }
            },
            [&generate_one_random_number, &current_state](game_state &state) -> void {
                std::array<std::pair<int, int>, 27> remove_per_block{};

                int block_count = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            remove_per_block[block_count++] = {generate_one_random_number(i * 3 + 0, i * 3 + 2),
                                                               generate_one_random_number(j * 3 + 0, j * 3 + 2)};
                        }
                    }
                }

                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        if (std::find(remove_per_block.begin(), remove_per_block.end(), std::make_pair(i, j)) ==
                            remove_per_block.end()) {
                            state.unsafe_place_initial_number(i, j, current_state[i][j]);
                        }
                    }
                }
            },
            [&generate_one_random_number, &current_state](game_state &state) -> void {
                std::array<std::pair<int, int>, 54> remove_per_block{};

                int block_count = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 6; k++) {
                            remove_per_block[block_count++] = {generate_one_random_number(i * 3 + 0, i * 3 + 2),
                                                               generate_one_random_number(j * 3 + 0, j * 3 + 2)};
                        }
                    }
                }

                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        if (std::find(remove_per_block.begin(), remove_per_block.end(), std::make_pair(i, j)) ==
                            remove_per_block.end()) {
                            state.unsafe_place_initial_number(i, j, current_state[i][j]);
                        }
                    }
                }
            }
    };

    difficult_remove_block_number[difficulty](state);
}

