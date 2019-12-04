#include <cstring>

#include "game_state.h"

game_state::game_state() : col{}, row{}, table{}, read_only{}, map{} {
    std::memset(this->map, 0, sizeof(this->map));
    std::memset(this->read_only, 0, sizeof(read_only));

    std::memset(this->col, 0, sizeof(col));
    std::memset(this->row, 0, sizeof(row));
    std::memset(this->table, 0, sizeof(table));
}

void game_state::unsafe_remove_number(unsigned int i, unsigned int j) {
    const unsigned int previousNumber = map[i][j];
    map[i][j] = 0;

    row[i] ^= 1u << (previousNumber - 1);
    col[j] ^= 1u << (previousNumber - 1);
    table[i / 3][j / 3] ^= 1u << (previousNumber - 1);
}

void game_state::unsafe_place_number(unsigned int i, unsigned int j, unsigned int number) {
    map[i][j] = number;

    row[i] |= 1u << (number - 1);
    col[j] |= 1u << (number - 1);
    table[i / 3][j / 3] |= 1u << (number - 1);
}

void game_state::unsafe_place_initial_number(unsigned int i, unsigned int j, unsigned int number) {
    unsafe_place_number(i, j, number);
    read_only[i][j] = true;
}