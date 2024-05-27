#include "../Board.h"
#include "../Player.h"
#include "SimpleJSONReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// Test the legal moves for a given FEN string
// input jsonl file 
// file format: {"FEN": "r2qk2r/1ppb2p1/pnn1pbBp/3p4/3P1B1P/P1N2N2/1PPQ1PP1/2KR3R b kq - 1 13", "legal_moves": ["e8f8", "e8e7"], "legal_move_count": 2, "cp": 261}

void testLegalMove(std::string fenString, std::vector<Move>& legalMoves) {
    Board board(fenString);
    //board.printBoard();

    if (board.isWhiteTurn()) {
        std::cout << "White to move" << std::endl;
        Player<true> p;
        p.getLegalMoves(board, legalMoves);
    } else {
        std::cout << "Black to move" << std::endl;
        Player<false> p;
        p.getLegalMoves(board, legalMoves);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::runtime_error("Usage: ./legalMoveTests <filename>");
    }

    char* filename = argv[1];

    std::ifstream input( filename );
    if (!input.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    int count = 1;
    std::cout << "Starting test" << std::endl;

    std::string line; 
    while(getline( input, line ))
    {   
        std::cout << "Line " << count << std::endl;
        SimpleJSONReader reader(line);
        std::string FEN =  reader.getString("FEN");
        std::vector<std::string> moves = reader.getStringArray("legal_moves");
        std::sort(moves.begin(), moves.end());
        int  numMoves = reader.getInteger("legal_move_count");
        if (size_t(numMoves) != moves.size()) {
            std::cout << "Line " << count << ": numMoves: " << numMoves << ", moves.size(): " << moves.size() << std::endl;
            for (auto& m : moves) {
                std::cout << m << " ";
            }
            std::cout << std::endl;
            throw std::runtime_error("Number of moves does not match count");
        }

        std::vector<Move> legalMoves;
        testLegalMove(FEN, legalMoves);
        std::sort(legalMoves.begin(), legalMoves.end(), [](const Move& a, const Move& b) {
            return a.toString() < b.toString();
        });

        std::cout << "read    : ";
        for (auto& m : moves) {
            std::cout << m << " ";
        }
        std::cout << "\ncomputed: ";
        for (auto& m : legalMoves) {
            std::cout << m.toString() << " ";
        }  
        std::cout << std::endl;
        
        if (legalMoves.size() != size_t(numMoves)) {
            std::cout << "Line " << count << ": numMoves: " << numMoves << ", legalMoves.size(): " << legalMoves.size() << std::endl;
            //throw std::runtime_error("Computed number of legal moves does not match count");
        }
        for (int i = 0; i < numMoves; i++) {
            if (legalMoves[i].toString() != moves[i]) {
                std::cout << "Line " << count << ": computed: " << legalMoves[i].toString() << ", read " << moves[i] << std::endl; 
                throw std::runtime_error("Compute legal move does not match");
            }
        }
        count++;
    }

    return 0;
}