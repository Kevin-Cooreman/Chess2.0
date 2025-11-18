#include "board.hpp"
#include "moveGeneration.hpp"
#include <iostream>
using namespace std;

int main(void){
    initBoard();

    printBoard();

    setupStartPos();
    
    printBoard();

    for (int sq = 0; sq < 64; ++sq) {
        uint64_t queen = RookMoves[sq] | BishopMoves[sq];
        std::cout << "0x" << std::hex << queen << "ULL";
        if (sq != 63) std::cout << ", ";
        if ((sq + 1) % 8 == 0) std::cout << std::endl;
    }
    
    return 0;
}