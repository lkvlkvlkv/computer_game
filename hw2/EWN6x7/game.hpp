#ifndef GAME_HPP
#define GAME_HPP

#include <bits/stdc++.h>
using namespace std;

enum {
    RED = 0,
    BLUE = 1
};

class Game {
public:
    int n, m;
    int turn = 0;
    int id2number[2][6] = {{0, 1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11}};
    string id2name[2][6] = {{"R0", "R1", "R2", "R3", "R4", "R5"}, {"B0", "B1", "B2", "B3", "B4", "B5"}};
    vector<vector<int>> board;
    int position[2][6];
    int dx[2][3] = {{0, 1, 1}, {0, -1, -1}};
    int dy[2][3] = {{1, 0, 1}, {-1, 0, -1}};
    int remain[2] = {6, 6};
    string dir2name[2][3] = {{"R", "D", "RD"}, {"L", "U", "LU"}};

    Game(int n, int m, bool by_input = false) {
        this->n = n;
        this->m = m;
        if (by_input) {
            inputBoard();
        }
        else {
            randomBoard();
        }
    }

    vector<vector<int>> randomBoard() {
        board = vector<vector<int>>(n, vector<int>(m, -1));
        vector<int> red_start = {0, 1, 2, 7, 8, 14};
        vector<int> blue_start = {27, 33, 34, 39, 40, 41};
        random_device rd;
        std::mt19937 rng(rd());
        shuffle(red_start.begin(), red_start.end(), rng);
        shuffle(blue_start.begin(), blue_start.end(), rng);
        for (int i = 0; i < 6; i++) {
            board[red_start[i] / m][red_start[i] % m] = i;
            board[blue_start[i] / m][blue_start[i] % m] = i + 6;
            position[RED][i] = red_start[i];
            position[BLUE][i] = blue_start[i];
        }
        return board;
    }

    vector<vector<int>> inputBoard() {
        board = vector<vector<int>>(n, vector<int>(m, -1));

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 6; j++) {
                int x, y;
                cout << "Enter position of " << id2name[i][j] << " (x y): ";
                cin >> x >> y;
                board[x][y] = i * 6 + j;
                position[i][j] = x * m + y;
            }
        }
        return board;
    }

    void printBoard() {
        printBoard(board);
    }

    void printBoard(vector<vector<int>> &board) {
        cout << "------------------------------------" << endl;
        cout << "turn: " << turn << endl
             << endl;
        for (int i = 0; i < n; i++) {
            cout << setw(4) << i;
            for (int j = 0; j < m; j++) {
                if (board[i][j] == -1) {
                    cout << setw(4) << "__";
                    continue;
                }
                else if (board[i][j] < 6) {
                    cout << setw(4) << id2name[RED][board[i][j]];
                }
                else {
                    cout << setw(4) << id2name[BLUE][board[i][j] - 6];
                }
            }
            cout << endl;
        }
        cout << "\n    ";
        for (int j = 0; j < m; j++) {
            cout << setw(4) << char('A' + j);
        }
        cout << endl;
        cout << "------------------------------------" << endl;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 6; j++) {
                if (position[i][j] == -1) {
                    for (int x = 0; x < n; x++) {
                        for (int y = 0; y < m; y++) {
                            if (board[x][y] == id2number[i][j]) {
                                cout << "Error: " << id2name[i][j] << " is not in position " << x << " " << y << endl;
                                exit(0);
                            }
                        }
                    }
                }
                else {
                    if (board[position[i][j] / m][position[i][j] % m] != id2number[i][j]) {
                        cout << "Error: " << id2name[i][j] << " is not in position " << position[i][j] / m << " " << position[i][j] % m << endl;
                        exit(0);
                    }
                }
            }
        }
    }

    vector<array<int, 2>> expand(int color) {
        vector<array<int, 2>> result;
        for (int i = 0; i < 6; i++) {
            if (position[color][i] == -1) continue;
            int x = position[color][i] / m;
            int y = position[color][i] % m;
            for (int dir = 0; dir < 3; dir++) {
                int nx = x + dx[color][dir];
                int ny = y + dy[color][dir];
                if (nx >= n || ny >= m || nx < 0 || ny < 0) continue;
                if (isEmpty(nx, ny) || isColor(nx, ny, color ^ 1)) {
                    result.push_back({i, dir});
                }
            }
        }
        return result;
    }

    bool isColor(int x, int y, int color) {
        if (color == RED) {
            return isRed(x, y);
        }
        return isBlue(x, y);
    }

    bool isRed(int x, int y) {
        return board[x][y] != -1 && board[x][y] < 6;
    }

    bool isBlue(int x, int y) {
        return board[x][y] >= 6;
    }

    bool isEmpty(int x, int y) {
        return board[x][y] == -1;
    }

    bool redSurvice() {
        return remain[RED] > 0;
    }

    bool blueSurvice() {
        return remain[BLUE] > 0;
    }

    bool isEnd() {
        if (!redSurvice() || !blueSurvice()) {
            return true;
        }
        if (isBlue(0, 0) && isRed(n - 1, m - 1)) {
            return true;
        }
        return false;
    }

    int winner() {
        if (!blueSurvice()) {
            return 1;
        }
        if (!redSurvice()) {
            return 2;
        }
        if (isBlue(0, 0) && isRed(n - 1, m - 1)) {
            if (board[0][0] - 6 < board[n - 1][m - 1]) {
                return 1;
            }
            else if (board[0][0] - 6 > board[n - 1][m - 1]) {
                return 2;
            }
            else {
                return 0;
            }
        }
        return -1;
    }

    int move(int id, int dir, int color) {
        int px = position[color][id] / m;
        int py = position[color][id] % m;
        board[px][py] = -1;
        int x = px + dx[color][dir];
        int y = py + dy[color][dir];
        int original = board[x][y];
        if (!isEmpty(x, y)) {
            position[color ^ 1][original % 6] = -1;
            remain[color ^ 1]--;
        }
        position[color][id] = x * m + y;
        board[x][y] = id2number[color][id];
        return original;
    }

    int undo(int id, int dir, int color, int original) {
        int px = position[color][id] / m;
        int py = position[color][id] % m;
        board[px][py] = original;
        int x = px - dx[color][dir];
        int y = py - dy[color][dir];
        if (original != -1) {
            position[color ^ 1][original % 6] = px * m + py;
            remain[color ^ 1]++;
        }
        position[color][id] = x * m + y;
        board[x][y] = id2number[color][id];
        return original;
    }
};

#endif