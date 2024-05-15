#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include <cstring>
#include "Piece.h"
#include "Defs.h"

#define NUM_PIECES 15 // no king

class Board {
public:

    Board() {
        board = new Piece*[nele];
        for (int i = 0; i < nele; i++) 
            board[i] = nullptr; 
        init();
    }

    Board(const Board* board_) {
        board = new Piece*[nele];
        for (int i = 0; i < nele; i++) {
            if (board_->board[i] != nullptr) {
                board[i] = board_->board[i]->clone();
            }
            else {
                board[i] = nullptr;
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
        for (int i = 0; i < nele; i++) 
            if (board[i] != nullptr)
                delete board[i];
        delete[] board;
    }

    template<bool isWhite>
    static Piece* createPiece(PieceType type, int row, int col, int id) {
        switch (type) {
            case PAWN:
                return createPiece<isWhite>(PieceType::PAWN,row, col, id);
            case ROOK:
                return createPiece<isWhite>(PieceType::ROOK,row, col, id);
            case KNIGHT:
                return createPiece<isWhite>(PieceType::KNIGHT,row, col, id);
            case BISHOP:
                return createPiece<isWhite>(PieceType::BISHOP,row, col, id);
            case QUEEN:
                return createPiece<isWhite>(PieceType::QUEEN,row, col, id);
            case KING:
                return createPiece<isWhite>(PieceType::KING,row, col, id);
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

    template<bool isWhite>
    const std::vector<Piece*>& getPieces() const {
        return isWhite ? whitePieces : blackPieces;
    }

    int whiteKingPos, blackKingPos;
    std::vector<Piece*> whitePieces, blackPieces; // no king

private:
    Piece** board;
    Move lastMove, lastMove2;
    Piece* lastMovePieceChange = nullptr;
    bool lastMoveFromInitPos = true;

    void setupPieces() {
        whitePieces.resize(NUM_PIECES);
        blackPieces.resize(NUM_PIECES);

        int ind_white = 0, ind_black = 0;
        for (int i = 0; i < nele; i++) {
            if (board[i] != nullptr && board[i]->_type) {
                if (board[i]->_isWhite) {
                    whitePieces[ind_white++] = board[i];
                }
                else {
                    blackPieces[ind_black++] = board[i];
                }
            }
        }
    }

    void init() {
        for (int c = 0; c < ncol; c++) {
            board[c +   ncol] = createPiece<true >(PieceType::PAWN, 1, c, 7+c); 
            board[c + 6*ncol] = createPiece<false>(PieceType::PAWN, 1, c, 7+c); 
        }     

        board[0         ] = createPiece<true> (PieceType::ROOK, 0, 0, 1);
        board[7         ] = createPiece<true >(PieceType::ROOK, 0, 7, 2);
        board[0 + 7*ncol] = createPiece<false>(PieceType::ROOK, 7, 0, 1);
        board[7 + 7*ncol] = createPiece<false>(PieceType::ROOK, 7, 7, 2);

        board[1         ] = createPiece<true >(PieceType::KNIGHT,0, 1, 3);
        board[6         ] = createPiece<true >(PieceType::KNIGHT,0, 6, 4);
        board[1 + 7*ncol] = createPiece<false>(PieceType::KNIGHT,7, 1, 3);
        board[6 + 7*ncol] = createPiece<false>(PieceType::KNIGHT,7, 6, 4);

        board[2         ] = createPiece<true >(PieceType::BISHOP,0, 2, 5);
        board[5         ] = createPiece<true >(PieceType::BISHOP,0, 5, 6);
        board[2 + 7*ncol] = createPiece<false>(PieceType::BISHOP,7, 2, 5);
        board[5 + 7*ncol] = createPiece<false>(PieceType::BISHOP,7, 5, 6);

        board[3         ] = createPiece<true >(PieceType::QUEEN,0, 3, 0);
        board[3 + 7*ncol] = createPiece<false>(PieceType::QUEEN,7, 3, 0);

        board[4         ] = createPiece<true >(PieceType::KING,0, 4, 15);
        board[4 + 7*ncol] = createPiece<false>(PieceType::KING,7, 4, 15);

        whiteKingPos = 3;
        blackKingPos = 4 + 7*ncol;   

        setupPieces();     
    };
};

#endif //BOARD_H
