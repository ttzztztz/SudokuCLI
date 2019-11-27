#include <optional>
#include <tuple>
#include "game_state.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

class game_control {
private:
    game_state state;

    static std::pair<int, int> unsafe_calc_offset(std::tuple<int, int, int> in);

    void print();

    static void interactive(const std::string &extra, bool isWin);

public:
    friend class game_alg;

    game_control();

    void init();

    void loop();

};


#endif //SUDOKU_GAME_H
