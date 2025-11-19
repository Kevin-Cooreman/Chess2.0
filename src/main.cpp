#include "board.hpp"
#include "moveGeneration.hpp"
#include <iostream>
#include <string>
#include <cctype>
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
    setupStartPos();
    printBoard();
    while (true) {
        Color color = isWhiteTurn ? WHITE : BLACK;
        cout << (isWhiteTurn ? "White" : "Black") << " to move. Enter your move (e.g., e2e4): ";
        string moveInput;
        cin >> moveInput;
        if (moveInput == "exit" || moveInput == "quit") break;
        if (moveInput.length() < 4 || moveInput.length() > 5) {
            cout << "Invalid input format. Use e2e4 or e7e8q for promotion.\n";
            continue;
        }
        int from = algebraicToSquare(moveInput.substr(0,2));
        int to = algebraicToSquare(moveInput.substr(2,2));
        int promo = 0;
        if (moveInput.length() == 5) {
            char p = tolower(moveInput[4]);
            if (isWhiteTurn) {
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
        if (from < 0 || to < 0) {
            cout << "Invalid square.\n";
            continue;
        }
        MoveList legalMoves = generateLegalmoves();
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
        if (!found) {
            cout << "Illegal move. Try again.\n";
            continue;
        }
        makeMove(chosen);
        printBoard();
        // Check for game end
        MoveList nextMoves = generateLegalmoves();
        if (nextMoves.count == 0) {
            if (inCheck(isWhiteTurn ? (isWhiteTurn ? WHITE : BLACK) : (isWhiteTurn ? BLACK : WHITE))) {
                cout << (isWhiteTurn ? "Black" : "White") << " is checkmated!\n";
            } else {
                cout << "Stalemate!\n";
            }
            break;
        }
    }
    cout << "Game over.\n";
    return 0;
}