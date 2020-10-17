#include <iostream>
#include <fstream>
#include <search.h>
#include <string>
#include <cstring>
#include <vector>

std::string getResultString(char * buffer, std::string * prev_last_X_character, std::string X){
    std::string new_buffer = std::string(&buffer[0], std::strlen(buffer));
    std::string result_string = *prev_last_X_character + new_buffer;
    if(new_buffer.size() > X.length()){
        *prev_last_X_character = new_buffer.substr(new_buffer.size() - X.length()+1);
    }
    return result_string;
}

int main(int argc, char * argv[]) {
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.is_open()){
        std::cout << "cannot read file\n";
        return 1;
    }
    std::string X = argv[2];
    uint32_t N = static_cast<uint32_t>(std::stoi(argv[3]));
    unsigned int length = 10;
    std::string prev_last_X_character = "";
    unsigned long position = 1;
    std::vector<uint32_t> positions = {};
    std::vector<uint32_t> result_positions = {};
    while (!input_file.eof()){
        char * buffer = new char [length];
        input_file.read(buffer, length);
        std::string result_string = getResultString(buffer,&prev_last_X_character,X);
        for (unsigned int i = 0; i < result_string.size() - X.length() + 1; ++i) {
            if (result_string.compare(i,X.size(),X) == 0){
                positions.push_back(position);
            }
            position++;
        }

        delete[ ] buffer;
    }
    for (unsigned long i = 1; i < positions.size(); i+=2) {
        if (positions[i] <= positions[i-1]+N){
            if (result_positions[result_positions.size()-1] != positions[i-1]){
                result_positions.push_back(positions[i-1]);
            }
            result_positions.push_back(positions[i]);
        }
        if (i+1 < positions.size() && positions[i]+N >= positions[i+1]){
            if (result_positions.empty() || result_positions[result_positions.size()-1] != positions[i]){
                result_positions.push_back(positions[i]);
            }
            result_positions.push_back(positions[i+1]);
        }
    }

    for (unsigned int result_position : result_positions) {
        std::cout << result_position << std::endl;
    }

}