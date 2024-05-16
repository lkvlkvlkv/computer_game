#ifndef SEARCH_H
#define SEARCH_H

#include <bits/stdc++.h>

using namespace std;

class Game;
class Search {
public:
    int depth;

    Search();

    Search(int depth);

    int getMove(vector<vector<int>> &board, vector<vector<int>> &position, int color, int turn, int remain[2], ofstream *file);

    int redScore(Game &game);

    int blueScore(Game &game);

    int evaluate(Game &game, int color);

    array<int, 2> alphaBetaPruning(Game &game, int depth, int alpha, int beta, int color);
};

#endif