#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <future>

#include "sha1.h"

struct Flags{
    bool mask = false;
    bool salt = false;
    bool dictionary = false;
    bool fileInput = false;
    bool multithreading = false;
};

std::string target;
auto start = std::chrono::high_resolution_clock::now();
Flags flags;
std::string nums = "0123456789"; // CODE 1
std::string lowerLetters = "abcdefghijklmnopqrstuvwxyz"; // CODE 2
std::string upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // CODE 4
std::string alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // CODE 8

void checkSHA1(const std::string& string){
//    {
//        std::unique_lock<std::mutex> uLock(turnstile.mutex);
//        while (!enterParallelSection()){
//            turnstile.cond.wait(uLock);
//        }
//    }
    if (sha1(string) == target){
        std::cout << "------------PASSWORD FOUND------------\n";
        std::cout << "PASSWORD> " << string << "\n";
        std::cout << "--------------------------------------\n";
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << time.count() * 1e-9;
        exit(0);
    }
//    exitParallelSection();
}

void generate(int i, std::string s) {
    if (i == 0) {
//        futures.push_back(std::async(std::launch::async,[&,s]() { checkSHA1(s); }));
//        std::thread(checkSHA1,s).detach();
        checkSHA1(s);
//        std::async(std::launch::async,checkSHA1,s);
        return;
    }
    for (char j : alphabet) {
        std::string appended = s + j;
        generate(i-1,appended);
    }
}

void crack() {
    if (flags.dictionary){
        std::cout << "dict start\n";
        //TODO dictionarys
        std::cout << "dict end\n";
    }
    for (int i = 1; i <= 4; i++) {
        std::cout << "generating passwords of length " << i << "\n";
        generate(i, "");
    }
}

int main(int argc, char *argv[]) {
    target = argv[1];
    crack();
    return 0;
}