#include "moveGeneration.hpp"

//precomputed tables for all psuedo-legal moves 
//sliding pieces
//To do: fill in precomputed tables
const uint64_t RookMoves[64] = {
    0x0080001020400080, 0x0040001000200040, 0x0080081000200080, 0x0080040800100080,
	0x0080020400080080, 0x0080010200040080, 0x0080008001000200, 0x0080002040800100,
	0x0000800020400080, 0x0000400020005000, 0x0000801000200080, 0x0000800800100080,
	0x0000800400080080, 0x0000800200040080, 0x0000800100020080, 0x0000800040800100,
	0x0000208000400080, 0x0000404000201000, 0x0000808010002000, 0x0000808008001000,
	0x0000808004000800, 0x0000808002000400, 0x0000010100020004, 0x0000020000408104,
	0x0000208080004000, 0x0000200040005000, 0x0000100080200080, 0x0000080080100080,
	0x0000040080080080, 0x0000020080040080, 0x0000010080800200, 0x0000800080004100,
	0x0000204000800080, 0x0000200040401000, 0x0000100080802000, 0x0000080080801000,
	0x0000040080800800, 0x0000020080800400, 0x0000020001010004, 0x0000800040800100,
	0x0000204000808000, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
	0x0000040008008080, 0x0000020004008080, 0x0000010002008080, 0x0000004081020004,
	0x0000204000800080, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
	0x0000040008008080, 0x0000020004008080, 0x0000800100020080, 0x0000800041000080,
	0x00FFFCDDFCED714A, 0x007FFCDDFCED714A, 0x003FFFCDFFD88096, 0x0000040810002101,
	0x0001000204080011, 0x0001000204000801, 0x0001000082000401, 0x0001FFFAABFAD1A2
};

const uint64_t BishopMoves[64]= {
    0x0002020202020200, 0x0002020202020000, 0x0004010202000000, 0x0004040080000000,
	0x0001104000000000, 0x0000821040000000, 0x0000410410400000, 0x0000104104104000,
	0x0000040404040400, 0x0000020202020200, 0x0000040102020000, 0x0000040400800000,
	0x0000011040000000, 0x0000008210400000, 0x0000004104104000, 0x0000002082082000,
	0x0004000808080800, 0x0002000404040400, 0x0001000202020200, 0x0000800802004000,
	0x0000800400A00000, 0x0000200100884000, 0x0000400082082000, 0x0000200041041000,
	0x0002080010101000, 0x0001040008080800, 0x0000208004010400, 0x0000404004010200,
	0x0000840000802000, 0x0000404002011000, 0x0000808001041000, 0x0000404000820800,
	0x0001041000202000, 0x0000820800101000, 0x0000104400080800, 0x0000020080080080,
	0x0000404040040100, 0x0000808100020100, 0x0001010100020800, 0x0000808080010400,
	0x0000820820004000, 0x0000410410002000, 0x0000082088001000, 0x0000002011000800,
	0x0000080100400400, 0x0001010101000200, 0x0002020202000400, 0x0001010101000200,
	0x0000410410400000, 0x0000208208200000, 0x0000002084100000, 0x0000000020880000,
	0x0000001002020000, 0x0000040408020000, 0x0004040404040000, 0x0002020202020000,
	0x0000104104104000, 0x0000002082082000, 0x0000000020841000, 0x0000000000208800,
	0x0000000010020200, 0x0000000404080200, 0x0000040404040400, 0x0002020202020200
};

