#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <bits/stdc++.h>
using namespace std;
#include "game.hpp"
#include "search.hpp"

class Player {
public:
    int color;
    bool computer;

    Player(int color) {
        this->color = color;
        cout << "Is player " << color << " computer? (0/1): ";
        cin >> computer;
    }

    Player(int color, bool computer) {
        this->color = color;
        this->computer = computer;
    }

    void showMove(Game &game, vector<array<int, 2>> &moves) {
        cout << "Possible moves: " << endl;
        for (int i = 0; i < moves.size(); i++) {
            auto [id, dir] = moves[i];
            cout << setw(2) << i + 1 << ") " << game.id2name[color][id] << " " << game.dir2name[color][dir] << endl;
        }
        cout << endl;
    }

    array<int, 2> getMove(Game &game, Search *search = nullptr, ofstream *file = nullptr) {
        if (computer) {
            array<int, 2> move = computerMove(game, search, file);
            cout << "Computer move: " << game.id2name[color][move[0]] << " " << game.dir2name[color][move[1]] << endl;
            return move;
        }
        array<int, 2> move = playerMove(game);
        cout << "Player move: " << game.id2name[color][move[0]] << " " << game.dir2name[color][move[1]] << endl;
        return move;
    }

    array<int, 2> playerMove(Game &game) {
        vector<array<int, 2>> moves = game.expand(color);
        showMove(game, moves);
        int move;
        cout << "Enter your move: ";
        cin >> move;
        return moves[move - 1];
    }

    array<int, 2> computerMove(Game &game, Search *search = nullptr, ofstream *file = nullptr) {
        vector<array<int, 2>> moves = game.expand(color);
        showMove(game, moves);
        return moves[search->getMove(game, color, file)];
    }
};

#endif