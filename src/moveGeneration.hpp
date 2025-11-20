#pragma once
// source for move tables: https://github.com/nkarve/surge/blob/master/src/tables.cpp
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
#include <algorithm>
#include <stack>

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

//pawn attacks
extern const uint64_t WPawnAttacks[64];
extern const uint64_t BPawnAttacks[64];

//helpers for gamestate tracking
extern bool whiteKingMoved;
extern bool blackKingMoved;
extern bool whiteKingsideRookMoved;
extern bool whiteQueensideRookMoved;
extern bool blackKingsideRookMoved;
extern bool blackQueensideRookMoved;
extern bool isWhiteTurn;

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

// Structure to store all necessary game state for undo
struct GameState {
    int board[64];
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteKingsideRookMoved;
    bool whiteQueensideRookMoved;
    bool blackKingsideRookMoved;
    bool blackQueensideRookMoved;
    bool isWhiteTurn;
    int enPassantTargetSquare;
};

static std::stack<GameState> undoStack;

//functions
bool inCheck(const int board[64], bool isWhiteTurn, bool whiteKingMoved, bool blackKingMoved, bool whiteKingsideRookMoved, bool whiteQueensideRookMoved, bool blackKingsideRookMoved, bool blackQueensideRookMoved, int enPassantTargetSquare);
bool squareAttacked(const int board[64], int square, Color byColor);
bool isLegalMove(const int board[64], const Move& move, Color color);

//generate special moves
void generatePseudoLegalMoves(const int board[64], MoveList& moves, Color color);
void generateCastlingMoves(const int board[64], MoveList& moves, Color color, bool whiteKingMoved, bool blackKingMoved, bool whiteKingsideRookMoved, bool whiteQueensideRookMoved, bool blackKingsideRookMoved, bool blackQueensideRookMoved);
void generatePawnPromotionMoves(const int board[64], MoveList& moves, Color color);
void generateEnPassantMoves(const int board[64], MoveList& moves, Color color, int enPassantTargetSquare);

inline bool isOpponentPiece(int piece, Color color) {
    return (color == WHITE && isBlack(piece)) || (color == BLACK && isWhite(piece));
}

MoveList generateLegalmoves(const int board[64], bool isWhiteTurn, bool whiteKingMoved, bool blackKingMoved, bool whiteKingsideRookMoved, bool whiteQueensideRookMoved, bool blackKingsideRookMoved, bool blackQueensideRookMoved, int enPassantTargetSquare);

// Remove updateGameState, makeMove, saveGameState, undoMove from here (move to Game class)