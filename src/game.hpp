#pragma once
#include "board.hpp"
#include "moveGeneration.hpp"
#include <vector>
#include <stack>
#include <string>

int charToPiece(char c);
char pieceToChar(int piece);

class Game {
public:
	Game();
	void reset();
	void setFromFEN(const std::string& fen);
	std::string getFEN() const;
	void makeMove(const Move& move);
	void undoMove();
    void saveGameState();
    void updateGameState(const Move& move);
	bool isDrawBy50MoveRule() const;
	bool isThreefoldRepetition() const;
	// Add more as needed

	// Move generation
	MoveList generateLegalMoves() const;
	// Accessors
	const int* getBoard() const;
	bool whiteToMove() const;
	int getHalfmoveClock() const;
	int getFullmoveNumber() const;

private:
	int board[64];
	bool whiteKingMoved, blackKingMoved;
	bool whiteKingsideRookMoved, whiteQueensideRookMoved;
	bool blackKingsideRookMoved, blackQueensideRookMoved;
	bool isWhiteTurn;
	int enPassantTargetSquare;
	int halfmoveClock;
	int fullmoveNumber;
	std::stack<GameState> undoStack;
	std::vector<std::string> positionHistory; // for repetition
};