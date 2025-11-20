#include "game.hpp"
#include <cstring>
#include <sstream>

int charToPiece(char c) {
    switch (c) {
        case 'P': return WHITE_PAWN;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'R': return WHITE_ROOK;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;
        case 'p': return BLACK_PAWN;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'r': return BLACK_ROOK;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;
        default: return 0;
    }
}

char pieceToChar(int piece) {
    switch (piece) {
        case WHITE_PAWN: return 'P';
        case WHITE_KNIGHT: return 'N';
        case WHITE_BISHOP: return 'B';
        case WHITE_ROOK: return 'R';
        case WHITE_QUEEN: return 'Q';
        case WHITE_KING: return 'K';
        case BLACK_PAWN: return 'p';
        case BLACK_KNIGHT: return 'n';
        case BLACK_BISHOP: return 'b';
        case BLACK_ROOK: return 'r';
        case BLACK_QUEEN: return 'q';
        case BLACK_KING: return 'k';
        default: return '1';
    }
}

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


Game::Game() {
    reset();
}

// Make a move on the board and update game state
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
        if (movingPiece == WHITE_KING) whiteKingMoved = true;
        if (movingPiece == BLACK_KING) blackKingMoved = true;
        if (move.startSquare == 0) whiteQueensideRookMoved = true;
        if (move.startSquare == 7) whiteKingsideRookMoved = true;
        if (move.startSquare == 56) blackQueensideRookMoved = true;
        if (move.startSquare == 63) blackKingsideRookMoved = true;

        // Pawn double move (sets en passant target)
        if (movingPiece == WHITE_PAWN && move.targetSquare - move.startSquare == 16) {
            enPassantTargetSquare = move.startSquare + 8;
        }
        if (movingPiece == BLACK_PAWN && move.startSquare - move.targetSquare == 16) {
            enPassantTargetSquare = move.targetSquare + 8;
        }

    // Switch turn
    isWhiteTurn = !isWhiteTurn;
}

void Game::saveGameState() {
    GameState state;
    for (int i = 0; i < 64; ++i) state.board[i] = board[i];
    state.whiteKingMoved = whiteKingMoved;
    state.blackKingMoved = blackKingMoved;
    state.whiteKingsideRookMoved = whiteKingsideRookMoved;
    state.whiteQueensideRookMoved = whiteQueensideRookMoved;
    state.blackKingsideRookMoved = blackKingsideRookMoved;
    state.blackQueensideRookMoved = blackQueensideRookMoved;
    state.isWhiteTurn = isWhiteTurn;
    state.enPassantTargetSquare = enPassantTargetSquare;
    undoStack.push(state);
}

void Game::undoMove() {
    if (undoStack.empty()) return;
    GameState state = undoStack.top();
    undoStack.pop();
    for (int i = 0; i < 64; ++i) board[i] = state.board[i];
    whiteKingMoved = state.whiteKingMoved;
    blackKingMoved = state.blackKingMoved;
    whiteKingsideRookMoved = state.whiteKingsideRookMoved;
    whiteQueensideRookMoved = state.whiteQueensideRookMoved;
    blackKingsideRookMoved = state.blackKingsideRookMoved;
    blackQueensideRookMoved = state.blackQueensideRookMoved;
    isWhiteTurn = state.isWhiteTurn;
    enPassantTargetSquare = state.enPassantTargetSquare;
}

void Game::reset() {
    // Standard chess starting position
    setFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    positionHistory.clear();
    while (!undoStack.empty()) undoStack.pop();
}

void Game::setFromFEN(const std::string& fen) {
    // Minimal FEN parser (does not validate input)
    std::istringstream iss(fen);
    std::string boardPart, turn, castling, ep;
    int halfmove, fullmove;
    iss >> boardPart >> turn >> castling >> ep >> halfmove >> fullmove;
    std::memset(board, 0, sizeof(board));
    int sq = 0;
    for (char c : boardPart) {
        if (c == '/') continue;
        if (isdigit(c)) sq += c - '0';
        else {
            board[sq++] = charToPiece(c);
        }
    }
    isWhiteTurn = (turn == "w");
    whiteKingMoved = (castling.find('K') == std::string::npos);
    whiteQueensideRookMoved = (castling.find('Q') == std::string::npos);
    blackKingMoved = (castling.find('k') == std::string::npos);
    blackQueensideRookMoved = (castling.find('q') == std::string::npos);
    enPassantTargetSquare = (ep == "-") ? -1 : algebraicToSquare(ep);
    halfmoveClock = halfmove;
    fullmoveNumber = fullmove;
}

std::string Game::getFEN() const {
    std::ostringstream oss;
    for (int r = 0; r < 8; ++r) {
        int empty = 0;
        for (int f = 0; f < 8; ++f) {
            int sq = r * 8 + f;
            if (board[sq] == 0) ++empty;
            else {
                if (empty) { oss << empty; empty = 0; }
                oss << pieceToChar(board[sq]);
            }
        }
        if (empty) oss << empty;
        if (r != 7) oss << '/';
    }
    oss << ' ' << (isWhiteTurn ? 'w' : 'b');
    oss << ' ';
    std::string castling;
    if (!whiteKingMoved && !whiteKingsideRookMoved) castling += 'K';
    if (!whiteKingMoved && !whiteQueensideRookMoved) castling += 'Q';
    if (!blackKingMoved && !blackKingsideRookMoved) castling += 'k';
    if (!blackKingMoved && !blackQueensideRookMoved) castling += 'q';
    if (castling.empty()) castling = "-";
    oss << castling;
    oss << ' ';
    if (enPassantTargetSquare == -1) oss << '-';
    else oss << squareToAlgebraic(enPassantTargetSquare);
    oss << ' ' << halfmoveClock << ' ' << fullmoveNumber;
    return oss.str();
}

bool Game::isDrawBy50MoveRule() const {
    return halfmoveClock >= 100;
}

bool Game::isThreefoldRepetition() const {
    int count = 0;
    std::string fen = getFEN();
    for (const auto& pos : positionHistory) {
        if (pos == fen) ++count;
    }
    return count >= 3;
}

MoveList Game::generateLegalMoves() const {
    return generateLegalmoves(board, isWhiteTurn, whiteKingMoved, blackKingMoved, whiteKingsideRookMoved, whiteQueensideRookMoved, blackKingsideRookMoved, blackQueensideRookMoved, enPassantTargetSquare);
}
const int* Game::getBoard() const { return board; }
bool Game::whiteToMove() const { return isWhiteTurn; }
int Game::getHalfmoveClock() const { return halfmoveClock; }
int Game::getFullmoveNumber() const { return fullmoveNumber; }