const uint64_t QueenMoves[64]= {
    0x82021222420280, 0x42021202220040, 0x84091202200080, 0x84040880100080,
    0x81124400080080, 0x80831240040080, 0x80418411400200, 0x80106144904100,
    0x840424440480, 0x420222025200, 0x841102220080, 0x840c00900080,
    0x811440080080, 0x808210440080, 0x804104124080, 0x8020c2882100,
    0x4208808480880, 0x2404404241400, 0x1808212022200, 0x80880a005000,
    0x808404a00800, 0xa08102884400, 0x4101820a2004, 0x220041449104,
    0x2288090105000, 0x1240048085800, 0x308084210480, 0x484084110280,
    0x840080882080, 0x424082051080, 0x818081841200, 0xc04080824900, 
    0x1245000a02080, 0xa20840501000, 0x104480882800, 0xa0080881080,
    0x4440c0840900, 0x828180820500, 0x1030101030804, 0x8080c0810500,
    0xa2482080c000, 0x61045000a080, 0x1820a8009080, 0x82011008880,
    0xc0108408480, 0x1030105008280, 0x2030202008480, 0x1014181020204,
    0x614410c00080, 0x208248208080, 0x1020a4108080, 0x80030888080,
    0x4100a028080, 0x6040c028080, 0x4840504060080, 0x2820243020080,
    0xfffcddfcfd714a, 0x7ffcfdfeed714a, 0x3fffcdffdc9096, 0x4081020a901,
    0x10002140a0211, 0x1000604080a01, 0x1040486040401, 0x3fffaabfad3a2
};

//non-sliding pieces
const uint64_t KnightMoves[64]= {
    0x20400, 0x50800, 0xa1100, 0x142200,
	0x284400, 0x508800, 0xa01000, 0x402000,
	0x2040004, 0x5080008, 0xa110011, 0x14220022,
	0x28440044, 0x50880088, 0xa0100010, 0x40200020,
	0x204000402, 0x508000805, 0xa1100110a, 0x1422002214,
	0x2844004428, 0x5088008850, 0xa0100010a0, 0x4020002040,
	0x20400040200, 0x50800080500, 0xa1100110a00, 0x142200221400,
	0x284400442800, 0x508800885000, 0xa0100010a000, 0x402000204000,
	0x2040004020000, 0x5080008050000, 0xa1100110a0000, 0x14220022140000,
	0x28440044280000, 0x50880088500000, 0xa0100010a00000, 0x40200020400000,
	0x204000402000000, 0x508000805000000, 0xa1100110a000000, 0x1422002214000000,
	0x2844004428000000, 0x5088008850000000, 0xa0100010a0000000, 0x4020002040000000,
	0x400040200000000, 0x800080500000000, 0x1100110a00000000, 0x2200221400000000,
	0x4400442800000000, 0x8800885000000000, 0x100010a000000000, 0x2000204000000000,
	0x4020000000000, 0x8050000000000, 0x110a0000000000, 0x22140000000000,
	0x44280000000000, 0x0088500000000000, 0x0010a00000000000, 0x20400000000000
};

const uint64_t KingMoves[64]= {
    0x302, 0x705, 0xe0a, 0x1c14,
	0x3828, 0x7050, 0xe0a0, 0xc040,
	0x30203, 0x70507, 0xe0a0e, 0x1c141c,
	0x382838, 0x705070, 0xe0a0e0, 0xc040c0,
	0x3020300, 0x7050700, 0xe0a0e00, 0x1c141c00,
	0x38283800, 0x70507000, 0xe0a0e000, 0xc040c000,
	0x302030000, 0x705070000, 0xe0a0e0000, 0x1c141c0000,
	0x3828380000, 0x7050700000, 0xe0a0e00000, 0xc040c00000,
	0x30203000000, 0x70507000000, 0xe0a0e000000, 0x1c141c000000,
	0x382838000000, 0x705070000000, 0xe0a0e0000000, 0xc040c0000000,
	0x3020300000000, 0x7050700000000, 0xe0a0e00000000, 0x1c141c00000000,
	0x38283800000000, 0x70507000000000, 0xe0a0e000000000, 0xc040c000000000,
	0x302030000000000, 0x705070000000000, 0xe0a0e0000000000, 0x1c141c0000000000,
	0x3828380000000000, 0x7050700000000000, 0xe0a0e00000000000, 0xc040c00000000000,
	0x203000000000000, 0x507000000000000, 0xa0e000000000000, 0x141c000000000000,
	0x2838000000000000, 0x5070000000000000, 0xa0e0000000000000, 0x40c0000000000000
};

