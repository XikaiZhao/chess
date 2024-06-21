#include "Player.h"
#include "BoardWrapper.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono> 
#include <ctime>  
#include <cstdio>
#include <unistd.h>
#include "nnue/NNUEModel.h"

#define REDIRECT_STDOUT_TO_FILE 

#ifdef REDIRECT_STDOUT_TO_FILE
int redirect_stdout(std::string filename) {
    fflush(stdout); // Flush the output buffer
    int stdout_copy = dup(STDOUT_FILENO);
    if (!freopen(filename.c_str(), "a", stdout)) {
        std::cerr << "Failed to redirect stdout" << std::endl;
        close(stdout_copy);
        return 1;
    }
    return stdout_copy;
}
void restore_stdout(int stdout_copy) {
    fflush(stdout); // Flush the output buffer
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdout_copy);
    //fclose(stdout);  // Close the output file
}
#else
#define redirect_stdout(x) 0
#define restore_stdout(x)
#endif


GameStatus makeMove(Board& board, Player<true>* whitePlayer, Player<false>* blackPlayer) {
    std::vector<Move> legalMoves;
    GameStatus gameStatus = BoardWrapper::getGameStatus(board, legalMoves);
    for (const auto& m : legalMoves) {
        std::cout << m.toString() << " ";
    }
    std::cout << std::endl;
    if (gameStatus == GameStatus::NONE) {
        if (board.isWhiteTurn()) whitePlayer->makeMove(board, legalMoves);
        else blackPlayer->makeMove(board, legalMoves);
    } 
    return gameStatus;
};

bool handleUCICommand(const std::string& command, Board& board, std::string outFileName) {
    std::ofstream file(outFileName, std::ios::app);
    file << std::endl;
    file << "Command: " << command << std::endl;
    file.close();

    std::istringstream iss(command);
    std::string token;
    iss >> token;  // Get the first word of the command

    if (token == "uci") {
        std::cout << "id name MyChessEngine\n";
        std::cout << "id author MyName\n";
        std::cout << "uciok\n";
    } else if (token == "isready") {
#ifdef USE_NNUE
        int stdout_copy = redirect_stdout(outFileName);
        NNUEModel::getInstance();
        restore_stdout(stdout_copy);
#endif
        std::cout << "readyok\n";
    } else if (token == "position") {
        std::string positionCmd;
        iss >> positionCmd;
        if (positionCmd == "startpos") {
            board.setInitialPosition(); // Initialize the board with the starting position
            std::string moveCmd;
            iss >> moveCmd;
            if (moveCmd == "moves") {
                std::string move;
                while (iss >> move) { 
                    board.makeMove(Move{move});
                } 
            }
        } else if (positionCmd == "fen") {
            // Initialize the board with the given FEN string
            std::string fenString;
            iss >> fenString;
            board.setBoard(fenString);
        }
        else 
            throw std::runtime_error("Invalid position command");
    } else if (token == "go") {
        // Handle the engine's move calculation and output best move
        //std::streambuf* coutbuf = std::cout.rdbuf(); // Backup the buffer of std::cout
        //std::cout.rdbuf(outFile.rdbuf()); // Redirect std::cout to the output file
        int stdout_copy = redirect_stdout(outFileName);
        Move bestMove;
        std::vector<Move> legalMoves;
        GameStatus gameStatus = BoardWrapper::getGameStatus(board, legalMoves);
        printf("Legal moves: ");
        for (const auto& m : legalMoves) 
            printf("%s ", m.toString().data());
        printf("\n");

        if (gameStatus == GameStatus::NONE) {
            if (board.isWhiteTurn()) {
                AIPlayer<true> whitePlayer(5.0f);
                bestMove = whitePlayer.makeMove(board, legalMoves);
            } else {
                AIPlayer<false> blackPlayer(5.0f);
                bestMove = blackPlayer.makeMove(board, legalMoves);
            }
        }
        restore_stdout(stdout_copy);
        //std::cout.rdbuf(coutbuf); // Restore std::cout's original buffer
        std::cout << "bestmove " << bestMove.toString() << std::endl;
    } else if (token == "quit") {
        return false;
    }
    return true;
}

int main()
{
#if 0
    Board board("r1br2k1/ppp2ppp/5n2/8/Pn2pP2/1NN5/1PP1PP1P/R3KB1R w KQ - 3 12");
    board.printBoard();
    AIPlayer<true> whitePlayer(5.0f);
    AIPlayer<false> blackPlayer(5.0f);
    makeMove(board, &whitePlayer, &blackPlayer);

#else
    // Redirect stdout to a file

    auto now = std::chrono::system_clock::now();   // Get current system time as time_point
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);  // Convert to time_t
    std::tm now_tm = *std::localtime(&now_c); // Convert to tm (local time)
    
    // Use stringstream to format the filename
    std::ostringstream filename;
    filename << "output_"
             << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S")  // Format: YYYY-MM-DD_HH-MM-SS
             << ".txt";

    Board board;
    bool continueGame = true;
    while(continueGame) {
        std::string command;
        std::getline(std::cin, command);
        continueGame = handleUCICommand(command, board, filename.str());
    }
    fclose(stdout); // Close the output file

#endif

    return 0;
}