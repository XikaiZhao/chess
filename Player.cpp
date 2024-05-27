#include "Player.h"
#include "Piece.h"
#include "King.h"

template <bool isWhite>
void Player<isWhite>::getLegalMoves(const Board& board, std::vector<Move >& legalMoves) {
    int pieceIndexKingChecked = -1;
    
    King<isWhite> king(board.getKingIndex(isWhite));
    int numChecks = king.isChecked(board.getBoard(), &pieceIndexKingChecked);
    //std::cout << "numChecks: " << numChecks << std::endl;
    king.getLegalMoves(board, legalMoves, pieceIndexKingChecked);

    if (numChecks <= 1) {
        std::vector<int64_t> pinnedPieces(4, 0);
        king.getPinnedPieces(board.getBoard(), pinnedPieces);

        for (int i = 0; i < nele; i++) {
            const char p = board.getPieceAt(i);
            if (p != '-' && isupper(p) == isWhite && p != (isWhite? 'K' : 'k')) {
                getLegalMovesFactory<isWhite>(i, p, board, pinnedPieces, legalMoves, pieceIndexKingChecked);
            }
        }
    }

    /*
    std::cout << "numLegalMoves: " << legalMoves.size() << std::endl;
    for (const auto& m : legalMoves) {
        std::cout << m.toString() << " ";
    }
    std::cout << std::endl;
    */
};

template <bool isWhite>
void Player<isWhite>::makeMove(Board& board, const Move& move) {
    board.makeMove(move);
};

///////////////////////////////////////////////////////////////////////////// 
template <bool isWhite>
void HumanPlayer<isWhite>::makeMove(Board& board) {
    std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    
    std::vector<Move> legalMoves;
    getLegalMoves(board, legalMoves);

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
};

///////////////////////////////////////////////////////////////////////////// 
#include <random>

template <bool isWhite>
void RandomPlayer<isWhite>::makeMove(Board& board) {
    std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    
    std::vector<Move> legalMoves;
    getLegalMoves(board, legalMoves);

    Move move = legalMoves[generateRandomInt(0, legalMoves.size()-1)];


    Player<isWhite>::makeMove(board, move);
    std::cout << "Move made: " << move.toString() << std::endl;
};

/////////////////////////////////////////////////////////////////////////////
template void Player<true>::getLegalMoves(const Board& board, std::vector<Move >& legalMoves);
template void Player<false>::getLegalMoves(const Board& board, std::vector<Move >& legalMoves);
template void HumanPlayer<true>::makeMove(Board& Board);
template void HumanPlayer<false>::makeMove(Board& Board);
template void HumanPlayer<true>::getLegalMovesWrapper(const Board& board, std::vector<Move>& legalMoves);
template void HumanPlayer<false>::getLegalMovesWrapper(const Board& board, std::vector<Move>& legalMoves);
template void RandomPlayer<true>::makeMove(Board& Board);
template void RandomPlayer<false>::makeMove(Board& Board);
template void RandomPlayer<true>::getLegalMovesWrapper(const Board& board, std::vector<Move>& legalMoves);
template void RandomPlayer<false>::getLegalMovesWrapper(const Board& board, std::vector<Move>& legalMoves);
