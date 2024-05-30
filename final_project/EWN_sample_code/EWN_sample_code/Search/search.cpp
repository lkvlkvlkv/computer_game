#include "search.h"

#include "../Display/display.h"
#include "../Game/game.h"
using namespace std;

const int MAX = 1e7;
const int MIN = -1e7;

Search::Search() {
    depth = 6;
}

Search::Search(int depth) {
    this->depth = depth;
}

string Search::getMove(vector<vector<int>> board, vector<vector<int>> position, int color, int remain[2], int dice, ofstream *file) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Game game = Game();
    game.fromBoard(board, position, 0, remain);
    // cerr << "Depth: " << depth << endl;
    // cerr << "Color: " << color << endl;
    // cerr << "Dice: " << dice << endl;
    // Display().printBoard(board, position, 0, 5, 5);
    // auto [score, computerMove] = NegaMax_VALUE(game, depth, color, dice);
    auto [score, computerMove] = NegaMax_VALUE(game, depth, color, MIN, MAX, dice);
    // compare(game, depth, color, dice);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cerr << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << endl;
    if (file != nullptr) {
        (*file) << depth << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;
    }
    auto [id, dir] = game.expand(color, dice)[computerMove];
    return game.move2string(color, id, dir);
    return "";
}

int Search::redScore(Game &game) {
    int score = 0;
    for (int i = 0; i < 6; i++) {
        if (game.position[RED][i] != -1) {
            int x = game.position[RED][i] / game.m;
            int y = game.position[RED][i] % game.m;
            score += (x + y);
            score += 3;
        }
    }
    return score;
}

int Search::blueScore(Game &game) {
    int score = 0;
    for (int i = 0; i < 6; i++) {
        if (game.position[BLUE][i] != -1) {
            int x = game.position[BLUE][i] / game.m;
            int y = game.position[BLUE][i] % game.m;
            score += (game.n - 1 - x) + (game.m - 1 - y);
            score += 3;
        }
    }
    return score;
}

int Search::evaluate(Game &game) {
    if (game.isEnd()) {
        int winner = game.winner();
        if (winner == RED_WIN) {
            return MAX;
        }
        else if (winner == BLUE_WIN) {
            return MIN;
        }
        else {
            return DRAW;
        }
    }
    return redScore(game) - blueScore(game);
}

array<int, 2> Search::AlphaBetaSearch(Game &game, int depth, int color, int dice) {
    if (color == RED) {
        auto [score, computerMove] = MAX_VALUE(game, depth, color, MIN, MAX, dice);
        return {score, computerMove};
    }
    auto [score, computerMove] = MIN_VALUE(game, depth, color, MIN, MAX, dice);
    return {score, computerMove};
}

array<int, 2> Search::MAX_VALUE(Game &game, int depth, int color, int alpha, int beta, int dice) {
    if (depth == 0 || game.isEnd()) {
        return {evaluate(game), -1};
    }
    if (dice == -1) {
        int ret = 0;
        for (int i = 0; i < 6; i++) {
            ret += MAX_VALUE(game, depth, color, alpha, beta, i)[0] / 6;
        }
        return {ret, -1};
    }
    vector<array<int, 2>> moves = game.expand(color, dice);
    if (moves.size() == 0) {
        return {MIN_VALUE(game, depth - 1, color ^ 1, alpha, beta)[0], -1};
    }
    int v = MIN;
    int best = 0;
    for (int i = moves.size() - 1; i >= 0; i--) {
        auto [id, dir] = moves[i];
        int original = game.move(id, dir, color);
        int result = MIN_VALUE(game, depth - 1, color ^ 1, alpha, beta)[0];
        game.undo(id, dir, color, original);
        if (result > v) {
            v = result;
            best = i;
        }
        if (v >= beta) {
            return {v, best};
        }
        alpha = max(alpha, v);
    }
    return {v, best};
}

