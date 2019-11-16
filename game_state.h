#ifndef SUDOKU_GAME_STATE_H
#define SUDOKU_GAME_STATE_H

const int N = 9;
const int M = 3;

class game_state {
private:
    int map[N][N];
    bool read_only[N][N];
    int col[N], row[N], table[M][M];
public:
    friend class game_control;

    friend class game_alg;

    game_state();

    void unsafe_place_number(int i, int j, int number);

    void unsafe_remove_number(int i, int j);
};


#endif //SUDOKU_GAME_STATE_H
