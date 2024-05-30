#include "game.h"

#include "../Display/display.h"
using namespace std;

int Game::dx[2][3] = {{0, 1, 1}, {0, -1, -1}};
int Game::dy[2][3] = {{1, 0, 1}, {-1, 0, -1}};

Game::Game() {
    this->n = 5;
    this->m = 5;
}

Game &Game::operator=(const Game &rhs) {
    if (this == &rhs) {
        return *this;
    }
    this->n = rhs.n;
    this->m = rhs.m;
    this->turn = rhs.turn;
    this->remain[RED] = rhs.remain[RED];
    this->remain[BLUE] = rhs.remain[BLUE];
    this->board = rhs.board;
    this->position = rhs.position;
    return *this;
}

bool Game::operator==(const Game &other) const {
    if (n != other.n || m != other.m || turn != other.turn) {
        return false;
    }
    if (remain[RED] != other.remain[RED] || remain[BLUE] != other.remain[BLUE]) {
        return false;
    }
    if (board != other.board || position != other.position) {
        return false;
    }
    return true;
}

bool Game::operator!=(const Game &other) const {
    return !(*this == other);
}

void Game::fromBoard(vector<vector<int>> &board, vector<vector<int>> &position, int turn, int remain[2]) {
    this->board = board;
    this->position = position;
    this->turn = turn;
    this->remain[RED] = remain[RED];
    this->remain[BLUE] = remain[BLUE];
}

vector<array<int, 2>> Game::expandNumber(int color, int dice) {
    if (dice < 0 || dice >= 6) {
        return {};
    }
    vector<array<int, 2>> result;
    int x = position[color][dice] / m;
    int y = position[color][dice] % m;
    for (int dir = 0; dir < 3; dir++) {
        int nx = x + dx[color][dir];
        int ny = y + dy[color][dir];
        if (nx >= n || ny >= m || nx < 0 || ny < 0) continue;
        result.push_back({dice, dir});
    }
    return result;
}

vector<array<int, 2>> Game::expand(int color, int dice) {
    vector<array<int, 2>> result;
    if (position[color][dice] != -1) {
        return expandNumber(color, dice);
    }
    // TODO
    // 用 int survice 存每個旗子的生死狀態
    // 用 prev[survice][dice] 存前面最近的旗子
    // 用 next[survice][dice] 存後面最近的旗子
    int prev = (dice - 1);
    while (prev >= 0 && position[color][prev] == -1) {
        prev = prev - 1;
    }
    int next = dice + 1;
    while (next < 6 && position[color][next] == -1) {
        next = next + 1;
    }
    result = expandNumber(color, prev);
    vector<array<int, 2>> temp = expandNumber(color, next);
    result.insert(result.end(), temp.begin(), temp.end());
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
    if (isBlue(0, 0) || isRed(n - 1, m - 1)) {
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
    if (isBlue(0, 0))
        return BLUE_WIN;

    if (isRed(n - 1, m - 1)) {
        return RED_WIN;
    }
    return DRAW;
}

int Game::move(int id, int dir, int color) {
    int px = position[color][id] / m;
    int py = position[color][id] % m;
    board[px][py] = -1;
    int x = px + dx[color][dir];
    int y = py + dy[color][dir];
    int original = board[x][y];
    if (!isEmpty(x, y)) {
        if (isRed(x, y)) {
            remain[RED]--;
            position[RED][original % 6] = -1;
        }
        else {
            remain[BLUE]--;
            position[BLUE][original % 6] = -1;
        }
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
        if (isRed(px, py)) {
            position[RED][original % 6] = px * m + py;
            remain[RED]++;
        }
        else {
            position[BLUE][original % 6] = px * m + py;
            remain[BLUE]++;
        }
    }
    position[color][id] = x * m + y;
    board[x][y] = Display().id2number[color][id];
    return original;
}

string Game::move2string(int color, int id, int dir) {
    int position = this->position[color][id];
    int x = position / m;
    int y = position % m;
    int nx = x + dx[color][dir];
    int ny = y + dy[color][dir];
    return Display().coordinates(x, y) + " " + Display().coordinates(nx, ny);
}