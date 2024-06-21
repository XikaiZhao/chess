#ifndef BOARD_H
#define BOARD_H

#include <sstream>
#include <iostream>
#include <memory>
#include "Defs.h"

class Board {
public:
    Board() {
        //setInitialPosition();
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

    void setInitialPosition() {
        _board = "RNBQKBNRPPPPPPPP--------------------------------pppppppprnbqkbnr";
        isWhiteToMove = true;
        whiteCanCastleKingSide = true;
        whiteCanCastleQueenSide = true;
        blackCanCastleKingSide = true;
        blackCanCastleQueenSide = true;
        whiteKingIndex = 4;
        blackKingIndex = 60;
        enPassantCapture = -1;
    }

    void setBoard(std::string fenString) {
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

    void printBoard() const {
        for (int row = 7; row >= 0; row--) {
            printf("%d ", row + 1);
            for (int col = 0; col < 8; col++) {
                printf("%c ", _board[col + row*ncol]);
            }
            printf("\n");
        }
        printf("  a b c d e f g h\n");
    }

    std::string toString() const {
        std::ostringstream oss;
        for (int row = 7; row >= 0; row--) {
            oss << row + 1 << " ";
            for (int col = 0; col < 8; col++) {
                oss << _board[col + row*ncol] << " ";
            }
            oss << "\n";
        }
        oss << "  a b c d e f g h\n";
        oss << "White to move: " << (isWhiteToMove ? "true" : "false") << "\n";
        oss << "White can castle kingside: " << (whiteCanCastleKingSide ? "true" : "false") << "\n";
        oss << "White can castle queenside: " << (whiteCanCastleQueenSide ? "true" : "false") << "\n";
        oss << "Black can castle kingside: " << (blackCanCastleKingSide ? "true" : "false") << "\n";
        oss << "Black can castle queenside: " << (blackCanCastleQueenSide ? "true" : "false") << "\n";
        oss << "En passant capture square: " << enPassantCapture << "\n";
        oss << "White king index: " << whiteKingIndex << "\n";
        oss << "Black king index: " << blackKingIndex;
        return oss.str();
    }

    std::string toFenString() const {
        std::string fenString;
        for (int row = 7; row >= 0; row--) {
            int emptyCount = 0;
            for (int col = 0; col < 8; col++) {
                char p = _board[col + row*ncol];
                if (p == '-') {
                    emptyCount++;
                }
                else {
                    if (emptyCount > 0) {
                        fenString += std::to_string(emptyCount);
                        emptyCount = 0;
                    }
                    fenString += p;
                }
            }
            if (emptyCount > 0) {
                fenString += std::to_string(emptyCount);
            }
            if (row > 0) {
                fenString += "/";
            }
        }
        fenString += isWhiteToMove ? " w " : " b ";
        fenString += whiteCanCastleKingSide ? "K" : "";
        fenString += whiteCanCastleQueenSide ? "Q" : ""; 
        fenString += blackCanCastleKingSide ? "k" : "";
        fenString += blackCanCastleQueenSide ? "q" : "";
        fenString += " ";
        if (enPassantCapture == -1) {
            fenString += "-";
        }
        else {
            int r = enPassantCapture / ncol;
            int c = enPassantCapture % ncol;
            if (isWhiteToMove) r++;
            else r--;
            fenString += (char)('a' + c);
            fenString += (char)('1' + r);
        }
        fenString += " 0 1";
        return fenString;
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
