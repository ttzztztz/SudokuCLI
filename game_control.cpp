#include <iostream>
#include <tuple>
#include <optional>
#include <functional>
#include <ctime>

#include "game_control.h"
#include "./command_color.h"
#include "game_alg.h"

void game_control::init() {
    int difficulty = 0;

    do {
        std::cout << "Difficulty: [0]Easy [1]Medium [2]Hard";
        std::cin >> difficulty;
    } while (difficulty < 0 || difficulty > 2);

    game_alg::generate(this->state, difficulty);
    this->start_time = time(nullptr);
}

void game_control::interactive(const std::string &extra, bool isWin = false) {
    if (!isWin) {
        std::cout << CYAN << "OPERATIONS: [I]INSERT  [D]Delete" << std::endl;
    }

    if (!extra.empty()) {
        std::cout << std::endl;
        std::cout << MAGENTA << extra << std::endl;
    }
}

void game_control::print() {
    std::cout << RESET << "U201816816 Sudoku Game" << std::endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (this->state.read_only[i][j]) {
                std::cout << BLUE << this->state.map[i][j];
            } else {
                if (this->state.map[i][j] == 0) {
                    std::cout << RED << "?";
                } else {
                    std::cout << GREEN << this->state.map[i][j];
                }
            }

            if (0 == (j + 1) % 3) {
                std::cout << " ";
            }
        }
        if (0 == (i + 1) % 3) {
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

game_control::game_control() : state(game_state()), start_time(0) {}

std::pair<int, int> game_control::unsafe_calc_offset(std::tuple<int, int, int> in) {
    const int startX[] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
    const int startY[] = {0, 3, 6, 0, 3, 6, 0, 3, 6};

    return {
            startX[std::get<0>(in) - 1] + (std::get<1>(in) - 1),
            startY[std::get<0>(in) - 1] + (std::get<2>(in) - 1)
    };
}

void game_control::loop() {
    bool isWin = false;
    std::string infoMessage;

    std::function<std::optional<int>()> input_value = [&infoMessage]() -> std::optional<int> {
        std::cout << YELLOW << "Input VALUE (1~9) : ";
        int val;
        std::cin >> val;
        if (val <= 0 || val >= 10) {
            infoMessage = "INVALID INPUT";
            return std::nullopt;
        }

        return std::optional<int>{val};
    };

    std::function<std::optional<std::tuple<int, int, int>>()> input_offset =
            [&infoMessage]() -> std::optional<std::tuple<int, int, int>> {
                int block, x, y;
                std::cout << std::endl << YELLOW;

                std::cout << "Input block Number (1~9) : ";
                std::cin >> block;
                if (block <= 0 || block >= 10) {
                    infoMessage = "INVALID INPUT";
                    return std::nullopt;
                }

                std::cout << "Input X axis offset : ";
                std::cin >> x;
                if (x <= 0 || x >= 4) {
                    infoMessage = "INVALID INPUT";
                    return std::nullopt;
                }

                std::cout << "Input Y axis offset : ";
                std::cin >> y;
                if (y <= 0 || y >= 4) {
                    infoMessage = "INVALID INPUT";
                    return std::nullopt;
                }

                return std::optional<std::tuple<int, int, int>>{{block, x, y}};
            };

    while (!isWin) {
        print();
        interactive(infoMessage);
        infoMessage = "";

        char ch = '\0';
        std::cin >> ch;
        if (ch == 'I' || ch == 'i') {
            std::optional<std::tuple<int, int, int>> ans = input_offset();

            if (ans == std::nullopt) {
                infoMessage = "INVALID INPUT";
                continue;
            }

            auto[x, y] = unsafe_calc_offset(ans.value());
            if (this->state.read_only[x][y]) {
                infoMessage = "INVALID INPUT";
                continue;
            } else {
                std::optional<int> in = input_value();

                if (in == std::nullopt) {
                    infoMessage = "INVALID INPUT";
                    continue;
                }

                if (this->state.map[x][y] == 0 && game_alg::can_place(state, x, y, in.value())) {
                    this->state.unsafe_place_number(x, y, in.value());

                    if (game_alg::check_win(state)) {
                        isWin = true;
                        unsigned int now_time = time(nullptr);

                        infoMessage = "You WIN in " + std::to_string(now_time - this->start_time) + " time!!!";
                    } else {
                        infoMessage = "Operate successfully";
                    }

                } else {
                    infoMessage = "INVALID INPUT";
                }
            }

        } else if (ch == 'D' || ch == 'd') {
            std::optional<std::tuple<int, int, int>> ans = input_offset();

            if (ans == std::nullopt) {
                infoMessage = "INVALID INPUT";
                continue;
            }

            auto[x, y] = unsafe_calc_offset(ans.value());
            if (this->state.read_only[x][y]) {
                infoMessage = "INVALID INPUT";
            } else {
                this->state.unsafe_remove_number(x, y);
                infoMessage = "Operate successfully";
            }

        } else {
            infoMessage = "INVALID INPUT";
        }
    }

    print();
    interactive(infoMessage, true);
}
