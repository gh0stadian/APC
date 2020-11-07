#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

class Comparator {
private:
    unsigned long column;
public:
    explicit Comparator(unsigned long col) :
    column(col)
    {

    }
    bool operator ()(std::vector<std::string>lhs, std::vector<std::string>rhs) const {
        return lhs[column] < rhs[column];
    }
};

std::vector<std::string> convert_string_to_array(std::string string){
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

std::vector<std::vector<std::string>> sort_data
                            (std::vector<std::vector<std::string>> data,std::vector<unsigned long> position_vector){
    for (unsigned long i = position_vector.size()-1; i != ULONG_MAX; --i) {
        std::stable_sort(data.begin(), data.end(), Comparator(position_vector[i] - 1));
    }
    return data;
}

std::vector<unsigned long> convert_position_vector_to_ulong(std::vector<std::string> position_vector){
    std::vector<unsigned long> output;
//    CONVERT STRING TO UNSIGNED LONG
    for (unsigned int i = 0; i < position_vector.size();i++){
        char* p;
        unsigned long converted = std::strtoul(position_vector[i].c_str(), &p, 10);
        if (*p == 0){
            //    CHECK FOR DUPLICITY
            for(unsigned long j = 0; j < i; j++){
                if (output[j] == converted) {
                    std::cout << "wrong parameters" << std::endl;
                    exit(1);
                }
            }
            output.push_back(converted);
        }
        else{
            std::cout << "wrong parameters" << std::endl;
            exit(1);
        }
    }

    return output;
}

std::vector<std::string> convert_data_to_string(std::vector<std::vector<std::string>> data){
    std::vector<std::string> output;
    for (auto & i : data) {
        std::string line;
        for(unsigned long j = 0; j < i.size(); ++j){
            if (j != 0){
                line.push_back(',');
            }
            for(unsigned long k = 0; k < i[j].size(); ++k)
                line.push_back(i[j][k]);
        }
        output.push_back(line);
    }
    return output;
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
    int commas_count = -1;
    std::string line;
    std::vector<std::vector<std::string>> data;
    while(std::getline(input_file, line)){
//        CHECK IF LINE ISN T EMPTY
        if (line.empty()){
            std::cout << "Empty line" << std::endl;
            return 0;
        }

//        COUNT COLUMNS
        int commas_in_line = 0;
        for (char i : line) {
            if (i == ',')
                commas_in_line++;
        }
        if (commas_count == -1){
            commas_count = commas_in_line;
        }
//        CHECK IF ALL ROWS HAVE SAME COLUMNS
        if (commas_count == commas_in_line){
            data.push_back(convert_string_to_array(line));
        }
        else{
            std::cout << "different columns in row" << std::endl;
            return 1;
        }
    }
    if (!input_file.eof()){     //CHECK IF ALL FILE CONTENT WAS READ
        std::cout << "error reading file\n";
        return 1;
    }

//    GET SORT_POSITIONS AND SORT BY THEM
    std::vector<std::string> position_vector = convert_string_to_array(positions);
    std::vector<unsigned long> converted_position_vector = convert_position_vector_to_ulong(position_vector);
    data = sort_data(data, converted_position_vector);

//    CONVERT AND WRITE TO FILE
    std::vector<std::string> output_data = convert_data_to_string(data);
    for (auto & i : output_data){
        if (!(output_file << i << std::endl)){
            std::cout << "cannot write file\n";
            return 1;
        }
    }
    return 0;
}