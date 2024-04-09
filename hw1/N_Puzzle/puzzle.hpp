#ifndef puzzle_H
#define puzzle_H

#include <bits/stdc++.h>

#include "node.hpp"

using namespace std;
namespace fs = std::filesystem;

class Puzzle {
public:
    Puzzle(int size) {
        this->size = size;
    }

    vector<__int8> read_vector(vector<int> &_board) {
        vector<__int8> board;
        for (auto &x : _board) {
            board.push_back(static_cast<__int8>(x));
        }
        return board;
    }

    vector<__int8> read_file(fs::path file_path) {
        ifstream file(file_path);
        vector<__int8> board;
        int _size;
        file >> _size;
        for (int i = 0; i < size * size; i++) {
            int x;
            file >> x;
            board.push_back(static_cast<__int8>(x));
        }
        return board;
    }

    vector<__int8> default_goal_state() {
        vector<__int8> board;
        for (int i = 1; i < size * size; i++) {
            board.push_back(i);
        }
        board.push_back(0);
        return board;
    }

    bool from_vector(vector<int> &board) {
        vector<__int8> start_state = read_vector(board), goal_state = default_goal_state();
        if (!verify_input(start_state, goal_state, nullptr, false)) {
            error_board_printer(start_state, goal_state, nullptr, false);
            return false;
        }
        if (!is_solvable(start_state, goal_state, nullptr, false)) {
            error_board_printer(start_state, goal_state, nullptr, false);
            return false;
        }
        manhattan_helper(goal_state);
        start_node = new Node(start_state, '\0', mahattan_distance(start_state));
        goal_node = new Node(goal_state, '\0', 0);
        return true;
    }

    bool from_file(fs::path file_path, fs::path output_file_path) {
        vector<__int8> start_state = read_file(file_path), goal_state = default_goal_state();
        ofstream file(output_file_path);
        if (!verify_input(start_state, goal_state, &file)) {
            error_board_printer(start_state, goal_state, &file);
            return false;
        }
        if (!is_solvable(start_state, goal_state, &file)) {
            error_board_printer(start_state, goal_state, &file);
            return false;
        }
        manhattan_helper(goal_state);
        start_node = new Node(start_state, '\0', mahattan_distance(start_state));
        goal_node = new Node(goal_state, '\0', 0);
        file.close();
        return true;
    }

    void error_board_printer(vector<__int8> &start_state, vector<__int8> &goal_state, ofstream *file, bool write_file = true) {
        cout << "start state:\n";
        int width = start_state.size() / size;
        for (int i = 0; i < start_state.size(); i++) {
            cout << setw(2) << static_cast<int>(start_state[i]) << " ";
            if ((i + 1) % width == 0) cout << endl;
        }
        cout << "--------------------\n";
        cout << endl;
        cout << "goal state:\n";
        for (int i = 0; i < goal_state.size(); i++) {
            cout << setw(2) << static_cast<int>(goal_state[i]) << " ";
            if ((i + 1) % width == 0) cout << endl;
        }
        cout << "--------------------\n";
        cout << endl;
        if (write_file) {
            (*file) << "start state:\n";
            for (int i = 0; i < start_state.size(); i++) {
                (*file) << static_cast<int>(start_state[i]) << " ";
                if ((i + 1) % width == 0) (*file) << endl;
            }
            (*file) << "--------------------\n";
            (*file) << endl;
            (*file) << "goal state:\n";
            for (int i = 0; i < goal_state.size(); i++) {
                (*file) << static_cast<int>(goal_state[i]) << " ";
                if ((i + 1) % width == 0) (*file) << endl;
            }
            (*file) << "--------------------\n";
            (*file) << endl;
        }
    }

    vector<__int8> get_input() {
        string s;
        getline(cin, s);
        vector<__int8> input;
        stringstream ss;
        ss.clear();
        ss.str(s);
        int x;
        while (ss >> x) {
            input.push_back(x);
        }
        return input;
    }

    bool verify_input(vector<__int8> &start_state, vector<__int8> &goal_state, ofstream *file, bool write_file = true) {
        if (start_state.size() != goal_state.size()) {
            cout << "Error: Invalid input" << endl;
            cout << "The size of start state and goal state must be the same" << endl;
            if (write_file) {
                (*file) << "Error: Invalid input" << endl;
                (*file) << "The size of start state and goal state must be the same" << endl;
            }
            return false;
        }
        vector<bool> check(size * size, false);
        for (int i = 0; i < start_state.size(); i++) {
            if (start_state[i] < 0 || start_state[i] >= size * size) {
                cout << "Error: Invalid input" << endl;
                cout << "The start state must contain numbers from 0 to " << size * size - 1 << endl;
                cout << "The number " << start_state[i] << " is invalid" << endl;
                if (write_file) {
                    (*file) << "Error: Invalid input" << endl;
                    (*file) << "The start state must contain numbers from 0 to " << size * size - 1 << endl;
                    (*file) << "The number " << start_state[i] << " is invalid" << endl;
                }
                return false;
            }
            if (check[start_state[i]]) {
                cout << "Error: Invalid input" << endl;
                cout << "The start state must contain each number exactly once" << endl;
                if (write_file) {
                    (*file) << "Error: Invalid input" << endl;
                    (*file) << "The start state must contain each number exactly once" << endl;
                }
                return false;
            }
            check[start_state[i]] = true;
        }
        check.clear();
        check.resize(size * size, false);
        for (int i = 0; i < goal_state.size(); i++) {
            if (goal_state[i] < 0 || goal_state[i] >= size * size) {
                cout << "Error: Invalid input" << endl;
                cout << "The goal state must contain numbers from 0 to " << size * size - 1 << endl;
                if (write_file) {
                    (*file) << "Error: Invalid input" << endl;
                    (*file) << "The goal state must contain numbers from 0 to " << size * size - 1 << endl;
                }
                return false;
            }
            if (check[goal_state[i]]) {
                cout << "Error: Invalid input" << endl;
                cout << "The goal state must contain each number exactly once" << endl;
                if (write_file) {
                    (*file) << "Error: Invalid input" << endl;
                    (*file) << "The goal state must contain each number exactly once" << endl;
                }
                return false;
            }
            check[goal_state[i]] = true;
        }
        return true;
    }

