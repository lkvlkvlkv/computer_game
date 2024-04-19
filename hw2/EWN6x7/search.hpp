#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <bits/stdc++.h>
using namespace std;
#include "game.hpp"

class Search {
public:
    int depth;

    Search(int depth) {
        this->depth = depth;
    }

    int getMove(Game &game, int color, ofstream *file = nullptr) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        int computerMove = alphaBetaPruning(game, depth, INT_MIN, INT_MAX, color)[1];
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << endl;
        if (file != nullptr) {
            (*file) << depth << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;
        }
        return computerMove;
    }

    int redScore(Game &game) {
        int score = 0;
        for (int i = 0; i < 6; i++) {
            if (game.position[RED][i] != -1) {
                int x = game.position[RED][i] / game.m;
                int y = game.position[RED][i] % game.m;
                score += (i + 1) * (x + y);
                score += (i + 1) * 50;
            }
        }
        return score;
    }

    int blueScore(Game &game) {
        int score = 0;
        for (int i = 0; i < 6; i++) {
            if (game.position[BLUE][i] != -1) {
                int x = game.position[BLUE][i] / game.m;
                int y = game.position[BLUE][i] % game.m;
                score += (i + 1) * ((game.n - 1 - x) + (game.m - 1 - y));
                score += (i + 1) * 50;
            }
        }
        return score;
    }

    int evaluate(Game &game, int color) {
        if (game.isEnd()) {
            int winner = game.winner();
            if (winner == RED) {
                return INT_MAX;
            }
            else if (winner == BLUE) {
                return INT_MIN;
            }
            else {
                return 0;
            }
        }
        return redScore(game) - blueScore(game);
    }

    array<int, 2> alphaBetaPruning(Game &game, int depth, int alpha, int beta, int color) {
        if (depth == 0 || game.isEnd()) {
            return {evaluate(game, color), -1};
        }
        vector<array<int, 2>> moves = game.expand(color);
        if (color == 0) {
            int v = INT_MIN;
            int best = 0;
            if (moves.size() == 0) {
                return {alphaBetaPruning(game, depth - 1, alpha, beta, 1)[0], -1};
            }
            for (int i = moves.size() - 1; i >= 0; i--) {
                auto [id, dir] = moves[i];
                int original = game.move(id, dir, color);
                int result = alphaBetaPruning(game, depth - 1, alpha, beta, 1)[0];
                game.undo(id, dir, color, original);
                if (v < result) {
                    v = result;
                    best = i;
                }
                alpha = max(alpha, v);
                if (beta <= alpha) {
                    break;
                }
            }
            return {v, best};
        }
        else {
            int v = INT_MAX;
            int best = 0;
            if (moves.size() == 0) {
                return {alphaBetaPruning(game, depth - 1, alpha, beta, 0)[0], -1};
            }
            for (int i = moves.size() - 1; i >= 0; i--) {
                auto [id, dir] = moves[i];
                int original = game.move(id, dir, color);
                int result = alphaBetaPruning(game, depth - 1, alpha, beta, 0)[0];
                game.undo(id, dir, color, original);
                if (v > result) {
                    v = result;
                    best = i;
                }
                beta = min(beta, v);
                if (beta <= alpha) {
                    break;
                }
            }
            return {v, best};
        }
    }
};

#endif