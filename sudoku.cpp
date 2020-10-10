#include <iostream>
#include <fstream>
#include <search.h>
#include <string>
#include <cstring>

std::istream *input(&std::cin);
std::ostream *output(&std::cout);

int is_possible(unsigned position, unsigned int x, unsigned int y, char num_to_place, std::string line){
    for (unsigned int i = 0; i < 9; i++){
        if (unsigned (9 * y) + i != position && num_to_place == line[unsigned (9 * y) + i])    // CHECK X-AXIS
            return 0;
        if (unsigned (9 * i) + x != position && num_to_place == line[unsigned (9 * i) + x])    // CHECK Y-AXIS
            return 0;
    }
    unsigned int box_start_x = x-x%3, box_start_y = y-y%3;
    for (unsigned int j = 0; j < 3; j++){
        if (unsigned (9 * box_start_y) + j + box_start_x != position
                && num_to_place == line[unsigned (9 * box_start_y) + j + box_start_x])
            return 0;
        if (unsigned (9 * (box_start_y + 1)) + j + box_start_x != position
                && num_to_place == line[unsigned (9 * (box_start_y + 1)) + j + box_start_x])
            return 0;
        if (unsigned (9 * (box_start_y + 2)) + j + box_start_x != position
                && num_to_place == line[unsigned (9 * (box_start_y + 2)) + j + box_start_x])
            return 0;
    }
    return 1;
}

int is_correct(std::string line){
    for (unsigned int i = 0; i < 81; ++i) {
        if (!is_possible(i, i%9, i/9, line[i], line)){
            return 0;
        }
    }
    return 1;
}

void solve_sudoku(std::string line, std::string *solved_sudoku){
    for (unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == '0' || line[i] == '.'){
            for (int j = 1; j < 10; j++) {
                if (is_possible(i,i%9,i/9,char (j+'0'), line)){
                    line[i] = char(j) + '0';
                    solve_sudoku(line, solved_sudoku);
                    line[i] = '0';
                }
            }
            return;
        }
    }
    *solved_sudoku = line;
}

int main(int argc, char * argv[]) {
    int line_counter = 0;
    std::ifstream input_file;
    std::ofstream output_file;
    std::string empty_line = "", line;

    if (argc % 2){
        if (argc>1){
            for (int i = 1; i < argc; i += 2) {
                if (std::strcmp(argv[i], "-i") == 0) {
                    input_file.open(argv[i+1]);
                    if(!input_file.is_open()){
                        std::cout << "cannot read file\n";
                        return 1;
                    }
                    input = &input_file;

                } else if (std::strcmp(argv[i], "-o") == 0){
                    output_file.open(argv[i+1]);
                    if(!output_file.is_open()){
                        std::cout << "cannot write file\n";
                        return 1;
                    }
                    output = &output_file;
                }
            }
        }
    } else{
        std::cout << "wrong number of parameters\n";
        return 1;
    }
    while (std::getline(*input, line)){
        line_counter++;
        if (line == empty_line){
            std::cout << "Invalid input (" << line_counter << ")" << std::endl;
            return 1;
        }
        if (line.length() != 81){
            std::cout << "line length != 81 characters (" << line_counter << ")" << std::endl;
            return 1;
        }
        std::string solved_sudoku = line;
        solve_sudoku(line, &solved_sudoku);
        if (is_correct(solved_sudoku)) {
            *output << solved_sudoku << std::endl;
        } else {
            *output << std::endl;
        }
    }
    if (input_file.is_open() && !input_file.eof()){
        std::cout << "error reading file\n";
        return 1;
    }
    return 0;
}
