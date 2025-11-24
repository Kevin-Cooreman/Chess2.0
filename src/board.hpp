// board header file
// Defines the Board class and its member functions
// using a 8x8 board representation 

#pragma once
#include <iostream>
#include <cstdint>
#include <array>
#include <string>
using namespace std;

//FEN string for start position
const string startingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

// 8x8 array for a board
// 0,0 = a8
extern int board[64];

// empty square = 0
const uint8_t EMPTY = 0b0000;

//pieces
const uint8_t WHITE_PAWN = 0b0001; //1
const uint8_t WHITE_KNIGHT = 0b0010; //2
const uint8_t WHITE_BISHOP = 0b0011; //3
const uint8_t WHITE_ROOK = 0b0100; //4
const uint8_t WHITE_QUEEN = 0b0101; //5
const uint8_t WHITE_KING = 0b0110; //6

const uint8_t BLACK_PAWN = 0b1001; //9
const uint8_t BLACK_KNIGHT = 0b1010; //10
const uint8_t BLACK_BISHOP = 0b1011; //11
const uint8_t BLACK_ROOK = 0b1100; //12
const uint8_t BLACK_QUEEN = 0b1101; //13
const uint8_t BLACK_KING = 0b1110; //14

//functions
void initBoard(); //initialise empty board
void printBoard(); //prints board
void setupStartPos(); //setup starting position
void charToPiece(); //converts a charachter to a piece type
void pieceToChar();

//helper functions
inline bool isEmpty(int square){return square == 0;}
inline bool isBlack(int square){return square & 0b1000;} //direct bit check
inline bool isWhite(int square){return !(isBlack(square) || isEmpty(square));}
inline bool sameColour(int square1, int square2){
    return  (isWhite(square1) && isWhite(square2)) ||
            (isBlack(square1) && isBlack(square2));
}