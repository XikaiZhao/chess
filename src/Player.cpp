#include "Player.h"
#include "Piece.h"
#include "King.h"
#include "BoardWrapper.h"

template <bool isWhite>
void Player<isWhite>::makeMove(Board& board, const Move& move) {
    board.makeMove(move);
};

///////////////////////////////////////////////////////////////////////////// 
template <bool isWhite>
Move HumanPlayer<isWhite>::makeMove(Board& board, const std::vector<Move>& legalMoves) {
    if (legalMoves.size() == 0) 
        throw std::runtime_error("No legal moves available.");

    std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    
    Move move;
    bool validMove = false;
    while (!validMove) {
        std::string curPos, newPos;
        std::cout << "Enter move: from ";
        std::cin >> curPos;
        std::cout << " to ";
        std::cin >> newPos;
        if (curPos.size() != 2 || newPos.size() != 2) {
            std::cout << "Invalid input. Try again." << std::endl;
            continue;
        }

        int curPosInt = stringToPos(curPos);
        int newPosInt = stringToPos(newPos);
        if (curPosInt == -1 || newPosInt == -1) {
            std::cout << "Invalid input. Try again." << std::endl;
            continue;
        }
        move.curPos = curPosInt, move.newPos = newPosInt;

        const char p = board.getPieceAt(curPosInt); 
        if (p == '-' && std::tolower(p) == 'p' && (newPosInt/ncol == (isWhite ? 7 : 0))) {
            std::cout << "Pawn promotion, enter new piece type (queen, rook, knight, bishop): ";
            std::string newPieceType;
            std::cin >> newPieceType;

            if (newPieceType == "queen") {
                move.newPieceType = 'q';
            }
            else if (newPieceType == "rook") {
                move.newPieceType = 'r';
            }
            else if (newPieceType == "knight") {
                move.newPieceType = 'n';
            }
            else if (newPieceType == "bishop") {
                move.newPieceType = 'b';
            }
            else {
                std::cout << "Invalid input. Try again." << std::endl;
                continue;
            }
            
        }

        std::cout << "Entered move: " << move.toString() << std::endl;
        for (const auto& m : legalMoves) {
            if (m.curPos == move.curPos && m.newPos == move.newPos && m.newPieceType == move.newPieceType) {
                validMove = true;
                break;
            }
        }
        if (!validMove)
            std::cout << "Invalid move. Try again." << std::endl;
    }

    Player<isWhite>::makeMove(board, move);
    std::cout << "Move made: " << move.toString() << std::endl;
    return move;
};

///////////////////////////////////////////////////////////////////////////// 
#include <random>

template <bool isWhite>
Move RandomPlayer<isWhite>::makeMove(Board& board, const std::vector<Move>& legalMoves) {
    if (legalMoves.size() == 0) 
        throw std::runtime_error("No legal moves available.");

    std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    Move move = legalMoves[generateRandomInt(0, legalMoves.size()-1)];
    Player<isWhite>::makeMove(board, move);
    std::cout << "Move made: " << move.toString() << std::endl;
    return move;
};

/////////////////////////////////////////////////////////////////////////////
#include "GameMCTSNodeState.h"
#include "mcts/MCTS.h"
#ifdef USE_NNUE
#include "nnue/NNUE.h"
#endif

template <bool isWhite>
Move AIPlayer<isWhite>::makeMove(Board& board, const std::vector<Move>& legalMoves) {
    if (legalMoves.size() == 0) 
        throw std::runtime_error("No legal moves available.");

    //std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    
    GameMCTSNodeState state(board, legalMoves);
    MCTreeSearch<Move, GameMCTSNodeState> mcts(_timeLimit);
#ifdef USE_NNUE
    NNUE nnue;
    state.setNNUE(&nnue);
 #endif   
    Move move = mcts.search(state);
    Player<isWhite>::makeMove(board, move);
    //std::cout << "Move made: " << move.toString() << std::endl;
    return move;
}

/////////////////////////////////////////////////////////////////////////////
template Move HumanPlayer<true>::makeMove(Board& Board, const std::vector<Move>& legalMoves);
template Move HumanPlayer<false>::makeMove(Board& Board, const std::vector<Move>& legalMoves);
template Move RandomPlayer<true>::makeMove(Board& Board, const std::vector<Move>& legalMoves);
template Move RandomPlayer<false>::makeMove(Board& Board, const std::vector<Move>& legalMoves);
template Move AIPlayer<true>::makeMove(Board& Board, const std::vector<Move>& legalMoves);
template Move AIPlayer<false>::makeMove(Board& Board, const std::vector<Move>& legalMoves);

