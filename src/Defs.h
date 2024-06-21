#pragma once

#include <string>
#include <random>
#include <assert.h>

#define NDEBUG

#define nrow 8
#define ncol 8
#define nele 64

enum GameStatus {
    NONE = 0,
    CHECKMATE = 1,
    DRAW = 2
};

static inline bool isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

static inline bool islower(char c) {
    return c >= 'a' && c <= 'z';
}

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

static inline int generateRandomInt(int min, int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); // distribution in range [min, max]

    return dist6(rng);
}

struct Move
{
    int curPos = -1;
    int newPos = -1;
    char newPieceType = '-';

    Move() {};
    Move(int curPos, int newPos, char newPieceType = '-') : curPos(curPos), newPos(newPos), newPieceType(newPieceType) {}
    Move(std::string s) {
        assert(s.size() == 4 || s.size() == 5);
        curPos = stringToPos(s.substr(0, 2));
        newPos = stringToPos(s.substr(2, 2));
        newPieceType = '-';
        if (s.size() == 5) 
            newPieceType = s[4];
    }

    // define a print function
    std::string toString() const {
        //std::string s = std::to_string(curPos) + "->" + std::to_string(newPos) + " ";
        std::string s = posToString(curPos) + posToString(newPos);
        if (newPieceType != '-') 
            s += newPieceType;  

        return s;
    }
};

