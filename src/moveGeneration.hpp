#pragma once
/*generate legal moves for each piece
- possible moves
- check if blocked
- check if it can take
- complex moves like castle and en passant*/

#include "board.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include <array>

using namespace std;



/*To Do:    Normal moves:
            Rook Moves
            Bishop Moves
            Queen Moves
            Knight Moves
            King Moves
            Pawn Moves

            Legal moves:
            if in Check
            if pinned => if after move in check?
            castling
            en passant
*/

//precomputed tables for all psuedo-legal moves 
//sliding pieces
extern const uint64_t RookMoves[64];
extern const uint64_t BishopMoves[64];
extern const uint64_t QueenMoves[64];

//non-sliding pieces
extern const uint64_t KnightMoves[64];
extern const uint64_t KingMoves[64];

//pawns
extern const uint64_t WPawnMoves[64];
extern const uint64_t BPawnMoves[64];

//helpers for gamestate tracking
extern bool whiteKingMoved;
extern bool blackKingMoved;
extern bool whiteKingsideRookMoved;
extern bool whiteQueensideRookMoved;
extern bool blackKingsideRookMoved;
extern bool blackQueensideRookMoved;

// En passant target square (-1 means no en passant available)
extern int enPassantTargetSquare;

// move types
enum MoveType {
    NORMAL,
    CASTLING_KINGSIDE,
    CASTLING_QUEENSIDE,
    EN_PASSANT,
    PAWN_PROMOTION
};

//move struct
struct Move {
    int startSquare;   // 0-63
    int targetSquare;  // 0-63
    MoveType moveType;
    int promotionPiece;
    Move(int s, int t, MoveType type = NORMAL, int promo = 0)
        : startSquare(s), targetSquare(t), moveType(type), promotionPiece(promo) {}
};

//functions
bool inCheck();
bool squareAttacked();
vector<Move> getCastlingMoves(); // should return a fixed sized array??
vector<Move> getLegalmoves(); //return all legalmoves