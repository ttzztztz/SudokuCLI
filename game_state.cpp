#include <cstring>

#include "game_state.h"

game_state::game_state() : col{}, row{}, table{}, read_only{}, map{} {
    std::memset(this->map, 0xff, sizeof(this->map));
    std::memset(this->read_only, 0, sizeof(read_only));

    std::memset(this->col, 0, sizeof(col));
    std::memset(this->row, 0, sizeof(row));
    std::memset(this->table, 0, sizeof(table));

}

void game_state::unsafe_remove_number(int i, int j) {
    const int previousNumber = map[i][j];
    map[i][j] = -1;

    row[i] ^= 1 << (previousNumber - 1);
    col[j] ^= 1 << (previousNumber - 1);
    table[i][j] ^= 1 << (previousNumber - 1);
}

void game_state::unsafe_place_number(int i, int j, int number) {
    map[i][j] = number;

    row[i] |= 1 << (number - 1);
    col[j] |= 1 << (number - 1);
    table[i][j] |= 1 << (number - 1);
}

void game_state::unsafe_place_initial_number(int i, int j, int number) {
    unsafe_place_number(i, j, number);
    read_only[i][j] = true;
}