#ifndef NODE_H
#define NODE_H

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class Node {
public:
    Node() {
        state = vector<__int8>();
        move = '\0';
        depth = 0;
    }

    ~Node() {
        state.clear();
    }

    Node(vector<__int8> _state, char _move = '\0', int _heuristic = 0) {
        state = _state;
        move = _move;
        depth = 0;
        heuristic = _heuristic;
        size = sqrt(state.size());
        zero_index = 0;
        while (state[zero_index] != 0)
            zero_index++;
    }

    void repr(int indent = 0, bool print = false, bool write_file = false, ofstream *file = nullptr) {
        if (print) {
            // for (int x = 0; x < indent; x++) cout << "    ";
            // cout << "heuristic: " << heuristic << endl;
            for (int i = 0; i < size; i++) {
                for (int x = 0; x < indent; x++) cout << "    ";
                for (int j = 0; j < size; j++) {
                    cout << setw(2) << (int)(state[i * size + j]) << " ";
                }
                cout << endl;
            }
            cout << "--------------------\n";
        }
        if (write_file) {
            // for (int x = 0; x < indent; x++) (*file) << "    ";
            // (*file) << "heuristic: " << heuristic << endl;
            for (int i = 0; i < size; i++) {
                for (int x = 0; x < indent; x++) (*file) << "    ";
                for (int j = 0; j < size; j++) {
                    (*file) << setw(2) << (int)(state[i * size + j]) << " ";
                }
                (*file) << endl;
            }
            (*file) << "--------------------\n";
        }
        return;
    }

    vector<__int8> state;
    char move;
    int heuristic;
    int depth;
    int zero_index;
    int size;
};

#endif