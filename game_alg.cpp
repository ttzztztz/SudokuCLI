#include <cstring>

#include "game_alg.h"

bool game_alg::check_valid(game_control *control) {
    int col[N], row[N], table[M][M];
    memset(col, 0, sizeof(col));
    memset(row, 0, sizeof(row));
    memset(table, 0, sizeof(table));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const int number = control->map[i][j];
            if (number == -1) {
                continue;
            }

            const int bit = 1 << (number - 1);
            if ((row[i] & bit) || (col[j] & bit) || (table[i / 3][j / 3] & bit)) {
                return false;
            }

            row[i] |= bit, col[j] |= bit, table[i / 3][j / 3] |= bit;
        }
    }
    return true;
}