//pawns
const uint64_t WPawnMoves[64]= {
    0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x2, 0x5, 0xa, 0x14,
	0x28, 0x50, 0xa0, 0x40,
	0x200, 0x500, 0xa00, 0x1400,
	0x2800, 0x5000, 0xa000, 0x4000,
	0x20000, 0x50000, 0xa0000, 0x140000,
	0x280000, 0x500000, 0xa00000, 0x400000,
	0x2000000, 0x5000000, 0xa000000, 0x14000000,
	0x28000000, 0x50000000, 0xa0000000, 0x40000000,
	0x200000000, 0x500000000, 0xa00000000, 0x1400000000,
	0x2800000000, 0x5000000000, 0xa000000000, 0x4000000000,
	0x20000000000, 0x50000000000, 0xa0000000000, 0x140000000000,
	0x280000000000, 0x500000000000, 0xa00000000000, 0x400000000000,
	0x2000000000000, 0x5000000000000, 0xa000000000000, 0x14000000000000,
	0x28000000000000, 0x50000000000000, 0xa0000000000000, 0x40000000000000
};

// Precomputed pawn attack tables
const uint64_t WPawnAttacks[64] = {
    0x0000000000000200ULL, 0x0000000000000500ULL, 0x0000000000000A00ULL, 0x0000000000001400ULL,
    0x0000000000002800ULL, 0x0000000000005000ULL, 0x000000000000A000ULL, 0x0000000000004000ULL,
    0x0000000000020000ULL, 0x0000000000050000ULL, 0x00000000000A0000ULL, 0x0000000000140000ULL,
    0x0000000000280000ULL, 0x0000000000500000ULL, 0x0000000000A00000ULL, 0x0000000000400000ULL,
    0x0000000002000000ULL, 0x0000000005000000ULL, 0x000000000A000000ULL, 0x0000000014000000ULL,
    0x0000000028000000ULL, 0x0000000050000000ULL, 0x00000000A0000000ULL, 0x0000000040000000ULL,
    0x0000000200000000ULL, 0x0000000500000000ULL, 0x0000000A00000000ULL, 0x0000001400000000ULL,
    0x0000002800000000ULL, 0x0000005000000000ULL, 0x000000A000000000ULL, 0x0000004000000000ULL,
    0x0000020000000000ULL, 0x0000050000000000ULL, 0x00000A0000000000ULL, 0x0000140000000000ULL,
    0x0000280000000000ULL, 0x0000500000000000ULL, 0x0000A00000000000ULL, 0x0000400000000000ULL,
    0x0002000000000000ULL, 0x0005000000000000ULL, 0x000A000000000000ULL, 0x0014000000000000ULL,
    0x0028000000000000ULL, 0x0050000000000000ULL, 0x00A0000000000000ULL, 0x0040000000000000ULL,
    0x0200000000000000ULL, 0x0500000000000000ULL, 0x0A00000000000000ULL, 0x1400000000000000ULL,
    0x2800000000000000ULL, 0x5000000000000000ULL, 0xA000000000000000ULL, 0x4000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL
};

