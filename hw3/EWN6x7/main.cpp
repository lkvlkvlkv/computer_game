#include <bits/stdc++.h>
using namespace std;
#include "game.hpp"
#include "player.hpp"
#include "search.hpp"

int main() {
    Game game(6, 7);
    Search search(8);
    Player players[2] = {Player(0), Player(1)};
    while (!game.isEnd()) {
        game.printBoard();
        if (game.expand(players[game.turn % 2].color).size()) {
            auto [id, dir] = players[game.turn % 2].getMove(game, &search);
            game.move(id, dir, players[game.turn % 2].color);
        }
        else {
            cout << "No possible move..." << endl;
        }
        game.turn++;
    }
    game.printBoard();
    cout << "Game over: ";
    if (game.winner() == 0) {
        cout << "Draw" << endl;
    }
    else if (game.winner() == 1) {
        cout << "Red win" << endl;
    }
    else {
        cout << "Blue win" << endl;
    }
    return 0;
}