#include "Player.h"

template <bool isWhite>
void Player<isWhite>::getLegalMoves(std::vector<Move >& legalMoves) {
    int pieceIndexKingChecked = -1;
    
    int numChecks = _king -> isChecked(_board, &pieceIndexKingChecked);
    //std::cout << "numChecks: " << numChecks << ", pieceIndexKingChecked: " << pieceIndexKingChecked << std::endl;

    _king -> updatePinnedPieces(_board);
    
    //std::cout << "numLegalMoves: " << legalMoves.size() << std::endl;
    _king -> getLegalMoves(_board, legalMoves, pieceIndexKingChecked);
    //std::cout << "numLegalMoves: " << legalMoves.size() << std::endl;
    
    if (numChecks <= 1) {
        for (const auto& p : _pieces) {
            //std::cout << *p << std::endl;
            p->getLegalMoves(_board, legalMoves, pieceIndexKingChecked);
            //std::cout << "numLegalMoves: " << legalMoves.size() << std::endl;
            //for (const auto& m : legalMoves) {
            //    std::cout << m.toString() << std::endl;
            //}
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
void Player<isWhite>::makeMove(const Move& move) {
    _board->makeMove<isWhite>(move);
};

///////////////////////////////////////////////////////////////////////////// 
template <bool isWhite>
void HumanPlayer<isWhite>::makeMove() {
    std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    
    std::vector<Move> legalMoves;
    getLegalMoves(legalMoves);

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

        const Piece* p = _board->getPiece(curPosInt);
        if (p != nullptr && _board->getPiece(curPosInt)->_type == PieceType::PAWN && (newPosInt/ncol == (isWhite ? 7 : 0))) {
            std::cout << "Pawn promotion, enter new piece type (queen, rook, knight, bishop): ";
            std::string newPieceType;
            std::cin >> newPieceType;

            if (newPieceType == "queen") {
                move.newPieceType = PieceType::QUEEN;
            }
            else if (newPieceType == "rook") {
                move.newPieceType = PieceType::ROOK;
            }
            else if (newPieceType == "knight") {
                move.newPieceType = PieceType::KNIGHT;
            }
            else if (newPieceType == "bishop") {
                move.newPieceType = PieceType::BISHOP;
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

    Player<isWhite>::makeMove(move);
    std::cout << "Move made: " << move.toString() << std::endl;
};

///////////////////////////////////////////////////////////////////////////// 
#include <random>

template <bool isWhite>
void RandomPlayer<isWhite>::makeMove() {
    std::cout << (isWhite ? "White" : "Black") << " player's turn." << std::endl;
    
    std::vector<Move> legalMoves;
    getLegalMoves(legalMoves);

    Move move = legalMoves[generateRandomInt(0, legalMoves.size()-1)];


    Player<isWhite>::makeMove(move);
    std::cout << "Move made: " << move.toString() << std::endl;
};

template void Player<true>::getLegalMoves(std::vector<Move >& legalMoves);
template void Player<false>::getLegalMoves(std::vector<Move >& legalMoves);
template void HumanPlayer<true>::getLegalMovesWrapper(std::vector<Move >& legalMoves);
template void HumanPlayer<false>::getLegalMovesWrapper(std::vector<Move >& legalMoves);
template void HumanPlayer<true>::makeMove(); 
template void HumanPlayer<false>::makeMove();
template void RandomPlayer<true>::makeMove();
template void RandomPlayer<false>::makeMove();