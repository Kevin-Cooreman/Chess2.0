#include <cctype>
#include <sstream>

// Local helper functions for UCI move parsing/printing
int algebraicToSquare(const std::string& alg) {
    if (alg.length() != 2) return -1;
    int file = alg[0] - 'a';
    int rank = alg[1] - '1';
    return rank * 8 + file;
}
std::string squareToAlgebraic(int sq) {
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return std::string{file, rank};
}
#include "game.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;


int main() {
    Game game;
    string line;
    while (getline(cin, line)) {
        if (line == "uci") {
            cout << "id name Chess2.0" << endl;
            cout << "id author Kevin-Cooreman" << endl;
            cout << "uciok" << endl;
        } else if (line == "isready") {
            cout << "readyok" << endl;
        } else if (line == "ucinewgame") {
            game.reset();
        } else if (line.rfind("position", 0) == 0) {
            // Support 'position startpos moves ...'
            if (line.find("startpos") != string::npos) {
                game.reset();
                size_t movesIdx = line.find("moves");
                if (movesIdx != string::npos) {
                    string movesStr = line.substr(movesIdx + 5); // after 'moves'
                    istringstream ms(movesStr);
                    string move;
                    while (ms >> move) {
                        int from = algebraicToSquare(move.substr(0,2));
                        int to = algebraicToSquare(move.substr(2,2));
                        int promo = 0;
                        if (move.length() == 5) {
                            char p = tolower(move[4]);
                            if (game.whiteToMove()) {
                                if (p == 'q') promo = WHITE_QUEEN;
                                else if (p == 'r') promo = WHITE_ROOK;
                                else if (p == 'b') promo = WHITE_BISHOP;
                                else if (p == 'n') promo = WHITE_KNIGHT;
                            } else {
                                if (p == 'q') promo = BLACK_QUEEN;
                                else if (p == 'r') promo = BLACK_ROOK;
                                else if (p == 'b') promo = BLACK_BISHOP;
                                else if (p == 'n') promo = BLACK_KNIGHT;
                            }
                        }
                        // Generate legal moves for the current position
                        MoveList legalMoves = game.generateLegalMoves();
                        bool found = false;
                        Move chosen;
                        for (int i = 0; i < legalMoves.count; ++i) {
                            const Move& m = legalMoves.moves[i];
                            if (m.startSquare == from && m.targetSquare == to) {
                                if (m.moveType == PAWN_PROMOTION && promo != 0 && m.promotionPiece == promo) {
                                    chosen = m; found = true; break;
                                } else if (m.moveType != PAWN_PROMOTION) {
                                    chosen = m; found = true; break;
                                }
                            }
                        }
                        if (found) {
                            cout << "info string UCI replay: applying move " << move << endl;
                            game.makeMove(chosen);
                        } else {
                            cout << "info string UCI replay: WARNING - move not found in legal move list: " << move << endl;
                        }
                    }
                }
            }
        } else if (line.rfind("go", 0) == 0) {
            // Generate and play a move
            MoveList legalMoves = game.generateLegalMoves();
            if (legalMoves.count > 0) {
                const Move& m = legalMoves.moves[0];
                string moveStr = squareToAlgebraic(m.startSquare) + squareToAlgebraic(m.targetSquare);
                if (m.moveType == PAWN_PROMOTION) {
                    moveStr += 'q';
                }
                cout << "bestmove " << moveStr << endl;
                game.makeMove(m);
            } else {
                cout << "bestmove 0000" << endl;
            }
        } else if (line == "quit") {
            break;
        }
    }
    return 0;
}