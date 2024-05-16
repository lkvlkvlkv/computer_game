#include <bits/stdc++.h>
using namespace std;
#include "game.hpp"
#include "player.hpp"
#include "search.hpp"

int main() {
    int test_depth = 9;
    // cout << "Enter search depth limit: ";
    // cin >> test_depth;
    vector<Search> searches;
    for (int i = 1; i <= test_depth; i++) {
        searches.push_back(Search(i));
    }
    Game game(6, 7);
    Player players[2] = {Player(0, true), Player(1, true)};
    ofstream *file = new ofstream("log.txt", ios::app);
    for (int i = 0; i < 10; i++) {
        game = Game(6, 7);
        while (!game.isEnd()) {
            game.printBoard();
            if (game.expand(players[game.turn % 2].color).size()) {
                for (int i = 0; i + 1 < searches.size(); i++) {
                    auto [id, dir] = players[game.turn % 2].getMove(game, &searches[i], file);
                }
                auto [id, dir] = players[game.turn % 2].getMove(game, &searches.back(), file);
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
    }
    return 0;
}