const uint64_t BPawnAttacks[64] = {
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000002ULL, 0x0000000000000005ULL, 0x000000000000000AULL, 0x0000000000000014ULL,
    0x0000000000000028ULL, 0x0000000000000050ULL, 0x00000000000000A0ULL, 0x0000000000000040ULL,
    0x0000000000000200ULL, 0x0000000000000500ULL, 0x0000000000000A00ULL, 0x0000000000001400ULL,
    0x0000000000002800ULL, 0x0000000000005000ULL, 0x000000000000A000ULL, 0x0000000000004000ULL,
    0x0000000000020000ULL, 0x0000000000050000ULL, 0x00000000000A0000ULL, 0x0000000000140000ULL,
    0x0000000000280000ULL, 0x0000000000500000ULL, 0x0000000000A00000ULL, 0x0000000000400000ULL,
    0x0000000002000000ULL, 0x0000000005000000ULL, 0x000000000A000000ULL, 0x0000000014000000ULL,
    0x0000000028000000ULL, 0x0000000050000000ULL, 0x00000000A0000000ULL, 0x0000000040000000ULL,
    0x0000000200000000ULL, 0x0000000500000000ULL, 0x0000000A00000000ULL, 0x0000001400000000ULL,
    0x0000002800000000ULL, 0x0000005000000000ULL, 0x000000A000000000ULL, 0x0000004000000000ULL,
    0x0000020000000000ULL, 0x0000050000000000ULL, 0x00000A0000000000ULL, 0x0000140000000000ULL,
    0x0000280000000000ULL, 0x0000500000000000ULL, 0x0000A00000000000ULL, 0x0000400000000000ULL
};

const uint64_t BPawnMoves[64]= {
    0x200, 0x500, 0xa00, 0x1400,
	0x2800, 0x5000, 0xa000, 0x4000,
	0x20000, 0x50000, 0xa0000, 0x140000,
	0x280000, 0x500000, 0xa00000, 0x400000,
	0x2000000, 0x5000000, 0xa000000, 0x14000000,
	0x28000000, 0x50000000, 0xa0000000, 0x40000000,
	0x200000000, 0x500000000, 0xa00000000, 0x1400000000,
	0x2800000000, 0x5000000000, 0xa000000000, 0x4000000000,
	0x20000000000, 0x50000000000, 0xa0000000000, 0x140000000000,
	0x280000000000, 0x500000000000, 0xa00000000000, 0x400000000000,
	0x2000000000000, 0x5000000000000, 0xa000000000000, 0x14000000000000,
	0x28000000000000, 0x50000000000000, 0xa0000000000000, 0x40000000000000,
	0x200000000000000, 0x500000000000000, 0xa00000000000000, 0x1400000000000000,
	0x2800000000000000, 0x5000000000000000, 0xa000000000000000, 0x4000000000000000,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0
};

//helpers for gamestate tracking
bool whiteKingMoved;
bool blackKingMoved;
bool whiteKingsideRookMoved;
bool whiteQueensideRookMoved;
bool blackKingsideRookMoved;
bool blackQueensideRookMoved;
bool isWhiteTurn;

// En passant target square (-1 means no en passant available)
int enPassantTargetSquare;

