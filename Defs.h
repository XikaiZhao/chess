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

struct Move
{
    int curPos = -1;
    int newPos = -1;
    PieceType newPieceType = PieceType::NONE;
};