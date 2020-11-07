//
// Created by gh0stadian on 11/5/20.
//
#include <iostream>
#include <vector>
#include "bignum.h"

int main(){
    auto test_num = BigNum(-1);
    auto test_num2 = BigNum(200);
    auto test_num3 = test_num * test_num2;
//        std::cout << test_num3 << std::endl;
    std::cout << test_num3 << std::endl;
    return 0;
}