    bool is_solvable(vector<__int8> &start_state, vector<__int8> &goal_state, ofstream *file, bool write_file = true) {
        if (parity(start_state) != parity(goal_state)) {
            cout << "Error: Not solvable" << endl;
            if (write_file) {
                (*file) << "Error: Not solvable" << endl;
            }
            return false;
        }
        return true;
    }

    void manhattan_helper(vector<__int8> goal_state) {
        goal_x.resize(size * size);
        goal_y.resize(size * size);
        for (int i = 0; i < goal_state.size(); i++) {
            int x = i / size;
            int y = i % size;
            goal_x[goal_state[i]] = x;
            goal_y[goal_state[i]] = y;
        }
    }

    int mahattan_distance(vector<__int8> &state) {
        int distance = 0;
        for (int i = 0; i < state.size(); i++) {
            if (state[i] == 0) continue;
            int x = i / size;
            int y = i % size;
            distance += abs(x - goal_x[state[i]]) + abs(y - goal_y[state[i]]);
        }
        return distance;
    }

    void repr(fs::path *output_file_path, bool write_file = true) {
        ofstream *file;
        if (write_file) {
            file = new ofstream(*output_file_path);
        }
        cout << "Start State: " << endl;
        if (write_file) {
            (*file) << "Start State: " << endl;
        }
        start_node->repr(0, true, write_file, file);
        cout << endl;
        if (write_file) {
            (*file) << endl;
        }
        cout << "Goal State: " << endl;
        if (write_file) {
            (*file) << "Goal State: " << endl;
        }
        goal_node->repr(0, true, write_file, file);
        if (write_file) {
            file->close();
        }
    }

    Node expand(Node &node, char move) {
        static int dx[] = {0, 0, 1, -1};
        static int dy[] = {1, -1, 0, 0};
        static char move_list[] = {'R', 'L', 'D', 'U'};
        static unordered_map<char, int> move_map = {{'R', 0}, {'L', 1}, {'D', 2}, {'U', 3}};
        int x = node.zero_index / size;
        int y = node.zero_index % size;
        int next_x = x + dx[move_map[move]];
        int next_y = y + dy[move_map[move]];
        vector<__int8> next_state = node.state;
        swap(next_state[x * size + y], next_state[next_x * size + next_y]);
        return Node(next_state, move, mahattan_distance(next_state));
    }

    int getInvCount(vector<__int8> &arr) {
        int inv_count = 0;
        for (int i = 0; i < size * size - 1; i++) {
            for (int j = i + 1; j < size * size; j++) {
                if (arr[j] && arr[i] && arr[i] > arr[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

    int findXPosition(vector<__int8> &puzzle) {
        for (int i = size - 1; i >= 0; i--)
            for (int j = size - 1; j >= 0; j--)
                if (puzzle[i * size + j] == 0)
                    return size - i;
        return -1;
    }

    bool parity(vector<__int8> &puzzle) {
        int invCount = getInvCount(puzzle);

        if (size & 1)
            return !(invCount & 1);

        else {
            int pos = findXPosition(puzzle);
            if (pos & 1)
                return !(invCount & 1);
            else
                return invCount & 1;
        }
    }

    bool is_solvable() {
        return parity(start_node->state) == parity(goal_node->state);
    }

    int step() {
        int x1 = start_node->zero_index / size;
        int y1 = start_node->zero_index % size;
        int x2 = goal_node->zero_index / size;
        int y2 = goal_node->zero_index % size;
        return (abs(x1 - x2) + abs(y1 - y2)) % 2;
    }

    void print_state(vector<__int8> &state, ofstream &file, int indent = 0) {
        for (int i = 0; i < size; i++) {
            for (int x = 0; x < indent; x++) cout << "    ";
            for (int x = 0; x < indent; x++) file << "    ";
            for (int j = 0; j < size; j++) {
                cout << static_cast<int>(state[i * size + j]) << " ";
                file << static_cast<int>(state[i * size + j]) << " ";
            }
            cout << endl;
            file << endl;
        }
    }

    int size;
    Node *start_node;
    Node *goal_node;

    vector<__int8> goal_x, goal_y;
};

#endif