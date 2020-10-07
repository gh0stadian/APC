#include <iostream>
#include <fstream>
int is_possible(unsigned int x,unsigned int y,char n, std::string line){
    for (unsigned int i = 0; i < 9; i++){
        if (n == line[unsigned (9*y)+i])    // CHECK X-AXIS
            return 0;
        if (n == line[unsigned (9*i)+x])    // CHECK Y-AXIS
            return 0;
    }
    unsigned int box_start_x = x-x%3, box_start_y = y-y%3;
    for (unsigned int j = 0; j < 3; j++){
        if (n == line[unsigned (9 * box_start_y) + j + box_start_x])
            return 0;
        if (n == line[unsigned (9 * (box_start_y + 1)) + j + box_start_x])
            return 0;
        if (n == line[unsigned (9 * (box_start_y + 2)) + j + box_start_x])
            return 0;
    }
    return 1;
}
std::string solve_sudoku(std::string line){
    for (unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == '0'){
            for (int j = 1; j < 10; j++) {
                if (is_possible(i%9,i/9,char (j+'0'), line)){
                    line[i] = char(j) + '0';
                    solve_sudoku(line);
                    line[i] = '0';
                }
            }
            return line;
        }
    }
    std::cout << line << std::endl;
    return "0";
}

int main(int argc, char * argv[]) {
    int line_counter = 0;
    std::ofstream output;
    std::string empty_line = "", line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)){
        line_counter++;
        if (line == empty_line){
            std::cout << "Invalid input (" << line_counter << ")" << std::endl;
            return 0;
        }
//        std::cout << line;
        std::string result = solve_sudoku(line);
    }
    if (!input.eof()){
        std::cout << "error reading file\n";
        return 1;
    }
    return 0;
}
