#ifndef BOARD_H
#define BOARD_H

#include <cstring>
#include <iostream>
#include <memory>
#include "Defs.h"
#include "Piece.h" 

#define NUM_PIECES 15 

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

        isWhiteToMove = board_->isWhiteToMove;

        lastMove = board_->lastMove;
    }

    Board(const std::string fenString) {
        board.resize(nele, nullptr);
        init(fenString);
    }

    ~Board() {}

    template<bool isWhite>
    static std::shared_ptr<Piece> createPiece(PieceType type, int row, int col, int id) {
        switch (type) {
            case PieceType::PAWN:
                return std::make_shared<Pawn<isWhite> >(row, col, id);
            case PieceType::ROOK:
                return std::make_shared<Rook<isWhite> >(row, col, id);
            case PieceType::KNIGHT:
                return std::make_shared<Knight<isWhite> >(row, col, id);
            case PieceType::BISHOP:
                return std::make_shared<Bishop<isWhite> >(row, col, id);
            case PieceType::QUEEN:
                return std::make_shared<Queen<isWhite> >(row, col, id);
            case PieceType::KING:
                return std::make_shared<King<isWhite> >(row, col, id);
            default:
                return nullptr;
        }
    }

    Piece* getPiece(int loc) const {
        return board[loc].get();
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
        return isWhite ? dynamic_cast<King<isWhite>*>(board[whiteKingPos].get()) : dynamic_cast<King<isWhite>*>(board[blackKingPos].get());
    }

    template<bool isWhite>
    const std::vector<std::shared_ptr<Piece> >& getPieces() const {
        return isWhite ? whitePieces : blackPieces;
    }

    void printBoard() {
        for (int row = 7; row >= 0; row--) {
            printf("%d ", row + 1);
            for (int col = 0; col < 8; col++) {
                printf("%s ", getPieceSymbol(board[col + row*ncol].get()).c_str());
            }
            printf("\n");
        }
        printf("  a b c d e f g h\n");
    }


    int whiteKingPos, blackKingPos;
    std::vector<std::shared_ptr<Piece> > whitePieces, blackPieces; // no king
    bool isWhiteToMove = true;

private:
    std::vector<std::shared_ptr<Piece> > board;
    Move lastMove, lastMove2;
    int lastMovePieceChangeID = -1;
    bool lastMoveFromInitPos = true;

    void setupPieces() {
        whitePieces.resize(NUM_PIECES, nullptr);
        blackPieces.resize(NUM_PIECES, nullptr);

        for (int i = 0; i < nele; i++) {
            if (board[i] != nullptr && board[i]->_type != PieceType::KING) {
                if (board[i]->_isWhite) 
                    whitePieces[board[i]->_id] = board[i];
                else 
                    blackPieces[board[i]->_id] = board[i];
            }
        }
    }

    void init() {
        std::cout << "Board::init" << std::endl;
        int whiteID = 0, blackID = 0;
        for (int c = 0; c < ncol; c++) {
            board[c +   ncol] = createPiece<true >(PieceType::PAWN, 1, c, whiteID++); 
            board[c + 6*ncol] = createPiece<false>(PieceType::PAWN, 6, c, blackID++); 
        }     

        board[0         ] = createPiece<true> (PieceType::ROOK, 0, 0, whiteID++);
        board[7         ] = createPiece<true >(PieceType::ROOK, 0, 7, whiteID++);
        board[0 + 7*ncol] = createPiece<false>(PieceType::ROOK, 7, 0, blackID++);
        board[7 + 7*ncol] = createPiece<false>(PieceType::ROOK, 7, 7, blackID++);

        board[1         ] = createPiece<true >(PieceType::KNIGHT, 0, 1, whiteID++);
        board[6         ] = createPiece<true >(PieceType::KNIGHT, 0, 6, whiteID++);
        board[1 + 7*ncol] = createPiece<false>(PieceType::KNIGHT, 7, 1, blackID++);
        board[6 + 7*ncol] = createPiece<false>(PieceType::KNIGHT, 7, 6, blackID++);

        board[2         ] = createPiece<true >(PieceType::BISHOP, 0, 2, whiteID++);
        board[5         ] = createPiece<true >(PieceType::BISHOP, 0, 5, whiteID++);
        board[2 + 7*ncol] = createPiece<false>(PieceType::BISHOP, 7, 2, blackID++);
        board[5 + 7*ncol] = createPiece<false>(PieceType::BISHOP, 7, 5, blackID++);

        board[3         ] = createPiece<true >(PieceType::QUEEN, 0, 3, whiteID++);
        board[3 + 7*ncol] = createPiece<false>(PieceType::QUEEN, 7, 3, blackID++);

        board[4         ] = createPiece<true >(PieceType::KING, 0, 4, 15);
        board[4 + 7*ncol] = createPiece<false>(PieceType::KING, 7, 4, 15);

        whiteKingPos = 4;
        blackKingPos = 4 + 7*ncol;   

        setupPieces();  
    };

    void parsePiecePlacement(const std::string& placement);
    void parseActiveColor(const std::string &color);
    void parseCastlingAvailability(const std::string& castlingAvailability);
    void parseEnPassantTargetSquare(const std::string& enPassantTargetSquare);

    void init(std::string fenString);

    std::string getPieceSymbol(const Piece* piece) {
        if (piece == nullptr) 
            return "-";
    
        std::string colorCode;
        std::string resetCode = "\033[0m";

        if (piece->_isWhite) {
            colorCode = "\033[34m"; // Blue for white pieces
        } else {
            colorCode = "\033[32m"; // Green for black pieces
        }

        std::string symbol;
        if (piece->_isWhite) {
            switch (piece->_type) {
                case PieceType::PAWN:   symbol = "P"; break;
                case PieceType::KNIGHT: symbol = "N"; break;
                case PieceType::BISHOP: symbol = "B"; break;
                case PieceType::ROOK:   symbol = "R"; break;
                case PieceType::QUEEN:  symbol = "Q"; break;
                case PieceType::KING:   symbol = "K"; break;
                default: symbol = "-";
            }
        } else {
            switch (piece->_type) {
                case PieceType::PAWN:   symbol = "p"; break;
                case PieceType::KNIGHT: symbol = "n"; break;
                case PieceType::BISHOP: symbol = "b"; break;
                case PieceType::ROOK:   symbol = "r"; break;
                case PieceType::QUEEN:  symbol = "q"; break;
                case PieceType::KING:   symbol = "k"; break;
                default: symbol = "-";
            }
        }

        return colorCode + symbol + resetCode;
    }
};

#endif //BOARD_H
