//
// Created by gh0stadian on 11/5/20.
//
#include <iostream>
#include <vector>
#include "bignum.h"
#include <assert.h>

int main2(){
    try{
        BigNum x = BigNum("a54129861475217891278912516759217915758646546546521865132486219864621896462186847656846915");
        BigNum y = BigNum("684684125348646852418654185435434512374456456453452378373572353243737831237548537868615498");
        BigNum z = x * y;
        z *= z;
        std::cout << z;

    }
    catch (const char * e){
        std::cout << e << std::endl;
    }
    return 0;
}