bool squareAttacked(int square, Color byColor) {
    // Knights
    for (int sq = 0; sq < 64; ++sq) {
        int piece = board[sq];
        if ((byColor == WHITE && isWhite(piece) && piece == WHITE_KNIGHT) ||
            (byColor == BLACK && isBlack(piece) && piece == BLACK_KNIGHT)) {
            if (KnightMoves[sq] & (1ULL << square)) return true;
        }
    }
    // Pawns
    if (byColor == WHITE) {
        for (int sq = 0; sq < 64; ++sq) {
            int piece = board[sq];
            if (isWhite(piece) && piece == WHITE_PAWN && WPawnAttacks[sq] & (1ULL << square)) return true;
        }
    } else {
        for (int sq = 0; sq < 64; ++sq) {
            int piece = board[sq];
            if (isBlack(piece) && piece == BLACK_PAWN && BPawnAttacks[sq] & (1ULL << square)) return true;
        }
    }
    // Kings
    for (int sq = 0; sq < 64; ++sq) {
        int piece = board[sq];
        if ((byColor == WHITE && isWhite(piece) && piece == WHITE_KING) ||
            (byColor == BLACK && isBlack(piece) && piece == BLACK_KING)) {
            if (KingMoves[sq] & (1ULL << square)) return true;
        }
    }
    // Bishops and Queens (diagonals)
    for (int sq = 0; sq < 64; ++sq) {
        int piece = board[sq];
        if ((byColor == WHITE && isWhite(piece) && (piece == WHITE_BISHOP || piece == WHITE_QUEEN)) ||
            (byColor == BLACK && isBlack(piece) && (piece == BLACK_BISHOP || piece == BLACK_QUEEN))) {
            if (BishopMoves[sq] & (1ULL << square)) {
                // Inline path check
                int fromRow = sq / 8, fromCol = sq % 8;
                int toRow = square / 8, toCol = square % 8;
                int dRow = (toRow > fromRow) ? 1 : -1;
                int dCol = (toCol > fromCol) ? 1 : -1;
                int row = fromRow + dRow, col = fromCol + dCol;
                bool blocked = false;
                while (row != toRow && col != toCol) {
                    if (board[row * 8 + col] != EMPTY) { blocked = true; break; }
                    row += dRow; col += dCol;
                }
                if (!blocked) return true;
            }
        }
    }
    // Rooks and Queens (straight lines)
    for (int sq = 0; sq < 64; ++sq) {
        int piece = board[sq];
        if ((byColor == WHITE && isWhite(piece) && (piece == WHITE_ROOK || piece == WHITE_QUEEN)) ||
            (byColor == BLACK && isBlack(piece) && (piece == BLACK_ROOK || piece == BLACK_QUEEN))) {
            if (RookMoves[sq] & (1ULL << square)) {
                // Inline path check
                int fromRow = sq / 8, fromCol = sq % 8;
                int toRow = square / 8, toCol = square % 8;
                int dRow = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                int dCol = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
                int row = fromRow + dRow, col = fromCol + dCol;
                bool blocked = false;
                while (row != toRow || col != toCol) {
                    if (board[row * 8 + col] != EMPTY) { blocked = true; break; }
                    row += dRow; col += dCol;
                }
                if (!blocked) return true;
            }
        }
    }
    return false;
}

bool inCheck(int color) {
    int kingSquare = -1;
    // Find the king's square
    for (int sq = 0; sq < 64; ++sq) {
        if (board[sq] == (color == WHITE ? WHITE_KING : BLACK_KING)) {
            kingSquare = sq;
            break;
        }
    }
    // If not found, return false or handle as error
    if (kingSquare == -1) return false;
    // Check if the king's square is attacked by the opponent
    return squareAttacked(kingSquare, color == WHITE ? BLACK : WHITE);
}

