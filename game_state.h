#ifndef SUDOKU_GAME_STATE_H
#define SUDOKU_GAME_STATE_H

const unsigned int N = 9;
const unsigned int M = 3;

class game_state {
private:
    unsigned int map[N][N];
    bool read_only[N][N];
    unsigned int col[N], row[N], table[M][M];
public:
    game_state();

    void unsafe_place_number(unsigned int i, unsigned int j, unsigned int number);

    void unsafe_place_initial_number(unsigned int i, unsigned int j, unsigned int number);

    void unsafe_remove_number(unsigned int i, unsigned int j);

    friend class game_control;

    friend class game_alg;
};


#endif //SUDOKU_GAME_STATE_H
