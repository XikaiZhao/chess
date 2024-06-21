#include "Board.h"
#include <cmath>
#include <stdexcept>
#include <sstream>

void Board::parsePiecePlacement(const std::string& placement) {
    std::cout << "parsePiecePlacement, placement = " << placement << std::endl;
    _board = std::string(ncol*nrow, '-');
    int r = 7, c = 0;
    for (size_t i = 0; i < placement.size(); i++) {
        char ch = placement[i];
        if (ch == '/') {
            r--;
            c = 0;
        }
        else if (std::isdigit(ch)) {
            c += ch - '0';
        }
        else {
            _board[c+r*ncol] = ch;
            if (ch == 'K') 
                whiteKingIndex = c+r*ncol;
            else if (ch == 'k') 
                blackKingIndex = c+r*ncol;
            c++;
        }
    }
    //printBoard();
}

void Board::parseActiveColor(const std::string& color) {
    if (color == "w") 
        isWhiteToMove = true;
    else if (color == "b") 
        isWhiteToMove = false;
    else 
        throw std::runtime_error("Invalid active color");
}

void Board::parseCastlingAvailability(const std::string& castlingAvailability) {
    std::cout << "parseCastlingAvailability, castlingAvailability = " << castlingAvailability << std::endl;
    whiteCanCastleKingSide = whiteCanCastleQueenSide = blackCanCastleKingSide = blackCanCastleQueenSide = false;
    for (char ch : castlingAvailability) {
        switch (ch) {
            case 'K': whiteCanCastleKingSide = true; break;
            case 'Q': whiteCanCastleQueenSide = true; break;
            case 'k': blackCanCastleKingSide = true; break;
            case 'q': blackCanCastleQueenSide = true; break;
            case '-': break;
            default: throw std::runtime_error("Invalid castling availability");
        }
    }
}

void Board::parseEnPassantTargetSquare(const std::string& enPassantTargetSquare){
    std::cout << "parseEnPassantTargetSquare, enPassantTargetSquare = " << enPassantTargetSquare << std::endl;
    if (enPassantTargetSquare == "-") {
        enPassantCapture = -1;
    }
    else {
        int c = enPassantTargetSquare[0] - 'a';
        int r = enPassantTargetSquare[1] - '1';
        enPassantCapture = isWhiteToMove ? c + r*ncol - ncol : c + r*ncol + ncol;
    }
}

void Board::init(std::string fenString) {
    std::istringstream iss(fenString);
    std::string part;
    std::vector<std::string> parts;
    
    while (getline(iss, part, ' ')) 
        parts.push_back(part);

    if (parts.size() != 6) 
        throw std::runtime_error("Invalid FEN string");
    
    parsePiecePlacement(parts[0]);
    parseActiveColor(parts[1]);
    parseCastlingAvailability(parts[2]);
    parseEnPassantTargetSquare(parts[3]);
}

void Board::makeMove(const Move& move) {
    char newPosPiece = _board[move.newPos];

    // update move
    _board[move.newPos] = _board[move.curPos];
    _board[move.curPos] = '-';

    if ( _board[move.newPos] == (isWhiteToMove ? 'P' : 'p')) {
        if ( newPosPiece == '-' && move.curPos%ncol != move.newPos%ncol) {
            // en passant
            _board[enPassantCapture] = '-';
            enPassantCapture = -1;
        }
        else if (std::abs(move.newPos/ncol - move.curPos/ncol) == 2) {
            enPassantCapture = move.newPos;
        }
        else if (move.newPieceType != '-') {
            // promotion
            _board[move.newPos] = isWhiteToMove? std::toupper(move.newPieceType) : move.newPieceType;
            enPassantCapture = -1;
        }
    }
    else if (_board[move.newPos] == (isWhiteToMove ? 'K' : 'k')) {
        enPassantCapture = -1;
        if (std::abs(move.newPos%ncol - move.curPos%ncol) == 2) {
            // castling, update rook position
            int rook_c = (move.newPos%ncol > move.curPos%ncol) ? ncol-1 : 0;
            int ind = rook_c + move.newPos/ncol*ncol;
            int ind_new = (rook_c == 0) ?  move.newPos + 1 : move.newPos - 1;
            _board[ind_new] = _board[ind]; 
            _board[ind] = '-';
        }
        if (isWhiteToMove) {
            whiteKingIndex = move.newPos;
            whiteCanCastleKingSide = whiteCanCastleQueenSide = false;
        }
        else {
            blackKingIndex = move.newPos; 
            blackCanCastleKingSide = blackCanCastleQueenSide = false;
        }
    } 
    else
        enPassantCapture = -1;

    // update castling
    if (whiteCanCastleQueenSide && (move.curPos == 0 || move.newPos == 0))
        whiteCanCastleQueenSide = false;
    else if (whiteCanCastleKingSide && (move.curPos == 7 || move.newPos == 7))
        whiteCanCastleKingSide = false;
    else if (blackCanCastleKingSide && (move.curPos == 63 || move.newPos == 63))
        blackCanCastleKingSide = false;
    else if (blackCanCastleQueenSide && (move.curPos == 56 || move.newPos == 56))
        blackCanCastleQueenSide = false;

    // update turn
    isWhiteToMove = !isWhiteToMove;
}

