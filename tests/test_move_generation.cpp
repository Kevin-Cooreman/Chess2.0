#include <string>
#include "../src/moveGeneration.hpp"
#include "../src/board.hpp"
#include <cassert>
#include <iostream>

// Helper to convert square index to algebraic notation
std::string squareToString(int sq) {
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return std::string() + file + rank;
}

// Helper to print all moves in a MoveList
void printMoves(const MoveList& moves) {
    for (int i = 0; i < moves.count; ++i) {
        const Move& m = moves.moves[i];
        std::cout << squareToString(m.startSquare) << " -> " << squareToString(m.targetSquare);
        if (m.moveType == PAWN_PROMOTION) std::cout << " (promotion)";
        if (m.moveType == EN_PASSANT) std::cout << " (en passant)";
        if (m.moveType == CASTLING_KINGSIDE) std::cout << " (O-O)";
        if (m.moveType == CASTLING_QUEENSIDE) std::cout << " (O-O-O)";
        std::cout << std::endl;
    }
    std::cout << "Total moves: " << moves.count << std::endl;
}

void test_starting_position() {
    setupStartPos();
    MoveList moves = generateLegalmoves();
    assert(moves.count == 20);
    std::cout << "Starting position: PASS\n";
}

void test_simple_checkmate() {
    // Black king on h8, white queen on g7, white king on f6, black to move (checkmate)
    for (int i = 0; i < 64; ++i) board[i] = EMPTY;
    // Reset all game state flags
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    enPassantTargetSquare = -1;
    isWhiteTurn = false;
    // Set up position
    board[63] = BLACK_KING; // h8
    board[54] = WHITE_QUEEN; // g7
    board[45] = WHITE_KING; // f6;
    MoveList moves = generateLegalmoves();
    assert(moves.count == 0);
    std::cout << "Simple checkmate: PASS\n";
}

void test_stalemate() {
    // Black king on h8, white king on f7, white queen on g6, black to move (stalemate)
    for (int i = 0; i < 64; ++i) board[i] = EMPTY;
    // Reset all game state flags
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    enPassantTargetSquare = -1;
    isWhiteTurn = false;
    // Set up position
    board[63] = BLACK_KING; // h8
    board[53] = WHITE_QUEEN; // g6
    board[45] = WHITE_KING; // f7;
    MoveList moves = generateLegalmoves();
    assert(moves.count == 0);
    std::cout << "Stalemate: PASS\n";
}

void test_pawn_promotion() {
    // White pawn on g7, white king on h1, black king on h8, white to move
    for (int i = 0; i < 64; ++i) board[i] = EMPTY;
    // Reset all game state flags
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    enPassantTargetSquare = -1;
    isWhiteTurn = true;
    // Set up position
    board[54] = WHITE_PAWN; // g7
    board[7] = WHITE_KING; // h1
    board[63] = BLACK_KING; // h8;
    MoveList moves = generateLegalmoves();
    bool foundPromotion = false;
    for (int i = 0; i < moves.count; ++i) {
        if (moves.moves[i].moveType == PAWN_PROMOTION) foundPromotion = true;
    }
    assert(foundPromotion);
    std::cout << "Pawn promotion: PASS\n";
}

void test_en_passant() {
    // White pawn on e5, black pawn on d7, white king on h1, black king on h8, black to move
    for (int i = 0; i < 64; ++i) board[i] = EMPTY;
    // Reset all game state flags
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    enPassantTargetSquare = -1;
    isWhiteTurn = false;
    // Set up position
    board[36] = WHITE_PAWN; // e5
    board[51] = BLACK_PAWN; // d7
    board[7] = WHITE_KING; // h1
    board[63] = BLACK_KING; // h8;
    // Black pawn moves d7-d5
    Move m(51, 35, NORMAL);
    makeMove(m);
    printBoard();
    // Now white can capture en passant
    isWhiteTurn = true;
    MoveList moves = generateLegalmoves();
    bool foundEnPassant = false;
    for (int i = 0; i < moves.count; ++i) {
        if (moves.moves[i].moveType == EN_PASSANT) foundEnPassant = true;
    }
    printMoves(moves);
    assert(foundEnPassant);
    std::cout << "En passant: PASS\n";
    undoMove();
}

int main() {
    // Test: sliding piece single-square moves
    auto test_sliding_piece_moves = []() {
        for (int i = 0; i < 64; ++i) board[i] = EMPTY;
        // Place white bishop on d4, rook on d5, queen on d6
        board[27] = WHITE_BISHOP; // d4
        board[35] = WHITE_ROOK;   // d5
        board[43] = WHITE_QUEEN;  // d6
        board[7] = WHITE_KING;    // h1 (to avoid illegal position)
        isWhiteTurn = true;
        MoveList moves = generateLegalmoves();

        // Debug: Print all bishop, rook, and queen moves
        std::cout << "All bishop moves from d4:" << std::endl;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 27) {
                std::cout << "  d4 -> " << squareToString(moves.moves[i].targetSquare) << std::endl;
            }
        }
        std::cout << "All rook moves from d5:" << std::endl;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 35) {
                std::cout << "  d5 -> " << squareToString(moves.moves[i].targetSquare) << std::endl;
            }
        }
        std::cout << "All queen moves from d6:" << std::endl;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 43) {
                std::cout << "  d6 -> " << squareToString(moves.moves[i].targetSquare) << std::endl;
            }
        }

        // Bishop should be able to move to c3, e3, c5, e5
        bool bishopOk = false;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 27 &&
                (moves.moves[i].targetSquare == 18 || moves.moves[i].targetSquare == 20 ||
                 moves.moves[i].targetSquare == 34 || moves.moves[i].targetSquare == 36)) {
                bishopOk = true; break;
            }
        }
        // Rook should be able to move to d4, d6, c5, e5
        bool rookOk = false;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 35 &&
                (moves.moves[i].targetSquare == 27 || moves.moves[i].targetSquare == 43 ||
                 moves.moves[i].targetSquare == 34 || moves.moves[i].targetSquare == 36)) {
                rookOk = true; break;
            }
        }
        // Queen should be able to move to d5, d7, c6, e6, c7, e7
        bool queenOk = false;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 43 &&
                (moves.moves[i].targetSquare == 35 || moves.moves[i].targetSquare == 51 ||
                 moves.moves[i].targetSquare == 42 || moves.moves[i].targetSquare == 44 ||
                 moves.moves[i].targetSquare == 34 || moves.moves[i].targetSquare == 52)) {
                queenOk = true; break;
            }
        }
        assert(bishopOk && "Bishop single-square and diagonal moves failed");
        assert(rookOk && "Rook single-square and axis moves failed");
        assert(queenOk && "Queen single-square and axis/diagonal moves failed");
        std::cout << "Sliding piece single-square/axis moves: PASS\n";
    };

    test_sliding_piece_moves();
    test_starting_position();
    test_simple_checkmate();
    test_stalemate();
    test_pawn_promotion();
    test_en_passant();
    std::cout << "All move generation tests passed!\n";
    return 0;
}
