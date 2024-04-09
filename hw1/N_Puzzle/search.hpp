#ifndef search_H
#define search_H

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "puzzle.hpp"

using namespace std;

class Search {
public:
    Search(Puzzle *puzzle) {
        this->puzzle = puzzle;
        now_state = 1, max_state = 1, ans_path = "", explored_state = 0;
    }

#define failure -1
#define cutoff 0
#define success 1

    int iterative_deepening_search_v2() {
        auto start_time = chrono::high_resolution_clock::now();
        int depth = puzzle->start_node->heuristic;
        depth += (puzzle->step() ^ depth) & 1;
        while (true) {
            string path = "";
            int res = depth_limited_search_v2(puzzle->start_node->state, depth, puzzle->start_node->heuristic, 0, puzzle->start_node->zero_index / puzzle->size, puzzle->start_node->zero_index % puzzle->size, path);
            if (res != cutoff) {
                auto end_time = chrono::high_resolution_clock::now();
                execution_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
                return res;
            }
            depth += 2;
        }
        return 0;
    }

    int depth_limited_search_v2(vector<__int8> &state, int limit, int heuristic, int depth, int x, int y, string &path) {
        int result = failure;
        explored_state++;
        max_state = max(max_state, now_state);
        if (heuristic == 0) {
            result = success;
            this->ans_path = path;
            return result;
        }
        if (depth + heuristic <= limit) {
            static int dx[] = {0, 0, 1, -1};
            static int dy[] = {1, -1, 0, 0};
            static char move_list[] = {'R', 'L', 'D', 'U'};
            static unordered_map<char, int> move_map = {{'R', 0}, {'L', 1}, {'D', 2}, {'U', 3}};
            for (int i = 0; i < 4; i++) {
                int next_x = x + dx[i];
                int next_y = y + dy[i];
                if (next_x < 0 || next_x >= puzzle->size || next_y < 0 || next_y >= puzzle->size)
                    continue;
                if (path.size() && (move_map[path.back()] + move_map[move_list[i]] == 1 || move_map[path.back()] + move_map[move_list[i]] == 5))
                    continue;
                now_state++;
                swap(state[x * puzzle->size + y], state[next_x * puzzle->size + next_y]);
                int num = state[x * puzzle->size + y];
                path += move_list[i];
                int next_h = heuristic + abs(puzzle->goal_x[num] - x) - abs(puzzle->goal_x[num] - next_x) + abs(puzzle->goal_y[num] - y) - abs(puzzle->goal_y[num] - next_y);
                int ret = depth_limited_search_v2(state, limit, next_h, depth + 1, next_x, next_y, path);
                swap(state[x * puzzle->size + y], state[next_x * puzzle->size + next_y]);
                if (ret > result) {
                    result = ret;
                    if (result == success) {
                        break;
                    }
                }
                now_state--;
                path.pop_back();
            }
        }
        else {
            result = cutoff;
        }
        return result;
    }

    void show_result(fs::path *output_path, bool print = false, bool display_path = false, bool write_file = true) {
        if (print) {
            cout << "Path: " << ans_path << endl;
            cout << "Max state: " << max_state << endl;
            cout << "Explored state: " << explored_state << endl;
            cout << "Time: " << this->execution_time << "ms" << endl;
            if (ans_path != "failure") {
                cout << "Number of steps: " << ans_path.size() << endl;
                if (display_path == true) {
                    print_path(output_path, true, false);
                }
                else {
                    cout << "-------------------------------" << endl;
                }
            }
        }
        if (write_file) {
            ofstream file = ofstream(*output_path, ios::app);
            file << "Path: " << ans_path << endl;
            file << "Max state: " << max_state << endl;
            file << "Explored state: " << explored_state << endl;
            file << "Time: " << this->execution_time << "ms" << endl;
            if (ans_path != "failure") {
                file << "Number of steps: " << ans_path.size() << endl;
                print_path(output_path, false, true);
            }
            file.close();
        }
    }

    void reset() {
        max_state = 1, ans_path = "failure", explored_state = 0;
        execution_time = 0;
    }

    void print_path(fs::path *output_path, bool print = false, bool write_file = false) {
        ofstream *file;
        if (write_file) {
            file = new ofstream(*output_path, ios::app);
        }
        if (ans_path.empty()) {
            if (print) {
                cout << "No path found" << endl;
            }
            if (write_file) {
                (*file) << "No path found" << endl;
            }
        }
        else {
            if (print) {
                cout << "Passed state:\n";
            }
            if (write_file) {
                (*file) << "Passed state:\n";
            }
            int step = 0;
            Node current = *(puzzle->start_node);
            for (int i = 0; i < ans_path.size(); i++) {
                char move = ans_path[i];
                if (print) {
                    cout << "Step " << step++ << ":\n";
                    current.repr(1, print, write_file, file);
                }
                if (write_file) {
                    (*file) << "Step " << step++ << ":\n";
                    current.repr(1, print, write_file, file);
                }
                current = puzzle->expand(current, move);
            }
            if (print) {
                cout << "Step " << step++ << ":\n";
                current.repr(1, print, write_file, file);
            }
            if (write_file) {
                (*file) << "Step " << step++ << ":\n";
                current.repr(1, print, write_file, file);
            }
        }
        if (write_file) {
            file->close();
        }
        return;
    }

    Puzzle *puzzle;
    string ans_path;
    long long max_state;
    long long now_state;
    long long explored_state;
    int64_t execution_time;
};

#endif