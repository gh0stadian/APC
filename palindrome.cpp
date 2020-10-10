#include <iostream>
#include <fstream>

int get_palindrome_length(std::string string){
    int position = 0;
    int palindrome_length = 0;
    for (int i = int(string.length()) - 1; i >= 0; i--) {
        if (string[unsigned (position)] == string[unsigned(i)]){
            palindrome_length++;
            position++;
        }
    }
    return palindrome_length;
}

int main12(int argc, char * argv[]) {
    std::string line;
    if (argc != 3){
        std::cout << "invalid no of arguments";
        return 1;
    }
    std::ofstream output;
    output.open(argv[2]);

    if(!output.is_open()){
        std::cout << "cannot write file\n";
        return 1;
    }
    int line_counter = 0;
    std::string empty_line;
    empty_line = "";
    std::ifstream input(argv[1]);
    while (std::getline(input, line)){
        line_counter++;
        if (line == empty_line){
            std::cout << "Invalid input (" << line_counter << ")" << std::endl;
            return 0;
        }
        int palindrome_length = get_palindrome_length(line);
        if (unsigned (palindrome_length) == line.length()){
            if (!(output << "YES" << std::endl)){
                std::cout << "cannot write file\n";
                return 1;
            }
        } else{
            if (!(output << palindrome_length << std::endl)){
                std::cout << "cannot write file\n";
                return 1;
            }
        }
    }
    if (!input.eof()){
        std::cout << "error reading file\n";
        return 1;
    }
    return 0;
}