array<int, 2> Search::MIN_VALUE(Game &game, int depth, int color, int alpha, int beta, int dice) {
    if (depth == 0 || game.isEnd()) {
        return {evaluate(game), -1};
    }
    if (dice == -1) {
        int ret = 0;
        for (int i = 0; i < 6; i++) {
            ret += MIN_VALUE(game, depth, color, alpha, beta, i)[0] / 6;
        }
        return {ret, -1};
    }
    vector<array<int, 2>> moves = game.expand(color, dice);
    if (moves.size() == 0) {
        return {MAX_VALUE(game, depth - 1, color ^ 1, alpha, beta)[0], -1};
    }
    int v = MAX;
    int best = 0;
    for (int i = moves.size() - 1; i >= 0; i--) {
        auto [id, dir] = moves[i];
        int original = game.move(id, dir, color);
        int result = MAX_VALUE(game, depth - 1, color ^ 1, alpha, beta)[0];
        game.undo(id, dir, color, original);
        if (result < v) {
            v = result;
            best = i;
        }
        if (v <= alpha) {
            return {v, best};
        }
        beta = min(beta, v);
    }
    return {v, best};
}

array<int, 2> Search::NegaMax_VALUE(Game &game, int depth, int color, int alpha, int beta, int dice) {
    if (depth == 0 || game.isEnd()) {
        return {(color == 0 ? 1 : -1) * evaluate(game), -1};
    }
    // TODO
    // 看可不可以不重複移動同個數字
    // 最後再用機率表
    // prob[survive][dice] 存機率
    // 盼望可以回到 8 步深度
    if (dice == -1) {
        int ret = 0;
        for (int i = 0; i < 6; i++) {
            ret += NegaMax_VALUE(game, depth, color, alpha, beta, i)[0] / 6;
        }
        return {ret, -1};
    }
    vector<array<int, 2>> moves = game.expand(color, dice);
    if (moves.size() == 0) {
        return {-NegaMax_VALUE(game, depth - 1, color ^ 1, -beta, -alpha)[0], -1};
    }
    int v = MIN;
    int best = 0;
    for (int i = moves.size() - 1; i >= 0; i--) {
        auto [id, dir] = moves[i];
        int original = game.move(id, dir, color);
        int result = -NegaMax_VALUE(game, depth - 1, color ^ 1, -beta, -alpha)[0];
        game.undo(id, dir, color, original);
        if (result > v) {
            v = result;
            best = i;
        }
        if (v >= beta) {
            return {v, best};
        }
        alpha = max(alpha, v);
    }
    return {v, best};
}

array<int, 2> Search::NegaScoutSearch(Game &game, int depth, int color, int alpha, int beta, int dice) {
    if (depth == 0 || game.isEnd()) {
        return {(color == 0 ? 1 : -1) * evaluate(game), -1};
    }
    if (dice == -1) {
        int ret = 0;
        for (int i = 0; i < 6; i++) {
            ret += NegaScoutSearch(game, depth, color, alpha, beta, i)[0] / 6;
        }
        return {ret, -1};
    }
    vector<array<int, 2>> moves = game.expand(color, dice);
    if (moves.size() == 0) {
        return {-NegaScoutSearch(game, depth - 1, color ^ 1, -beta, -alpha)[0], -1};
    }
    bool fFoundPV = false;
    int best = 0;
    for (int i = moves.size() - 1; i >= 0; i--) {
        auto [id, dir] = moves[i];
        int original = game.move(id, dir, color);
        int result;
        if (fFoundPV) {
            result = -NegaScoutSearch(game, depth - 1, color ^ 1, -alpha - 1, -alpha)[0];
            if (result > alpha && result < beta) {
                result = -NegaScoutSearch(game, depth - 1, color ^ 1, -beta, -alpha)[0];
            }
        }
        else {
            result = -NegaScoutSearch(game, depth - 1, color ^ 1, -beta, -alpha)[0];
        }
        game.undo(id, dir, color, original);
        if (result > alpha) {
            alpha = result;
            fFoundPV = true;
            best = i;
        }
        if (result >= beta) {
            return {beta, best};
        }
    }
    return {alpha, best};
}

void Search::compare(Game &game, int depth, int color, int dice) {
    auto [score1, computerMove1] = AlphaBetaSearch(game, depth, color, dice);
    cerr << setw(20) << "AlphaBetaSearch:" << "    Score: " << score1 << " Computer move: " << computerMove1 << endl;
    auto [score2, computerMove2] = NegaMax_VALUE(game, depth, color, MIN, MAX, dice);
    cerr << setw(20) << "NegaMax_VALUE:" << "    Score: " << score2 << " Computer move: " << computerMove2 << endl;
    auto [score3, computerMove3] = NegaScoutSearch(game, depth, color, MIN, MAX, dice);
    cerr << setw(20) << "NegaScoutSearch:" << "    Score: " << score3 << " Computer move: " << computerMove3 << endl;
}
