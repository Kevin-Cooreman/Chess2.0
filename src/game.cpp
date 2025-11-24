#include "game.hpp"

Game::Game() {
    // Initialize game state variables
    for (int i = 0; i < 64; ++i) state.board[i] = EMPTY;
    state.whiteKingMoved = false;
    state.blackKingMoved = false;
    state.whiteKingsideRookMoved = false;
    state.whiteQueensideRookMoved = false;
    state.blackKingsideRookMoved = false;
    state.blackQueensideRookMoved = false;
    state.isWhiteTurn = true;
    state.enPassantTargetSquare = -1;
    halfmoveClock = 0;
    fullmoveNumber = 1;
    moveHistory.clear();
    while (!undoStack.empty()) undoStack.pop();
}

void Game::setupStartPos(){
    initBoard(); // Clear the board first
    state.isWhiteTurn = true; // White to move at start

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

void Game::makeMove(const Move& move) {
    saveGameState();
    int movingPiece = board[move.startSquare];
    int capturedPiece = board[move.targetSquare];

    // Handle special moves
    switch (move.moveType) {
        case CASTLING_KINGSIDE:
            board[move.targetSquare] = movingPiece;
            board[move.startSquare] = EMPTY;
            if (isWhite(movingPiece)) {
                board[move.targetSquare - 1] = WHITE_ROOK;
                board[move.targetSquare + 1] = EMPTY;
            } else {
                board[move.targetSquare - 1] = BLACK_ROOK;
                board[move.targetSquare + 1] = EMPTY;
            }
            break;
        case CASTLING_QUEENSIDE:
            board[move.targetSquare] = movingPiece;
            board[move.startSquare] = EMPTY;
            if (isWhite(movingPiece)) {
                board[move.targetSquare + 1] = WHITE_ROOK;
                board[move.targetSquare - 2] = EMPTY;
            } else {
                board[move.targetSquare + 1] = BLACK_ROOK;
                board[move.targetSquare - 2] = EMPTY;
            }
            break;
        case EN_PASSANT: {
            board[move.targetSquare] = movingPiece;
            board[move.startSquare] = EMPTY;
            int capSq = move.targetSquare + (isWhite(movingPiece) ? -8 : 8);
            board[capSq] = EMPTY;
            break;
        }
        case PAWN_PROMOTION:
            board[move.targetSquare] = move.promotionPiece;
            board[move.startSquare] = EMPTY;
            break;
        default:
            board[move.targetSquare] = movingPiece;
            board[move.startSquare] = EMPTY;
            break;
    }
    updateGameState(move);
}

void Game::updateGameState(const Move& move) {
    // Reset en passant target
        enPassantTargetSquare = -1;

        // Track king and rook movement for castling rights
        int movingPiece = board[move.targetSquare];
        if (movingPiece == WHITE_KING) state.whiteKingMoved = true;
        if (movingPiece == BLACK_KING) state.blackKingMoved = true;
        if (move.startSquare == 0) state.whiteQueensideRookMoved = true;
        if (move.startSquare == 7) state.whiteKingsideRookMoved = true;
        if (move.startSquare == 56) state.blackQueensideRookMoved = true;
        if (move.startSquare == 63) state.blackKingsideRookMoved = true;

        // Pawn double move (sets en passant target)
        if (movingPiece == WHITE_PAWN && move.targetSquare - move.startSquare == 16) {
            enPassantTargetSquare = move.startSquare + 8;
        }
        if (movingPiece == BLACK_PAWN && move.startSquare - move.targetSquare == 16) {
            enPassantTargetSquare = move.targetSquare + 8;
        }

    // Switch turn
    state.isWhiteTurn = !state.isWhiteTurn;
}

void Game::saveGameState() {
    GameState state;
    for (int i = 0; i < 64; ++i) state.board[i] = board[i];
    state.whiteKingMoved = state.whiteKingMoved;
    state.blackKingMoved = state.blackKingMoved;
    state.whiteKingsideRookMoved = state.whiteKingsideRookMoved;
    state.whiteQueensideRookMoved = state.whiteQueensideRookMoved;
    state.blackKingsideRookMoved = state.blackKingsideRookMoved;
    state.blackQueensideRookMoved = state.blackQueensideRookMoved;
    state.isWhiteTurn = state.isWhiteTurn;
    state.enPassantTargetSquare = enPassantTargetSquare;
    undoStack.push(state);
}

void Game::undoMove() {
    if (undoStack.empty()) return;
    GameState state = undoStack.top();
    undoStack.pop();
    for (int i = 0; i < 64; ++i) board[i] = state.board[i];
    state.whiteKingMoved = state.whiteKingMoved;
    state.blackKingMoved = state.blackKingMoved;
    state.whiteKingsideRookMoved = state.whiteKingsideRookMoved;
    state.whiteQueensideRookMoved = state.whiteQueensideRookMoved;
    state.blackKingsideRookMoved = state.blackKingsideRookMoved;
    state.blackQueensideRookMoved = state.blackQueensideRookMoved;
    state.isWhiteTurn = state.isWhiteTurn;
    enPassantTargetSquare = state.enPassantTargetSquare;
}

bool Game::isDraw() const {
    // 50-move rule
    if (halfmoveClock >= 100) return true;

    // Insufficient material (basic version)
    int whiteMaterial = 0, blackMaterial = 0, whiteBishop = 0, blackBishop = 0, whiteKnight = 0, blackKnight = 0;
    for (int i = 0; i < 64; ++i) {
        switch (state.board[i]) {
            case WHITE_PAWN: case WHITE_ROOK: case WHITE_QUEEN: whiteMaterial++; break;
            case BLACK_PAWN: case BLACK_ROOK: case BLACK_QUEEN: blackMaterial++; break;
            case WHITE_BISHOP: whiteBishop++; break;
            case BLACK_BISHOP: blackBishop++; break;
            case WHITE_KNIGHT: whiteKnight++; break;
            case BLACK_KNIGHT: blackKnight++; break;
        }
    }
    // Only kings
    if (whiteMaterial == 0 && blackMaterial == 0 && whiteBishop == 0 && blackBishop == 0 && whiteKnight == 0 && blackKnight == 0) return true;
    // King + bishop/knight vs king
    if ((whiteMaterial == 0 && blackMaterial == 0) && ((whiteBishop == 1 && blackBishop == 0 && blackKnight == 0) || (blackBishop == 1 && whiteBishop == 0 && whiteKnight == 0) || (whiteKnight == 1 && blackBishop == 0 && blackKnight == 0) || (blackKnight == 1 && whiteBishop == 0 && whiteKnight == 0))) return true;

    // Stalemate (no legal moves and not in check)
    MoveList moves = generateLegalMoves(state);
    if (moves.count == 0 && !inCheck(state.isWhiteTurn ? WHITE : BLACK)) return true;

    // Repetition not implemented yet
    return false;
}
    
bool Game::isCheckmate(Color color) const {
    // Generate legal moves for the current state and color
    MoveList moves = generateLegalMoves(state);
    // If no legal moves and in check, it's checkmate
    if (moves.count == 0 && inCheck(color)) return true;
    return false;
}

const int* Game::getBoard() const {
    return state.board;
}

int Game::getHalfmoveClock(){return halfmoveClock;}

int Game::getFullmoveNumber(){return fullmoveNumber;};
    
string Game::getFEN() const {
    // Piece placement
    string fen;
    for (int row = 7; row >= 0; --row) {
        int empty = 0;
        for (int col = 0; col < 8; ++col) {
            int sq = row * 8 + col;
            int piece = state.board[sq];
            if (piece == EMPTY) {
                ++empty;
            } else {
                if (empty > 0) { fen += to_string(empty); empty = 0; }
                fen += pieceToChar(piece);
            }
        }
        if (empty > 0) fen += to_string(empty);
        if (row > 0) fen += '/';
    }

    // Active color
    fen += ' ';
    fen += (state.isWhiteTurn ? 'w' : 'b');

    // Castling rights
    fen += ' ';
    std::string castling;
    if (!state.whiteKingMoved && !state.whiteKingsideRookMoved) castling += 'K';
    if (!state.whiteKingMoved && !state.whiteQueensideRookMoved) castling += 'Q';
    if (!state.blackKingMoved && !state.blackKingsideRookMoved) castling += 'k';
    if (!state.blackKingMoved && !state.blackQueensideRookMoved) castling += 'q';
    fen += (castling.empty() ? "-" : castling);

    // En passant
    fen += ' ';
    if (state.enPassantTargetSquare >= 0 && state.enPassantTargetSquare < 64) {
        char file = 'a' + (state.enPassantTargetSquare % 8);
        char rank = '1' + (state.enPassantTargetSquare / 8);
        fen += file;
        fen += rank;
    } else {
        fen += '-';
    }

        // Halfmove clock and fullmove number
    fen += ' ' + std::to_string(halfmoveClock);
    fen += ' ' + std::to_string(fullmoveNumber);
    return fen;
}

vector<string> Game::getMoveHistory() const {
    return moveHistory;
}

GameState Game::getState(){
    return state;
}
