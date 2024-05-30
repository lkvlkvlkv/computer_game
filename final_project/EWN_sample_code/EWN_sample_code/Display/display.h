#ifndef DISPLAY_H
#define DISPLAY_H

#include <bits/stdc++.h>

using namespace std;

enum {
    RED = 0,
    BLUE = 1
};

enum {
    DRAW = 0,
    RED_WIN = 1,
    BLUE_WIN = 2,
    IN_PROGRESS = 3
};

class Display {
public:
    static void printBoard(vector<vector<int>> &board, vector<vector<int>> &position, int turn, int n, int m);

    static void showValidMove(int color, vector<array<int, 2>> &moves);

    static void announceMove(int turn, int id, int dir);

    static string coordinates(int x, int y);

    static int id2number[2][6];
    static string id2name[2][6];
    static string dir2name[2][3];
};

#endif