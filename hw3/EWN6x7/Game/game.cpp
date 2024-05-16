#include "game.h"
#include "../Player/player.h"
#include "../Display/display.h"
using namespace std;

int Game::dx[2][3] = {{0, 1, 1}, {0, -1, -1}};
int Game::dy[2][3] = {{1, 0, 1}, {-1, 0, -1}};

Game::Game() {
    this->n = 6;
    this->m = 7;
}

void Game::initGame(bool by_input) {
    turn = 0;
    remain[RED] = remain[BLUE] = 6;

    for (int i = 0; i < 2; i++) {
        players[i] = Player::createPlayer(i);
    }
    if (by_input) {
        inputBoard();
    }
    else {
        randomBoard();
    }
}

void Game::fromBoard(vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) {
    this->board = board;
    this->position = position;
    this->turn = turn;
    this->remain[RED] = remain[RED];
    this->remain[BLUE] = remain[BLUE];
}

void Game::startGame() {
    while (!isEnd()) {
        Display().printBoard(board, position, turn, n, m);
        vector<array<int, 2>> moves = expand(turn % 2);
        if (moves.size()) {
            auto [id, dir] = players[turn % 2]->getMove(moves, board, position, turn, remain);
            move(id, dir, turn % 2);
            Display().announceMove(turn % 2, id, dir);
        }
        else {
            cout << "No possible move..." << endl;
        }
        turn++;
    }
}

void Game::endGame() {
    Display().printBoard(board, position, turn, n, m);
    cout << "Game over: ";
    int w = winner();

    if (w == DRAW) {
        cout << "Draw" << endl;
    }
    else if (w == RED_WIN) {
        cout << "Red win" << endl;
    }
    else if (w == BLUE_WIN) {
        cout << "Blue win" << endl;
    }
    else {
        cout << "Game is in progress or error occur" << endl;
    }
}

void Game::randomBoard() {
    position = vector<vector<int>>(2, vector<int>(6, -1));
    board = vector<vector<int>>(n, vector<int>(m, -1));
    vector<int> red_start = {0, 1, 2, 7, 8, 14};
    vector<int> blue_start = {27, 33, 34, 39, 40, 41};
    random_device rd;
    std::mt19937 rng(rd());
    shuffle(red_start.begin(), red_start.end(), rng);
    shuffle(blue_start.begin(), blue_start.end(), rng);
    for (int i = 0; i < 6; i++) {
        board[red_start[i] / m][red_start[i] % m] = i;
        board[blue_start[i] / m][blue_start[i] % m] = i + 6;
        position[RED][i] = red_start[i];
        position[BLUE][i] = blue_start[i];
    }
    return;
}

void Game::inputBoard() {
    position = vector<vector<int>>(2, vector<int>(6, -1));
    board = vector<vector<int>>(n, vector<int>(m, -1));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            int x, y;
            cout << "Enter position of " << Display().id2name[i][j] << " (x y): ";
            cin >> x >> y;
            board[x][y] = i * 6 + j;
            position[i][j] = x * m + y;
        }
    }
    return;
}


vector<array<int, 2>> Game::expand(int color) {
    vector<array<int, 2>> result;
    for (int i = 0; i < 6; i++) {
        if (position[color][i] == -1) continue;
        int x = position[color][i] / m;
        int y = position[color][i] % m;
        for (int dir = 0; dir < 3; dir++) {
            int nx = x + dx[color][dir];
            int ny = y + dy[color][dir];
            if (nx >= n || ny >= m || nx < 0 || ny < 0) continue;
            if (isEmpty(nx, ny) || isColor(nx, ny, color ^ 1)) {
                result.push_back({i, dir});
            }
        }
    }
    return result;
}

bool Game::isColor(int x, int y, int color) {
    if (color == RED) {
        return isRed(x, y);
    }
    return isBlue(x, y);
}

bool Game::isRed(int x, int y) {
    return board[x][y] != -1 && board[x][y] < 6;
}

bool Game::isBlue(int x, int y) {
    return board[x][y] >= 6;
}

bool Game::isEmpty(int x, int y) {
    return board[x][y] == -1;
}

bool Game::redSurvice() {
    return remain[RED] > 0;
}

bool Game::blueSurvice() {
    return remain[BLUE] > 0;
}

bool Game::isEnd() {
    if (!redSurvice() || !blueSurvice()) {
        return true;
    }
    if (isBlue(0, 0) && isRed(n - 1, m - 1)) {
        return true;
    }
    return false;
}

int Game::winner() {
    if (!blueSurvice()) {
        return RED_WIN;
    }
    if (!redSurvice()) {
        return BLUE_WIN;
    }
    if (isBlue(0, 0) && isRed(n - 1, m - 1)) {
        if (board[0][0] - 6 < board[n - 1][m - 1]) {
            return RED_WIN;
        }
        else if (board[0][0] - 6 > board[n - 1][m - 1]) {
            return BLUE_WIN;
        }
        else {
            return DRAW;
        }
    }
    return IN_PROGRESS;
}

int Game::move(int id, int dir, int color) {
    int px = position[color][id] / m;
    int py = position[color][id] % m;
    board[px][py] = -1;
    int x = px + dx[color][dir];
    int y = py + dy[color][dir];
    int original = board[x][y];
    if (!isEmpty(x, y)) {
        position[color ^ 1][original % 6] = -1;
        remain[color ^ 1]--;
    }
    position[color][id] = x * m + y;
    board[x][y] = Display().id2number[color][id];
    return original;
}

int Game::undo(int id, int dir, int color, int original) {
    int px = position[color][id] / m;
    int py = position[color][id] % m;
    board[px][py] = original;
    int x = px - dx[color][dir];
    int y = py - dy[color][dir];
    if (original != -1) {
        position[color ^ 1][original % 6] = px * m + py;
        remain[color ^ 1]++;
    }
    position[color][id] = x * m + y;
    board[x][y] = Display().id2number[color][id];
    return original;
}