void generatePseudoLegalMoves(MoveList& moves, Color color) {
    for (int sq = 0; sq < 64; ++sq) {
        int piece = board[sq];
        if (piece == EMPTY) continue;
        if ((color == WHITE && !isWhite(piece)) || (color == BLACK && !isBlack(piece)))
            continue;

        // Knights
        if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT) {
            uint64_t targets = KnightMoves[sq];
            for (int t = 0; t < 64; ++t) {
                if (targets & (1ULL << t)) {
                    if (board[t] == EMPTY || isOpponentPiece(board[t], color))
                        moves.add(Move(sq, t));
                }
            }
        }
        // Bishops
        else if (piece == WHITE_BISHOP || piece == BLACK_BISHOP) {
            uint64_t targets = BishopMoves[sq];
            for (int t = 0; t < 64; ++t) {
                if (targets & (1ULL << t)) {
                    // Path blocking check
                    int fromRow = sq / 8, fromCol = sq % 8;
                    int toRow = t / 8, toCol = t % 8;
                    int dRow = (toRow > fromRow) ? 1 : -1;
                    int dCol = (toCol > fromCol) ? 1 : -1;
                    int row = fromRow + dRow, col = fromCol + dCol;
                    bool blocked = false;
                    while (row != toRow && col != toCol) {
                        if (board[row * 8 + col] != EMPTY) { blocked = true; break; }
                        row += dRow; col += dCol;
                    }
                    if (!blocked && (board[t] == EMPTY || isOpponentPiece(board[t], color)))
                        moves.add(Move(sq, t));
                }
            }
        }
        // Rooks
        else if (piece == WHITE_ROOK || piece == BLACK_ROOK) {
            uint64_t targets = RookMoves[sq];
            for (int t = 0; t < 64; ++t) {
                if (targets & (1ULL << t)) {
                    int fromRow = sq / 8, fromCol = sq % 8;
                    int toRow = t / 8, toCol = t % 8;
                    int dRow = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                    int dCol = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
                    int row = fromRow + dRow, col = fromCol + dCol;
                    bool blocked = false;
                    while (row != toRow || col != toCol) {
                        if (board[row * 8 + col] != EMPTY) { blocked = true; break; }
                        row += dRow; col += dCol;
                    }
                    if (!blocked && (board[t] == EMPTY || isOpponentPiece(board[t], color)))
                        moves.add(Move(sq, t));
                }
            }
        }
        // Queens
        else if (piece == WHITE_QUEEN || piece == BLACK_QUEEN) {
            uint64_t targets = QueenMoves[sq];
            for (int t = 0; t < 64; ++t) {
                if (targets & (1ULL << t)) {
                    // Queen can move like both rook and bishop, so check both blockings
                    int fromRow = sq / 8, fromCol = sq % 8;
                    int toRow = t / 8, toCol = t % 8;
                    int dRow = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                    int dCol = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
                    if (dRow == 0 && dCol == 0) continue;
                    int row = fromRow + dRow, col = fromCol + dCol;
                    bool blocked = false;
                    while (row != toRow || col != toCol) {
                        if (board[row * 8 + col] != EMPTY) { blocked = true; break; }
                        row += dRow; col += dCol;
                    }
                    if (!blocked && (board[t] == EMPTY || isOpponentPiece(board[t], color)))
                        moves.add(Move(sq, t));
                }
            }
        }
        // King
        else if (piece == WHITE_KING || piece == BLACK_KING) {
            uint64_t targets = KingMoves[sq];
            for (int t = 0; t < 64; ++t) {
                if (targets & (1ULL << t)) {
                    if (board[t] == EMPTY || isOpponentPiece(board[t], color))
                        moves.add(Move(sq, t));
                }
            }
        }
        // Pawns
        else if (piece == WHITE_PAWN || piece == BLACK_PAWN) {
            int dir = (color == WHITE) ? 8 : -8;
            int startRank = (color == WHITE) ? 1 : 6;
            int sqRank = sq / 8;
            int forward = sq + dir;
            // Single push
            if (forward >= 0 && forward < 64 && board[forward] == EMPTY) {
                moves.add(Move(sq, forward));
                // Double push
                if (sqRank == startRank) {
                    int doubleForward = sq + 2 * dir;
                    if (board[doubleForward] == EMPTY)
                        moves.add(Move(sq, doubleForward));
                }
            }
            // Captures
            for (int d : {-1, 1}) {
                int file = sq % 8;
                int cap = sq + dir + d;
                if (cap >= 0 && cap < 64 && ((d == -1 && file > 0) || (d == 1 && file < 7))) {
                    if (isOpponentPiece(board[cap], color))
                        moves.add(Move(sq, cap));
                }
            }
        }
    }
}

