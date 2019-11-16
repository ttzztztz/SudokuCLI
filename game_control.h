#include <optional>
#include <tuple>

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

const int N = 9;
const int M = 3;

class game_control {
private:
    int map[N][N];
    bool readOnly[N][N];

    std::pair<int, int> unsafe_calcOffset(std::tuple<int, int, int> in);

    void print();

    void interactive(const std::string &extra);

public:
    friend class game_alg;

    game_control();

    void print_notice(const std::string &extra);

    void init();

    void control();

    std::optional<std::tuple<int, int, int>> input_offset();

    std::optional<int> input_value();
};


#endif //SUDOKU_GAME_H
