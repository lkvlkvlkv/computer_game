#include "search.h"
#include "../Game/game.h"
#include "../Display/display.h"
using namespace std;

const int MAX = 1e7;
const int MIN = -1e7;

Search::Search() {
    depth = 6;
}

Search::Search(int depth) {
    this->depth = depth;
}

int Search::getMove(vector<vector<int>> board, vector<vector<int>> position, int color, int turn, int remain[2], ofstream *file) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Game game = Game();
    game.fromBoard(board, position, turn, remain);
    auto [score, computerMove] = NegaScoutSearch(game, depth, color, MIN, MAX);
    // compare(game, depth, color);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << endl;
    if (file != nullptr) { 
        (*file) << depth << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;
    }
    return computerMove;
}

int Search::redScore(Game &game) {
    int score = 0;
    for (int i = 0; i < 6; i++) {
        if (game.position[RED][i] != -1) {
            int x = game.position[RED][i] / game.m;
            int y = game.position[RED][i] % game.m;
            score += (i + 1) * (x + y);
            score += (i + 1) * 10;
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
            score += (i + 1) * ((game.n - 1 - x) + (game.m - 1 - y));
            score += (i + 1) * 10;
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

array<int, 2> Search::AlphaBetaSearch(Game &game, int depth, int color) {
    if (color == RED) {
        auto [score, computerMove] = MAX_VALUE(game, depth, color, MIN, MAX);
        return {score, computerMove};
    }
    auto [score, computerMove] = MIN_VALUE(game, depth, color, MIN, MAX);
    return {score, computerMove};
}

array<int, 2> Search::MAX_VALUE(Game &game, int depth, int color, int alpha, int beta) {
    if (depth == 0 || game.isEnd()) {
        return {evaluate(game), -1};
    }
    vector<array<int, 2>> moves = game.expand(color);
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

array<int, 2> Search::MIN_VALUE(Game &game, int depth, int color, int alpha, int beta) {
    if (depth == 0 || game.isEnd()) {
        return {evaluate(game), -1};
    }
    vector<array<int, 2>> moves = game.expand(color);
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

array<int, 2> Search::alphaBetaPruning(Game &game, int depth, int alpha, int beta, int color) {
    if (depth == 0 || game.isEnd()) {
        return {evaluate(game), -1};
    }
    vector<array<int, 2>> moves = game.expand(color);
    if (color == 0) {
        int v = MIN;
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
        int v = MAX;
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

array<int, 2> Search::NegaMax_VALUE(Game &game, int depth, int color, int alpha, int beta) {
    if (depth == 0 || game.isEnd()) {
        return {(color == 0 ? 1 : -1) *evaluate(game), -1};
    }
    vector<array<int, 2>> moves = game.expand(color);
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

array<int, 2> Search::NegaScoutSearch(Game &game, int depth, int color, int alpha, int beta) {
    if (depth == 0 || game.isEnd()) {
        return {(color == 0 ? 1 : -1) * evaluate(game), -1};
    }
    vector<array<int, 2>> moves = game.expand(color);
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

void Search::compare(Game &game, int depth, int color) {
    auto [score1, computerMove1] = AlphaBetaSearch(game, depth, color);
    cout << setw(20) << "AlphaBetaSearch:" << "    Score: " << score1 << " Computer move: " << computerMove1 << endl;
    auto [score2, computerMove2] = NegaMax_VALUE(game, depth, color, MIN, MAX);
    cout << setw(20) << "NegaMax_VALUE:" << "    Score: " << score2 << " Computer move: " << computerMove2 << endl;
    auto [score3, computerMove3] = NegaScoutSearch(game, depth, color, MIN, MAX);
    cout << setw(20) << "NegaScoutSearch:" << "    Score: " << score3 << " Computer move: " << computerMove3 << endl;
    auto [score4, computerMove4] = alphaBetaPruning(game, depth, MIN, MAX, color);
    cout << setw(20) << "alphaBetaPruning:" << "    Score: " << score4 << " Computer move: " << computerMove4 << endl;
}
