#pragma once

#define nrow 8
#define ncol 8
#define nele 64

#define PAWN_IND 0
#define ROOK_IND 8
#define KNIGHT_IND 10
#define BISHOP_IND 12
#define QUEEN_IND 14 

enum PieceType {NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

#include <string>

static inline std::string posToString(int pos) {
    int row = pos / ncol, col = pos % ncol;
    std::string s = std::string(1, 'a'+col) + std::to_string(row+1);
    return s;
}

static inline int stringToPos(const std::string& s) {
    if (s.size() != 2) return -1;
    int col = s[0] - 'a', row = s[1] - '1';
    if (row < 0 || row >= nrow || col < 0 || col >= ncol) 
        return -1;
    return row * ncol + col;
}
struct Move
{
    int curPos = -1;
    int newPos = -1;
    PieceType newPieceType = PieceType::NONE;

    // define a print function
    std::string toString() const {
        //std::string s = std::to_string(curPos) + "->" + std::to_string(newPos) + " ";
        std::string s = posToString(curPos) + "->" + posToString(newPos);
        if (newPieceType != PieceType::NONE) {
            s += " " + std::to_string(newPieceType);
        }
        return s;
    }
};
