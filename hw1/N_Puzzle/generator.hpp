#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
// c++17
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

class Generator {
public:
    Generator(int size, bool using_random_seed = false) {
        this->size = size;
        if (using_random_seed) {
            std::random_device rd;
            this->seed = rd();
        }
        else {
            // default seed
            this->seed = 1726539273;
        }
        this->rng = std::mt19937(this->seed);
    }

    // using custom seed
    Generator(int size, unsigned int seed) {
        this->size = size;
        this->seed = seed;
        this->rng = std::mt19937(this->seed);
    }

    void repr() {
        cout << "--------------------------" << endl;
        cout << "Generator information:" << endl;
        cout << "Size: " << this->size << endl;
        cout << "Seed: " << this->seed << endl;
        cout << "--------------------------" << endl;
    }

    vector<int> get_random_board(bool print = false) {
        vector<int> board;
        for (int i = 0; i < size * size; i++) {
            board.push_back(i);
        }
        shuffle(board.begin(), board.end(), rng);
        if (print) {
            print_board(board);
            cout << endl;
        }
        return board;
    }

    vector<int> default_goal_state() {
        vector<int> board;
        for (int i = 1; i < size * size; i++) {
            board.push_back(i);
        }
        board.push_back(0);
        return board;
    }

    void generate_multiple_random_boards(int n, bool print = false) {
        fs::path dir_path = "./input/" + to_string(seed) + "_" + to_string(size) + "x" + to_string(size);
        fs::create_directory("./input");
        fs::create_directory(dir_path);
        for (int i = 0; i < n; i++) {
            save_random_board(i, dir_path, print);
        }
    }

    void save_random_board(int number, fs::path dir_path, bool print = false) {
        vector<int> board = get_random_board(print);
        string filename = "random_board_" + to_string(number) + ".in";
        fs::path path = dir_path / filename;
        ofstream file(path);
        file << size << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                file << board[i * size + j] << " ";
            }
            file << endl;
        }
        file.close();
    }

    void print_board(vector<int> board) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << board[i * size + j] << " ";
            }
            cout << endl;
        }
    }

    fs::path get_directory_path() {
        return fs::path("./input/" + to_string(seed) + "_" + to_string(size) + "x" + to_string(size));
    }

private:
    unsigned int seed;
    int size;
    std::mt19937 rng;
};