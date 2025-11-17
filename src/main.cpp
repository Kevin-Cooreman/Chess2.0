#include "board.hpp"
#include <iostream>
using namespace std;

int main(void){
    initBoard();

    printBoard();

    setupStartPos();
    
    printBoard();
    return 0;
}