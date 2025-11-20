#include "board.hpp"
#include "moveGeneration.hpp"

int board[64];

// initialise empty board
void initBoard(){
    for(int i = 0; i < 64; i++) {
        board[i] = EMPTY;    
    }
}

//prints the board
void printBoard(){
    cout << "\n  +---+---+---+---+---+---+---+---+\n";
    
    for(int row = 7; row >= 0; row--) {
        // Print rank number (8, 7, 6, ..., 1)
        cout << (row + 1) << " ";
        for(int col = 0; col < 8; col++) {
            cout << "| " << pieceToChar(board[row*8+col]) << " ";
        }
        cout << "|\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    
    // Print file letters
    cout << "    a   b   c   d   e   f   g   h\n\n";
}

//sets up starting position
void setupStartPos(){
    initBoard(); // Clear the board first
    isWhiteTurn = true; // White to move at start

    int row = 7, col = 0;

    for(char c : startingPosition) {
        if(c == '/') {
            row--;      // Move to next rank (downwards)
            col = 0;    // Reset to file 'a'
        }
        else if(isdigit(c)) {
            col += (c - '0');  // Skip empty squares (number tells us how many)
        }
        else {
            board[row*8+col] = charToPiece(c);  // Place the piece
            col++;
        }
    }
}