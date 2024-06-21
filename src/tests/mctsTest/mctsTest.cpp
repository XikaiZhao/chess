#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "../../Board.h"
#include "../../BoardWrapper.h"
#include "../../Player.h"

// Test mcts for a given board position in FEN string
struct ChessPuzzle {
    std::string puzzleId;
    std::string fen;
    std::string moves;
    int rating;
    int ratingDeviation;
    int popularity;
    int nbPlays;
    std::string themes;
    std::string gameUrl;
    std::string openingTags;
};

void printPuzzle(const ChessPuzzle& puzzle) {
    std::cout << "Puzzle ID: " << puzzle.puzzleId << std::endl;
    std::cout << "FEN: " << puzzle.fen << std::endl;
    std::cout << "Moves: " << puzzle.moves << std::endl;
    std::cout << "Rating: " << puzzle.rating << std::endl;
    std::cout << "Rating Deviation: " << puzzle.ratingDeviation << std::endl;
    std::cout << "Popularity: " << puzzle.popularity << std::endl;
    std::cout << "Number of Plays: " << puzzle.nbPlays << std::endl;
    std::cout << "Themes: " << puzzle.themes << std::endl;
    std::cout << "Game URL: " << puzzle.gameUrl << std::endl;
    std::cout << "Opening Tags: " << puzzle.openingTags << std::endl;
}

void testMCTSMove(std::string fenString) {
    Board board(fenString);
    board.printBoard();

    std::vector<Move> legalMoves;
    if (board.isWhiteTurn()) {
        AIPlayer<true> whitePlayer(1.0f);
        GameStatus gameStatus = BoardWrapper::getGameStatus<true>(board, legalMoves);
        float score = BoardWrapper::getScore(board, gameStatus);
        std::cout << "Init position score: " << score << std::endl;
        if (gameStatus == 1.0f) {
            std::cout << "Black wins" << std::endl;
        } else if (gameStatus == 0.5f) {
            std::cout << "Draw" << std::endl;
        } else
            whitePlayer.makeMove(board, legalMoves);
    } else {
        AIPlayer<false> blackPlayer(1.0f);
        GameStatus gameStatus = BoardWrapper::getGameStatus<false>(board, legalMoves);
        float score = BoardWrapper::getScore(board, gameStatus);
        std::cout << "Init position score: " << score << std::endl;
        if (gameStatus == 1.0f) {
            std::cout << "Black wins" << std::endl;
        } else if (gameStatus == 0.5f) {
            std::cout << "Draw" << std::endl;
        } else
            blackPlayer.makeMove(board, legalMoves);
    }
}

int main(int argc, char** argv) {
#if 0
    if (argc != 2) {
        throw std::runtime_error("Usage: ./mctsTest <FENString>");
    }

    std::string inputFEN = argv[1];
    testMCTSMove(inputFEN);
#else

    if (argc < 2) {
        throw std::runtime_error("Usage: ./mctsTest <test csv file> <(optional)theme>");
    }
    std::string filename = argv[1];
    std::string theme = "not_used";
    if (argc == 3) {
        theme = argv[2];
    }

    std::ifstream file(filename.c_str()); // Open the CSV file
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line); // Skip the header line

    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string field;
        std::vector<std::string> fields;

        while (getline(s, field, ',')) {
            fields.push_back(field);
        }

        ChessPuzzle puzzle;
        puzzle.puzzleId = fields[0];
        puzzle.fen = fields[1];
        puzzle.moves = fields[2];
        puzzle.rating = std::stoi(fields[3]);
        puzzle.ratingDeviation = std::stoi(fields[4]);
        puzzle.popularity = std::stoi(fields[5]);
        puzzle.nbPlays = std::stoi(fields[6]);
        puzzle.themes = fields[7];
        puzzle.gameUrl = fields[8];
        puzzle.openingTags = fields[9];

        //printPuzzle(puzzle);

        if (theme != "not_used" && puzzle.themes.find(theme) == std::string::npos) {
            continue;
        }
       
        std::cout << "Testing MCTS for puzzle: " << line << std::endl;
        testMCTSMove(puzzle.fen);
        std::cout << "Read moves: " << puzzle.moves << std::endl;
    }

    file.close();

#endif

    return 0;
}
