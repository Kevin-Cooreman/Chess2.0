#pragma once
#include "moveGeneration.hpp"
#include "board.hpp"
#include <vector>
#include <stack>
#include <string>

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

class Game {
public:
    Game();
    void setupStartPos();
    void updateGameState(const Move& move);
    void makeMove(const Move& move);
    void saveGameState();
    void undoMove();
    bool isDraw() const; // 50-move rule, repetition, insufficient material, stalemate
    bool isCheckmate(Color color) const;
    const int* getBoard() const;
    inline int getHalfmoveClock();
    int getFullmoveNumber();
    std::string getFEN() const;
    std::vector<std::string> getMoveHistory() const;
    GameState getState();
    // Add a method to directly set the game state
    void setGameState(const GameState& newState) {
        state = newState;
    }

private:
    GameState state; // All game state in one struct
    int halfmoveClock; // for 50-move rule
    int fullmoveNumber;
    std::vector<std::string> moveHistory; // algebraic notation or UCI
    std::stack<GameState> undoStack;
};
