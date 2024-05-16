#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include <cstring>
#include <iostream>
#include "Defs.h"
#include "Piece.h" 

#define NUM_PIECES 15 
#define QUEEN_ID 0
#define FIRST_ROOK_ID 1
#define FIRST_KNIGHT_ID 3
#define FIRST_BISHOP_ID 5
#define FIRST_PAWN_ID 8

//class Piece;
template<bool isWhite> class Pawn;
template<bool isWhite> class Rook;
template<bool isWhite> class Knight;
template<bool isWhite> class Bishop;
template<bool isWhite> class Queen;
template<bool isWhite> class King;

class Board {
public:

    Board() {
        board.resize(nele, nullptr); 
        init();
    }

    Board(const Board* board_) {
        board.resize(nele, nullptr);
        for (int i = 0; i < nele; i++) {
            if (board_->board[i] != nullptr) {
                board[i] = board_->board[i]->clone();
            }
        }
        whiteKingPos = board_->whiteKingPos;    
        blackKingPos = board_->blackKingPos;

        whitePieces.resize(NUM_PIECES);
        blackPieces.resize(NUM_PIECES);
        for (int i = 0; i < NUM_PIECES; i++) {
            whitePieces[i] = board[board_->whitePieces[i]->getPosition()];
            blackPieces[i] = board[board_->blackPieces[i]->getPosition()];
        }
    }

    ~Board() {
        delete board[whiteKingPos];
        delete board[blackKingPos]; 
        for (int i = 0; i < NUM_PIECES; i++) {
            if (whitePieces[i] != nullptr) {
                delete whitePieces[i];
            }
            if (blackPieces[i] != nullptr)
                delete blackPieces[i];
        }
    }

    template<bool isWhite>
    static Piece* createPiece(PieceType type, int row, int col, int id) {
        switch (type) {
            case PieceType::PAWN:
                return new Pawn<isWhite>(row, col, id);
            case PieceType::ROOK:
                return new Rook<isWhite>(row, col, id);
            case PieceType::KNIGHT:
                return new Knight<isWhite>(row, col, id);
            case PieceType::BISHOP:
                return new Bishop<isWhite>(row, col, id);
            case PieceType::QUEEN:
                return new Queen<isWhite>(row, col, id);
            case PieceType::KING:
                return new King<isWhite>(row, col, id);
            default:
                return nullptr;
        }
    }

    Piece* getPiece(int loc) const {
        return board[loc];
    }

    const Move& getLastMove() const {
        return lastMove;
    }

    template<bool isWhite>
    void makeMove(const Move& move);
    
    template<bool isWhite>
    void undoLastMove();

    template<bool isWhite>
    King<isWhite>* getKing() const {
        return isWhite ? dynamic_cast<King<isWhite>*>(board[whiteKingPos]) : dynamic_cast<King<isWhite>*>(board[blackKingPos]);
    }

    // return a list of white/black pieces without the king
    template<bool isWhite>
    const std::vector<Piece*>& getPieces() const {
        return isWhite ? whitePieces : blackPieces;
    }

    void printBoard() {
        for (int row = nrow-1; row >= 0; row--) {
            std::cout << row + 1 << " ";
            for (int col = 0; col < ncol; col++) {
                std::cout << getPieceSymbol(board[col + row*ncol]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "  a b c d e f g h" << std::endl;
    }


    int whiteKingPos, blackKingPos;
    std::vector<Piece*> whitePieces, blackPieces; // no king

private:
    std::vector<Piece*> board;
    Move lastMove, lastMove2;
    Piece* lastMovePieceChange = nullptr;
    bool lastMoveFromInitPos = true;

    void setupPieces() {
        whitePieces.resize(NUM_PIECES);
        blackPieces.resize(NUM_PIECES);

        for (int i = 0; i < nele; i++) {
            if (board[i] != nullptr && board[i]->_type != PieceType::KING) {
                if (board[i]->_isWhite) {
                    whitePieces[board[i]->_id] = board[i];
                }
                else {
                    blackPieces[board[i]->_id] = board[i];
                }
            }
        }
    }

    void init() {
        for (int c = 0; c < ncol; c++) {
            board[c +   ncol] = createPiece<true >(PieceType::PAWN, 1, c, FIRST_PAWN_ID+c); 
            board[c + 6*ncol] = createPiece<false>(PieceType::PAWN, 1, c, FIRST_PAWN_ID+c); 
        }     

        board[0         ] = createPiece<true> (PieceType::ROOK, 0, 0, FIRST_ROOK_ID);
        board[7         ] = createPiece<true >(PieceType::ROOK, 0, 7, FIRST_ROOK_ID+1);
        board[0 + 7*ncol] = createPiece<false>(PieceType::ROOK, 7, 0, FIRST_ROOK_ID);
        board[7 + 7*ncol] = createPiece<false>(PieceType::ROOK, 7, 7, FIRST_ROOK_ID+1);

        board[1         ] = createPiece<true >(PieceType::KNIGHT,0, 1, FIRST_KNIGHT_ID);
        board[6         ] = createPiece<true >(PieceType::KNIGHT,0, 6, FIRST_KNIGHT_ID+1);
        board[1 + 7*ncol] = createPiece<false>(PieceType::KNIGHT,7, 1, FIRST_KNIGHT_ID);
        board[6 + 7*ncol] = createPiece<false>(PieceType::KNIGHT,7, 6, FIRST_KNIGHT_ID+1);

        board[2         ] = createPiece<true >(PieceType::BISHOP,0, 2, FIRST_BISHOP_ID);
        board[5         ] = createPiece<true >(PieceType::BISHOP,0, 5, FIRST_BISHOP_ID+1);
        board[2 + 7*ncol] = createPiece<false>(PieceType::BISHOP,7, 2, FIRST_BISHOP_ID);
        board[5 + 7*ncol] = createPiece<false>(PieceType::BISHOP,7, 5, FIRST_BISHOP_ID+1);

        board[3         ] = createPiece<true >(PieceType::QUEEN,0, 3, QUEEN_ID);
        board[3 + 7*ncol] = createPiece<false>(PieceType::QUEEN,7, 3, QUEEN_ID);

        board[4         ] = createPiece<true >(PieceType::KING,0, 4, 15);
        board[4 + 7*ncol] = createPiece<false>(PieceType::KING,7, 4, 15);

        whiteKingPos = 3;
        blackKingPos = 4 + 7*ncol;   

        setupPieces();     
    };

    std::string getPieceSymbol(const Piece* piece) {
        if (piece == nullptr) 
            return " ";
        
        if (piece->_isWhite) {
            switch (piece->_type) {
                case PieceType::PAWN:   return "P";
                case PieceType::KNIGHT: return "N";
                case PieceType::BISHOP: return "B";
                case PieceType::ROOK:   return "R";
                case PieceType::QUEEN:  return "Q";
                case PieceType::KING:   return "K";
                default: return " ";
            }
        } else {
            switch (piece->_type) {
                case PieceType::PAWN:   return "p";
                case PieceType::KNIGHT: return "n";
                case PieceType::BISHOP: return "b";
                case PieceType::ROOK:   return "r";
                case PieceType::QUEEN:  return "q";
                case PieceType::KING:   return "k";
                default: return " ";
            }
        }
        return " ";
    }
};

#endif //BOARD_H
