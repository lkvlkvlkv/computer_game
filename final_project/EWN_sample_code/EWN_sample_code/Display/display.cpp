#include "display.h"

#include <bits/stdc++.h>
using namespace std;

int Display::id2number[2][6] = {{0, 1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11}};
string Display::id2name[2][6] = {{"R0", "R1", "R2", "R3", "R4", "R5"}, {"B0", "B1", "B2", "B3", "B4", "B5"}};
string Display::dir2name[2][3] = {{"R", "D", "RD"}, {"L", "U", "LU"}};

void Display::printBoard(vector<vector<int>> &board, vector<vector<int>> &position, int turn, int n, int m) {
    cerr << "------------------------------------" << endl;
    cerr << "turn: " << turn << endl
         << endl;
    for (int i = 0; i < n; i++) {
        cerr << setw(4) << i;
        for (int j = 0; j < m; j++) {
            if (board[i][j] == -1) {
                cerr << setw(4) << "__";
                continue;
            }
            else if (board[i][j] < 6) {
                cerr << setw(4) << id2name[RED][board[i][j]];
            }
            else {
                cerr << setw(4) << id2name[BLUE][board[i][j] - 6];
            }
        }
        cerr << endl;
    }
    cerr << "\n    ";
    for (int j = 0; j < m; j++) {
        cerr << setw(4) << char('A' + j);
    }
    cerr << endl;
    cerr << "------------------------------------" << endl;
    cerr << "n: " << n << " m: " << m << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            cerr << position[i][j] << " ";
        }
        cerr << endl;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            if (position[i][j] == -1) {
                for (int x = 0; x < n; x++) {
                    for (int y = 0; y < m; y++) {
                        if (board[x][y] == id2number[i][j]) {
                            cerr << "Error(0): " << id2name[i][j] << " is not in position " << x << " " << y << endl;
                            exit(0);
                        }
                    }
                }
            }
            else {
                if (board[position[i][j] / m][position[i][j] % m] != id2number[i][j]) {
                    cerr << "Error(1): " << id2name[i][j] << " is not in position " << position[i][j] / m << " " << position[i][j] % m << endl;
                    exit(0);
                }
            }
        }
    }
}

void Display::showValidMove(int color, vector<array<int, 2>> &moves) {
    cerr << "Possible moves: " << endl;
    for (int i = 0; i < moves.size(); i++) {
        auto [id, dir] = moves[i];
        cerr << setw(2) << i + 1 << ") " << id2name[color][id] << " " << dir2name[color][dir] << endl;
    }
    cerr << endl;
}

void Display::announceMove(int turn, int id, int dir) {
    cerr << "Player " << turn % 2 << " move: " << Display().id2name[turn % 2][id] << " " << Display().dir2name[turn % 2][dir] << endl;
}

string Display::coordinates(int x, int y) {
    return string(1, char('A' + y)) + to_string(x + 1);
}