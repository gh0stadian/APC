#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

class Comparator {
public:
    explicit Comparator(unsigned int col) :
    column(col)
    {}
    bool operator ()(std::vector<std::string>&lhs, std::vector<std::string>& rhs) const {
        return lhs[column] < rhs[column];
    }
private:
    unsigned int column;
};

std::vector<std::string> string_to_array(std::string string){
    std::vector<std::string> output;
    std::string number;
    number.push_back(string[0]);
    output.push_back(number);
    unsigned int position = 1;
    while (position <= string.length()){
        std::string column;
        if (string[position] == ','){
            unsigned int end = position+1;
            if (end == string.size()){
                output.push_back(column);
                break;
            }
            while (end < string.size() && string[end] != ','){
                column.push_back(string[end]);
                end++;
            }
            output.push_back(column);
            position = end;
        }
        else{
            position ++;
        }
    }
    return output;
}

void sort(std::vector<std::vector<std::string>> data, std::vector<std::string> position_vector){
    std::sort(data.begin(), data.end(), Comparator(6));
    std::sort(data.begin(), data.end(), Comparator(5));
    std::sort(data.begin(), data.end(), Comparator(1));
    std::sort(data.begin(), data.end(), Comparator(4));
    for (unsigned int i = 0; i < data.size(); ++i){
        for (unsigned int j = 0; j < data[i].size(); ++j) {
            if (j!=0){
                std::cout << ",";
            }
            std::cout << data[i][j];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char * argv[]){
// LOAD ALL ARGUMENTS AND OPEN FILES
    if(argc != 4){
        return 1;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.is_open()){
        std::cout << "cannot read file\n";
        return 1;
    }
    std::ofstream output_file;
    output_file.open(argv[2]);
    if(!output_file.is_open()){
        std::cout << "cannot read file\n";
        return 1;
    }
    std::string positions = argv[3];

// READ FILE
    int commas_count = -1, line_counter = 0;
    std::string line;
    std::vector<std::vector<std::string>> data;
    while(std::getline(input_file, line)){
//        CHECK IF LINE IS CORRECT
        int commas_in_line = 0;
        for (char i : line) {
            if (i == ',')
                commas_in_line++;
        }
        if (commas_count == -1){
            commas_count = commas_in_line;
        }
        if (commas_count != commas_in_line){
            std::cout << "different columns in row" << std::endl;
            return 1;
        }
        else{
            data.push_back(string_to_array(line));
        }
    }
    std::vector<std::string> position_vector = string_to_array(positions);
    sort(data,position_vector);
    for (unsigned int i = 0; i < position_vector.size(); ++i){
        if (i!=0){
            std::cout << ",";
        }
        std::cout << position_vector[i];
    }
}