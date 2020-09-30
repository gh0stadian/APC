#include <iostream>

int main() {
    std::string string;
    std::cin >> string;
    int position = 0;
    int palindrome_length = 0;
    for (int i = int(string.length()) - 1; i >= 0; i--) {
        if (i == position && string.length() % 2){
            palindrome_length++;
            break;
        }
        if (string[position] == string[i]){
            palindrome_length++;
            position++;
        }
    }
    if (palindrome_length == string.length()){
        std::cout << "YES" << std::endl;
    } else{
        std::cout << palindrome_length << std::endl;
    }
    return 0;
}
