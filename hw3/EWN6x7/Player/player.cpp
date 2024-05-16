#include "player.h"
#include "../Display/display.h"
#include "../Search/search.h"
using namespace std;

Player::Player() {
    color = -1;
    computer = false;
}

Player::Player(int color, bool computer) {
    this->color = color;
    this->computer = computer;
}

Player* Player::createPlayer(int color) {
    int computer;
    cout << "Is player " << color << " computer? (0/1): ";
    cin >> computer;
    if (computer == 0) {
        return new HumanPlayer(color);
    }
    return new ComputerPlayer(color);
}

ComputerPlayer::ComputerPlayer(int color) : Player(color, true) {
    cout << "Do you want to write the depth / compute_time to a file? (0/1): ";
    int write;
    cin >> write;
    if (write) {
        file = new ofstream("output" + to_string(color) + ".txt");
    }
    else {
        file = nullptr;
    }
    cout << "Enter the depth: ";
    int depth;
    cin >> depth;
    search = Search(depth);
}

array<int, 2> HumanPlayer::getMove(vector<array<int, 2>> &moves, vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) {
    Display().showValidMove(color, moves);
    int move;
    cout << "Enter your move: ";
    cin >> move;
    return moves[move - 1];
}

array<int, 2> ComputerPlayer::getMove(vector<array<int, 2>> &moves, vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) {
    Display().showValidMove(color, moves);
    return moves[search.getMove(board, position, color, turn, remain, file)];
}