void generatePawnPromotionMoves(MoveList& moves, Color color) {
    int startRank = (color == WHITE) ? 6 : 1;
    int pawn = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
    const int promoPieces[4] = {
        color == WHITE ? WHITE_QUEEN : BLACK_QUEEN,
        color == WHITE ? WHITE_ROOK  : BLACK_ROOK,
        color == WHITE ? WHITE_BISHOP: BLACK_BISHOP,
        color == WHITE ? WHITE_KNIGHT: BLACK_KNIGHT
    };

    for (int sq = startRank * 8; sq < (startRank + 1) * 8; ++sq) {
        if (board[sq] == pawn) {
            // Forward move
            int target = sq + ((color == WHITE) ? 8 : -8);
            if (board[target] == EMPTY) {
                for (int i = 0; i < 4; ++i) {
                    moves.add(Move(sq, target, PAWN_PROMOTION, promoPieces[i]));
                }
            }
            // Captures
            for (int dir : {-1, 1}) {
                int file = sq % 8;
                if ((file == 0 && dir == -1) || (file == 7 && dir == 1)) continue;
                int capTarget = target + dir;
                if (isOpponentPiece(board[capTarget], color)) {
                    for (int i = 0; i < 4; ++i) {
                        moves.add(Move(sq, capTarget, PAWN_PROMOTION, promoPieces[i]));
                    }
                }
            }
        }
    }
}

void generateEnPassantMoves(MoveList& moves, Color color) {
    if (enPassantTargetSquare == -1) return;
    int pawn = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
    int rank = (color == WHITE) ? 4 : 3;
    for (int file = 0; file < 8; ++file) {
        int sq = rank * 8 + file;
        if (board[sq] == pawn) {
            for (int dir : {-1, 1}) {
                int capFile = file + dir;
                if (capFile < 0 || capFile > 7) continue;
                int capSq = rank * 8 + capFile;
                if (capSq == enPassantTargetSquare) {
                    moves.add(Move(sq, enPassantTargetSquare, EN_PASSANT));
                }
            }
        }
    }
}

void generateCastlingMoves(MoveList& moves, Color color) {
    //white
    if (color == WHITE && !whiteKingMoved) {
        // Kingside
        if (!whiteKingsideRookMoved &&
            board[5] == EMPTY && board[6] == EMPTY &&
            !squareAttacked(4, BLACK) && !squareAttacked(5, BLACK) && !squareAttacked(6, BLACK)) {
            moves.add(Move(4, 6, CASTLING_KINGSIDE));
        }
        // Queenside
        if (!whiteQueensideRookMoved &&
            board[1] == EMPTY && board[2] == EMPTY && board[3] == EMPTY &&
            !squareAttacked(2, BLACK) && !squareAttacked(3, BLACK) && !squareAttacked(4, BLACK)) {
            moves.add(Move(4, 2, CASTLING_QUEENSIDE));
        }
    }
	//black
    else if (color == BLACK && !blackKingMoved) {
        // Kingside
        if (!blackKingsideRookMoved &&
            board[5] == EMPTY && board[6] == EMPTY &&
            !squareAttacked(4, WHITE) && !squareAttacked(5, WHITE) && !squareAttacked(6, WHITE)) {
            moves.add(Move(4, 6, CASTLING_KINGSIDE));
        }
        // Queenside
        if (!blackQueensideRookMoved &&
            board[1] == EMPTY && board[2] == EMPTY && board[3] == EMPTY &&
            !squareAttacked(2, WHITE) && !squareAttacked(3, WHITE) && !squareAttacked(4, WHITE)) {
            moves.add(Move(4, 2, CASTLING_QUEENSIDE));
        }
    }
}

MoveList generateLegalmoves() {
    MoveList pseudoMoves, legalMoves;
    Color color = isWhiteTurn ? WHITE : BLACK;
    generatePseudoLegalMoves(pseudoMoves, color);
    generatePawnPromotionMoves(pseudoMoves, color);
    generateEnPassantMoves(pseudoMoves, color);
    generateCastlingMoves(pseudoMoves, color);

    for (int i = 0; i < pseudoMoves.count; ++i) {
        makeMove(pseudoMoves.moves[i]);
        if (!inCheck(color)) {
            legalMoves.add(pseudoMoves.moves[i]);
        }
        undoMove();
    }
    return legalMoves;
}