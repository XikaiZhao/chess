#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <memory>
#include "Defs.h"

class Board {
public:
    Board() {
        _board = "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr";
    }

    Board(const Board &board) {
        this->_board = board.getBoard();
        this->whiteCanCastleKingSide = board.whiteCanCastleKingSide;
        this->whiteCanCastleQueenSide = board.whiteCanCastleQueenSide;
        this->blackCanCastleKingSide = board.blackCanCastleKingSide;
        this->blackCanCastleQueenSide = board.blackCanCastleQueenSide;

        this->isWhiteToMove = board.isWhiteToMove;
        this->whiteKingIndex = board.whiteKingIndex;
        this->blackKingIndex = board.blackKingIndex;
        this->enPassantCapture = board.enPassantCapture;
    } 

    Board(std::string fenString) {
        init(fenString);
    }

    const std::string& getBoard() const {
        return _board;
    }

    bool getWhiteCanCastleKingSide() const {
        return whiteCanCastleKingSide;
    }

    bool getWhiteCanCastleQueenSide() const {
        return whiteCanCastleQueenSide;
    }

    bool getBlackCanCastleKingSide() const {
        return blackCanCastleKingSide;
    }

    bool getBlackCanCastleQueenSide() const {
        return blackCanCastleQueenSide;
    }

    int getEnPassantCapture() const {
        return enPassantCapture;
    }

    int getKingIndex(bool isWhite) const {
        return isWhite ? whiteKingIndex : blackKingIndex;
    }

    char getPieceAt(int index) const {
        return _board[index];
    }

    bool isWhiteTurn() const {
        return isWhiteToMove;
    }

    void makeMove(const Move &move);
    //void undoMove(const Move &move);

    void printBoard() {
        for (int row = 7; row >= 0; row--) {
            printf("%d ", row + 1);
            for (int col = 0; col < 8; col++) {
                printf("%c ", _board[col + row*ncol]);
            }
            printf("\n");
        }
        printf("  a b c d e f g h\n");
    }

private:
    void parsePiecePlacement(const std::string &placement);
    void parseActiveColor(const std::string &color);
    void parseCastlingAvailability(const std::string &castlingAvailability);
    void parseEnPassantTargetSquare(const std::string &enPassantTargetSquare);
    void init(std::string fenString);
    
    std::string _board;
    bool isWhiteToMove = true;

    bool whiteCanCastleKingSide = true, whiteCanCastleQueenSide = true, 
         blackCanCastleKingSide = true, blackCanCastleQueenSide = true;

    int whiteKingIndex = 4, blackKingIndex = 60;
    int enPassantCapture = -1;
};

#endif //BOARD_H
