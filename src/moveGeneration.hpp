#pragma once
// source for move tables: https://github.com/nkarve/surge/blob/master/src/tables.cpp
/*generate legal moves for each piece
- possible moves
- check if blocked
- check if it can take
- complex moves like castle and en passant*/

#include "board.hpp"
// Forward declaration to break circular dependency
struct GameState;
#include <cstdint>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <stack>

using namespace std;

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

//pawn attacks
extern const uint64_t WPawnAttacks[64];
extern const uint64_t BPawnAttacks[64];

// En passant target square (-1 means no en passant available)
extern int enPassantTargetSquare;

enum Color { WHITE, BLACK };

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
    Move() : startSquare(0), targetSquare(0), moveType(NORMAL), promotionPiece(0) {}
    Move(int s, int t, MoveType type = NORMAL, int promo = 0)
        : startSquare(s), targetSquare(t), moveType(type), promotionPiece(promo) {}
};

constexpr int MAX_MOVES = 256;

struct MoveList {
    Move moves[MAX_MOVES];
    int count;

    MoveList() : count(0) {}

    void add(const Move& move) {
        if (count < MAX_MOVES) moves[count++] = move;
    }
};

// Pin and check info for fast legal move generation
struct PinCheckInfo {
    bool inCheck = false;
    std::vector<int> pinnedSquares; // indices of pinned pieces
    std::vector<int> checkRays;     // squares delivering check (could be improved to bitboards)
};

PinCheckInfo detectPinsAndChecks(Color color);
//functions
bool inCheck(Color color);
bool squareAttacked(int square, Color byColor);

//generate special moves
void generatePseudoLegalMoves(MoveList& moves, Color color);
void generateCastlingMoves(MoveList& moves, GameState state, Color color);
void generatePawnPromotionMoves(MoveList& moves, Color color);
void generateEnPassantMoves(MoveList& moves, Color color);

inline bool isOpponentPiece(int piece, Color color) {
    return (color == WHITE && isBlack(piece)) || (color == BLACK && isWhite(piece));
}

// Add correct legal move generator declaration
MoveList generateLegalMoves(const GameState& state); //return all legalmoves