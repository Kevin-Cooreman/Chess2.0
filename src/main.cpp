#include "board.hpp"
#include "moveGeneration.hpp"
#include "game.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

// Helper: convert algebraic (e2) to 0-63 square
int algebraicToSquare(const string& s) {
    if (s.length() != 2) return -1;
    char file = tolower(s[0]);
    char rank = s[1];
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return -1;
    int col = file - 'a';
    int row = rank - '1';
    return row * 8 + col;
}

// Helper: convert 0-63 square to algebraic
string squareToAlgebraic(int sq) {
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return string() + file + rank;
}

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
            game.setupStartPos();
        } else if (line.rfind("position", 0) == 0) {
            if (line.find("startpos") != string::npos) {
                game.setupStartPos();
                size_t movesIdx = line.find("moves");
                if (movesIdx != string::npos) {
                    string movesStr = line.substr(movesIdx + 5);
                    istringstream ms(movesStr);
                    string move;
                    while (ms >> move) {
                        int from = algebraicToSquare(move.substr(0,2));
                        int to = algebraicToSquare(move.substr(2,2));
                        int promo = 0;
                        if (move.length() == 5) {
                            char p = tolower(move[4]);
                            if (game.getState().isWhiteTurn == true) {
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

                        MoveList legalMoves = generateLegalMoves(game.getState());
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
            MoveList legalMoves = generateLegalMoves(game.getState());
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