#include <filesystem>
#include <iostream>
#include <string>

#include "generator.hpp"
#include "puzzle.hpp"
#include "search.hpp"
using namespace std;
namespace fs = std::filesystem;

int main() {
    int size;
    cout << "Please input size: ";
    cin >> size;
    Generator *generator = new Generator(size);
    generator->repr();
    int turns;
    cout << "Please input turns (1000): ";
    cin >> turns;
    generator->generate_multiple_random_boards(turns);

    fs::path input_dir_path = generator->get_directory_path();
    cout << "Input directory: " << input_dir_path << endl;
    string temp = input_dir_path.string();
    int index = temp.find("input");
    string suffix = temp.substr(index + 6);
    fs::path output_dir_path = fs::path("./output") / suffix;
    fs::create_directory("./output");
    fs::create_directory(output_dir_path);
    cout << "Output directory: " << output_dir_path << endl;

    vector<int64_t> times;
    Puzzle *puzzle = new Puzzle(size);
    for (int i = 0; i < turns; i++) {
        cout << "\nRandom board " << i << ":" << endl;
        fs::path input_file_path = input_dir_path / ("random_board_" + to_string(i) + ".in");
        fs::path output_file_path = output_dir_path / ("random_board_" + to_string(i) + ".out");
        cout << "Output file: " << output_file_path << endl;
        if (!puzzle->from_file(input_file_path, output_file_path)) {
            continue;
        }
        puzzle->repr(&output_file_path);
        Search *search = new Search(puzzle);
        search->reset();
        search->iterative_deepening_search_v2();
        search->show_result(&output_file_path, true, false);
        times.push_back(search->execution_time);
    }
    int64_t total = 0;
    for (int i = 0; i < times.size(); i++) {
        total += times[i];
    }
    cout << "Total time: " << total << "ms" << endl;
    cout << "Average time: " << accumulate(times.begin(), times.end(), 0.0) / times.size() << "ms" << endl;
    cout << "Solve:" << times.size() << " puzzles" << endl;
    cout << "UnSolvable:" << turns - times.size() << " puzzles" << endl;
    cout << "------------------------------" << endl;
    system("pause");
    return 0;
}