#include "board.hpp"
#include "moveGeneration.hpp"

int board[64];

// initialise empty board
void initBoard(){
    for(int i = 0; i < 64; i++) {
        board[i] = EMPTY;    
    }
}

//converts a piece to it's character representation
char pieceToChar(int piece){
    switch(piece){
        //WHITE
        case 0b0001: return 'P';
        case 0b0010: return 'N';
        case 0b0011: return 'B';
        case 0b0100: return 'R';
        case 0b0101: return 'Q';
        case 0b0110: return 'K';
        //BLACK
        case 0b1001: return 'p';
        case 0b1010: return 'n';
        case 0b1011: return 'b';
        case 0b1100: return 'r';
        case 0b1101: return 'q';
        case 0b1110: return 'k';
        //DEFAULT
        default: return ' ';
    }
}

int charToPiece(char piece){
        switch(piece){
        //WHITE
        case 'P': return WHITE_PAWN;
        case 'R': return WHITE_ROOK;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;
        //BLACK
        case 'p': return BLACK_PAWN;
        case 'r': return BLACK_ROOK;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;
        //DEFAULT
        default: return EMPTY;
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