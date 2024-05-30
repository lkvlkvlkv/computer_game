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

    string getMove(vector<vector<int>> board, vector<vector<int>> position, int color, int remain[2], int dice, ofstream *file);

    int redScore(Game &game);

    int blueScore(Game &game);

    int evaluate(Game &game);

    void compare(Game &game, int depth, int color, int dice);

    array<int, 2> AlphaBetaSearch(Game &game, int depth, int color, int dice = -1);
    array<int,2> MAX_VALUE(Game &game, int depth, int color, int alpha, int beta, int dice = -1);
    array<int,2> MIN_VALUE(Game &game, int depth, int color, int alpha, int beta, int dice = -1);
    array<int, 2> NegaMax_VALUE(Game &game, int depth, int color, int alpha, int beta, int dice = -1);
    array<int, 2> NegaScoutSearch(Game &game, int depth, int color, int alpha, int beta, int dice = -1);

};

#endif