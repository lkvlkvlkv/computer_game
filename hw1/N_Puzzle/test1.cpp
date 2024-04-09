#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "generator.hpp"
#include "puzzle.hpp"
#include "search.hpp"
using namespace std;
namespace fs = std::filesystem;

int main() {
    int size;
    cout << "Please input size: ";
    cin >> size;
    Generator *generator = new Generator(size, true);
    generator->repr();

    vector<int> board = generator->get_random_board(false);
    Puzzle *puzzle = new Puzzle(size);
    if (!puzzle->from_vector(board)) {
        system("pause");
        return 0;
    }
    puzzle->repr(nullptr, false);
    Search *search = new Search(puzzle);
    search->reset();
    search->iterative_deepening_search_v2();
    search->show_result(nullptr, true, true, false);
    system("pause");
    return 0;
}