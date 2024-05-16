#ifndef PLAYER_H
#define PLAYER_H

#include <bits/stdc++.h>

#include "../Display/display.h"
#include "../Search/search.h"
using namespace std;

class Game;
class Player {
public:
    int color;
    bool computer;

    Player();
    Player(int color);
    Player(int color, bool computer);

    virtual array<int, 2> getMove(vector<array<int, 2>> &moves, vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) = 0;
    static Player *createPlayer(int color);
};

class HumanPlayer : public Player {
public:
    HumanPlayer(int color) : Player(color, false){};

    array<int, 2> getMove(vector<array<int, 2>> &moves, vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) override;
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(int color);

    array<int, 2> getMove(vector<array<int, 2>> &moves, vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) override;

private:
    Search search;
    ofstream *file;
};

#endif