#include <cstring>
#include <iostream>
#include <fstream>
#include <tuple>
#include <optional>

#include "game_control.h"
#include "./command_color.h"
#include "game_alg.h"


void game_control::control() {
    char ch = '\0';
    std::cin >> ch;
    if (ch == 'I' || ch == 'i') {
        std::optional<std::tuple<int, int, int>> ans = input_offset();

        if (ans == std::nullopt) {
            print_notice("無効なコマンド");
            return;
        }

        auto[x, y] = unsafe_calcOffset(ans.value());
        if (readOnly[x][y]) {
            print_notice("無効なコマンド");
        } else {
            std::optional<int> in = input_value();
            if (in == std::nullopt) {
                print_notice("無効なコマンド");
                return;
            }

            map[x][y] = in.value();
            if (game_alg::check_valid(this)) {
                print_notice("操作の成功");
            } else {
                map[x][y] = -1;
                print_notice("無効なコマンド");
            }
        }

    } else if (ch == 'D' || ch == 'd') {
        std::optional<std::tuple<int, int, int>> ans = input_offset();

        if (ans == std::nullopt) {
            print_notice("無効なコマンド");
            return;
        }

        auto[x, y] = unsafe_calcOffset(ans.value());
        if (readOnly[x][y]) {
            print_notice("無効なコマンド");
        } else {
            print_notice("操作の成功");
        }

    } else {
        print_notice("無効なコマンド");
    }
}

void game_control::init() {
    std::fstream fs;
    fs.open("./test.in");
    if (fs.is_open()) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                char ch = '\0';
                fs >> ch;

                if (ch == '.') {
                    map[i][j] = -1;
                } else {
                    map[i][j] = ch - '1';
                    readOnly[i][j] = true;
                }
            }
        }
    } else {
        throw std::runtime_error("invalid file");
    }
}

void game_control::interactive(const std::string &extra) {
    std::cout << CYAN << "コマンド:" << std::endl;
    std::cout << "I: 一つの数を追加" << std::endl;
    std::cout << "D: 一つの数を削除" << std::endl;

    if (!extra.empty()) {
        std::cout << std::endl << std::endl;
        std::cout << MAGENTA << extra;
    }
}

void game_control::print() {
    std::cout << "U201816816 の すうどくゲーム" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (readOnly[i][j]) {
                std::cout << BLUE << map[i][j];
            } else {
                std::cout << RED;
                if (map[i][j] == -1) {
                    std::cout << "?";
                } else {
                    std::cout << map[i][j];
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

game_control::game_control() : map{}, readOnly{} {
    std::memset(this->map, 0xff, sizeof(this->map));
    std::memset(this->readOnly, 0, sizeof(readOnly));
}

std::optional<std::tuple<int, int, int>> game_control::input_offset() {
    int block, x, y;
    std::cout << std::endl << YELLOW;

    std::cout << "チェック番号 (1~9) を入力してください" << std::endl;
    std::cin >> block;
    if (block <= 0 || block >= 10) {
        print_notice("無効な入力");
        return std::nullopt;
    }

    std::cout << "相対的な横軸 (1~3) を入力してください" << std::endl;
    std::cin >> x;
    if (x <= 0 || x >= 4) {
        print_notice("無効な入力");
        return std::nullopt;
    }

    std::cout << "相対的な縦軸 (1~3) を入力してください" << std::endl;
    std::cin >> y;
    if (y <= 0 || y >= 4) {
        print_notice("無効な入力");
        return std::nullopt;
    }

    return std::optional<std::tuple<int, int, int>>{{block, x, y}};
}

std::pair<int, int> game_control::unsafe_calcOffset(std::tuple<int, int, int> in) {
    const int startX[] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
    const int startY[] = {0, 3, 6, 0, 3, 6, 0, 3, 6};

    return {
            startX[std::get<0>(in) - 1] + (std::get<1>(in) - 1),
            startY[std::get<0>(in) - 1] + (std::get<2>(in) - 1)
    };
}

void game_control::print_notice(const std::string &extra) {
    print();
    interactive(extra);
}

std::optional<int> game_control::input_value() {
    int val;
    std::cin >> val;
    if (val <= 0 || val >= 10) {
        print_notice("無効な入力");
        return std::nullopt;
    }

    return std::optional<int>{val};
}
