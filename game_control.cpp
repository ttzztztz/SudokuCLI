#include <iostream>
#include <fstream>
#include <tuple>
#include <optional>

#include "game_control.h"
#include "./command_color.h"
#include "game_alg.h"

void game_control::init() {
    std::fstream fs;
    fs.open("./test.in");
    if (fs.is_open()) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                char ch = '\0';
                fs >> ch;

                if (ch != '.') {
                    this->state.unsafe_place_number(i, j, ch - '0');
                    this->state.read_only[i][j] = true;
                }
            }
        }
    } else {
        throw std::runtime_error("invalid file");
    }
//    game_alg::generate(this->state);
}

void game_control::interactive(const std::string &extra, bool isWin = false) {
    if (!isWin) {
        std::cout << CYAN << "コマンド:" << std::endl;
        std::cout << "I: 一つの数を追加" << std::endl;
        std::cout << "D: 一つの数を削除" << std::endl;
    }

    if (!extra.empty()) {
        std::cout << std::endl;
        std::cout << MAGENTA << extra << std::endl;
    }
}

void game_control::print() {
    std::cout << RESET << "U201816816 の すうどくゲーム" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (this->state.read_only[i][j]) {
                std::cout << BLUE << this->state.map[i][j];
            } else {
                if (this->state.map[i][j] == -1) {
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

game_control::game_control() : state(game_state()) {}

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

    auto input_value = [&infoMessage]() -> std::optional<int> {
        std::cout << YELLOW << "数値 (1~9) を入力してください : ";
        int val;
        std::cin >> val;
        if (val <= 0 || val >= 10) {
            infoMessage = "無効な入力";
            return std::nullopt;
        }

        return std::optional<int>{val};
    };

    auto input_offset = [&infoMessage]() -> std::optional<std::tuple<int, int, int>> {
        int block, x, y;
        std::cout << std::endl << YELLOW;

        std::cout << "チェック番号 (1~9) を入力してください : ";
        std::cin >> block;
        if (block <= 0 || block >= 10) {
            infoMessage = "無効な入力";
            return std::nullopt;
        }

        std::cout << "相対的な横軸 (1~3) を入力してください : ";
        std::cin >> x;
        if (x <= 0 || x >= 4) {
            infoMessage = "無効な入力";
            return std::nullopt;
        }

        std::cout << "相対的な縦軸 (1~3) を入力してください : ";
        std::cin >> y;
        if (y <= 0 || y >= 4) {
            infoMessage = "無効な入力";
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
                infoMessage = "無効なコマンド";
                continue;
            }

            auto[x, y] = unsafe_calc_offset(ans.value());
            if (this->state.read_only[x][y]) {
                infoMessage = "無効なコマンド";
                continue;
            } else {
                std::optional<int> in = input_value();

                if (in == std::nullopt) {
                    infoMessage = "無効なコマンド";
                    continue;
                }

                if (this->state.map[x][y] == -1 && game_alg::can_place(state, x, y, in.value())) {
                    this->state.unsafe_place_number(x, y, in.value());

                    if (game_alg::check_win(state)) {
                        isWin = true;
                        infoMessage = "勝利おめでとうございます";
                    } else {
                        infoMessage = "操作の成功";
                    }

                } else {
                    infoMessage = "無効なコマンド";
                }
            }

        } else if (ch == 'D' || ch == 'd') {
            std::optional<std::tuple<int, int, int>> ans = input_offset();

            if (ans == std::nullopt) {
                infoMessage = "無効なコマンド";
                continue;
            }

            auto[x, y] = unsafe_calc_offset(ans.value());
            if (this->state.read_only[x][y]) {
                infoMessage = "無効なコマンド";
            } else {
                this->state.unsafe_remove_number(x, y);
                infoMessage = "操作の成功";
            }

        } else {
            infoMessage = "無効なコマンド";
        }
    }

    print();
    interactive(infoMessage, true);
}
