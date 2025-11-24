#include <string>
#include "../src/moveGeneration.hpp"
#include "../src/board.hpp"
#include "../src/game.hpp"
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
    Game game;
    game.setupStartPos();
    MoveList moves = generateLegalMoves(game.getState());
    assert(moves.count == 20);
    std::cout << "Starting position: PASS\n";
}

void test_simple_checkmate() {
    Game game;
    GameState state = game.getState();
    for (int i = 0; i < 64; ++i) state.board[i] = EMPTY;
    state.whiteKingMoved = false;
    state.blackKingMoved = false;
    state.whiteKingsideRookMoved = false;
    state.whiteQueensideRookMoved = false;
    state.blackKingsideRookMoved = false;
    state.blackQueensideRookMoved = false;
    state.enPassantTargetSquare = -1;
    state.isWhiteTurn = false;
    state.board[63] = BLACK_KING; // h8
    state.board[54] = WHITE_QUEEN; // g7
    state.board[45] = WHITE_KING; // f6;
    game.setGameState(state); // Use setGameState instead of updateGameState
    MoveList moves = generateLegalMoves(game.getState());
    assert(moves.count == 0);
    std::cout << "Simple checkmate: PASS\n";
}

void test_stalemate() {
    Game game;
    GameState state = game.getState();
    for (int i = 0; i < 64; ++i) state.board[i] = EMPTY;
    state.whiteKingMoved = false;
    state.blackKingMoved = false;
    state.whiteKingsideRookMoved = false;
    state.whiteQueensideRookMoved = false;
    state.blackKingsideRookMoved = false;
    state.blackQueensideRookMoved = false;
    state.enPassantTargetSquare = -1;
    state.isWhiteTurn = false;
    state.board[63] = BLACK_KING; // h8
    state.board[53] = WHITE_QUEEN; // g6
    state.board[45] = WHITE_KING; // f7;
    game.setGameState(state); // Use setGameState instead of updateGameState
    MoveList moves = generateLegalMoves(game.getState());
    assert(moves.count == 0);
    std::cout << "Stalemate: PASS\n";
}

void test_pawn_promotion() {
    Game game;
    GameState state = game.getState();
    for (int i = 0; i < 64; ++i) state.board[i] = EMPTY;
    state.whiteKingMoved = false;
    state.blackKingMoved = false;
    state.whiteKingsideRookMoved = false;
    state.whiteQueensideRookMoved = false;
    state.blackKingsideRookMoved = false;
    state.blackQueensideRookMoved = false;
    state.enPassantTargetSquare = -1;
    state.isWhiteTurn = true;
    state.board[54] = WHITE_PAWN; // g7
    state.board[7] = WHITE_KING; // h1
    state.board[63] = BLACK_KING; // h8;
    game.setGameState(state); // Use setGameState instead of updateGameState
    MoveList moves = generateLegalMoves(game.getState());
    bool foundPromotion = false;
    for (int i = 0; i < moves.count; ++i) {
        if (moves.moves[i].moveType == PAWN_PROMOTION) foundPromotion = true;
    }
    assert(foundPromotion);
    std::cout << "Pawn promotion: PASS\n";
}

// Refactor test_en_passant to use Game class methods

void test_en_passant() {
    Game game;
    GameState state = game.getState();
    for (int i = 0; i < 64; ++i) state.board[i] = EMPTY;
    state.whiteKingMoved = false;
    state.blackKingMoved = false;
    state.whiteKingsideRookMoved = false;
    state.whiteQueensideRookMoved = false;
    state.blackKingsideRookMoved = false;
    state.blackQueensideRookMoved = false;
    state.enPassantTargetSquare = -1;
    state.isWhiteTurn = false;
    state.board[36] = WHITE_PAWN; // e5
    state.board[51] = BLACK_PAWN; // d7
    state.board[7] = WHITE_KING; // h1
    state.board[63] = BLACK_KING; // h8;
    game.setGameState(state);

    // Black pawn moves d7-d5
    Move m(51, 35, NORMAL);
    game.makeMove(m); // Use Game class method

    // Now white can capture en passant
    state = game.getState();
    state.isWhiteTurn = true;
    game.setGameState(state);

    MoveList moves = generateLegalMoves(game.getState());
    bool foundEnPassant = false;
    for (int i = 0; i < moves.count; ++i) {
        if (moves.moves[i].moveType == EN_PASSANT) foundEnPassant = true;
    }
    printMoves(moves);
    assert(foundEnPassant);
    std::cout << "En passant: PASS\n";

    game.undoMove(); // Use Game class method
}

int main() {
    // Test: sliding piece single-square moves
    auto test_sliding_piece_moves = []() {
        Game game;
        GameState state = game.getState();
        for (int i = 0; i < 64; ++i) state.board[i] = EMPTY;
        // Place white bishop on d4, rook on d5, queen on d6
        state.board[27] = WHITE_BISHOP; // d4
        state.board[35] = WHITE_ROOK;   // d5
        state.board[43] = WHITE_QUEEN;  // d6
        state.board[7] = WHITE_KING;    // h1 (to avoid illegal position)
        state.isWhiteTurn = true;
        game.setGameState(state);

        MoveList moves = generateLegalMoves(game.getState());

        // Debug: Print board state
        std::cout << "Board state:" << std::endl;
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col < 8; ++col) {
                int sq = row * 8 + col;
                std::cout << state.board[sq] << " ";
            }
            std::cout << std::endl;
        }

        // Debug: Print all moves for bishop at d4
        std::cout << "All bishop moves from d4:" << std::endl;
        for (int i = 0; i < moves.count; ++i) {
            if (moves.moves[i].startSquare == 27) {
                std::cout << "  d4 -> " << squareToString(moves.moves[i].targetSquare) << std::endl;
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
