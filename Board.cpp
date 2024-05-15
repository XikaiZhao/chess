#include "Board.h"
#include <cmath>
#include <stdexcept>

template<bool isWhite>
void Board::makeMove(const Move& move) {
    lastMovePieceChange = board[move.newPos];
    lastMovePieceChange->_onBoard = false;

    lastMoveFromInitPos = board[move.curPos]->_atInitialPos;
    board[move.curPos]->_atInitialPos = false;

    int c = move.newPos%ncol;
    int r = move.newPos/ncol;

    if (move.newPieceType != NONE) {
        int id = board[move.curPos]->_id;
        board[move.newPos] = createPiece<isWhite>(move.newPieceType, r, c, id);
        board[move.newPos]->_atInitialPos = false;
        board[move.curPos] = nullptr;

        std::vector<Piece*>& pieces = (isWhite) ? whitePieces : blackPieces;
        delete pieces[id];
        pieces[id] = board[move.newPos];
    }
    else {
        // en passant
        if (board[move.curPos]->_type == PAWN && std::abs(move.curPos - move.newPos) != ncol && board[move.newPos] == nullptr) {
            lastMovePieceChange = board[lastMove.newPos];
            lastMovePieceChange->_onBoard = false;
            board[lastMove.newPos] = nullptr;     
        }
        board[move.newPos] = board[move.curPos]; // move piece to new cell
        board[move.newPos]->updatePosition(move.newPos); // reset piece position
        board[move.curPos] = nullptr; // remove current piece on the old cell
        
        // castling (move rook)
        if (board[move.newPos]->_type == KING && std::abs(move.curPos%ncol - c) == 2) {
            int rook_c = (ncol - c -1 < c) ? ncol-1 : 0;
            int ind = rook_c + r*ncol;
            int ind_new = (ncol - c -1 < c) ?  move.newPos - 1 : move.newPos + 1;
            board[ind_new] = board[ind];
            board[ind_new]->updatePosition(ind_new);
            board[ind_new]->_atInitialPos = false;
            board[ind] = nullptr;     
        }
    }

    lastMove2 = lastMove;
    lastMove = move;
}

template<bool isWhite>
void Board::undoLastMove() {
    if (lastMove.newPos == -1) {
        throw std::runtime_error("No moves to undo"); // call updatePosition first
    }

    lastMovePieceChange->_onBoard = true;
    
    int c = lastMove.newPos%ncol;
    int r = lastMove.newPos/ncol;

    if (lastMove.newPieceType != NONE) {
        int id = board[lastMove.newPos]->_id;
        board[lastMove.newPos] = lastMovePieceChange;
        board[lastMove.curPos] = createPiece<isWhite>(PieceType::PAWN, lastMove.curPos/ncol, lastMove.curPos%ncol, id);
        board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;
        
        std::vector<Piece*>& pieces = (isWhite) ? whitePieces : blackPieces;
        delete pieces[id];
        pieces[id] = board[lastMove.curPos];
    }
    else {
        // en passant
        if (board[lastMove.curPos]->_type == PAWN && std::abs(lastMove.curPos - lastMove.newPos) != ncol && lastMovePieceChange->getPosition() != lastMove.newPos) {
            board[lastMovePieceChange->getPosition()] = lastMovePieceChange;   
            board[lastMove.curPos] = board[lastMove.newPos];
            board[lastMove.curPos]->updatePosition(lastMove.curPos);  
            board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;
            board[lastMove.newPos] = nullptr;
        }
        else {
            board[lastMove.curPos] = board[lastMove.newPos];
            board[lastMove.curPos]->updatePosition(lastMove.curPos);
            board[lastMove.newPos] = lastMovePieceChange;
            board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;

            // castling (move rook)
            if (board[lastMove.newPos]->_type == KING && std::abs(lastMove.curPos%ncol - c) == 2) {
                int rook_c = (ncol - c -1 < c) ? ncol-1 : 0;
                int ind = rook_c + r*ncol;
                int ind_new = (ncol - c -1 < c) ?  lastMove.newPos - 1 : lastMove.newPos + 1;
                board[ind] = board[ind_new]; 
                board[ind]->updatePosition(ind);
                board[ind]->_atInitialPos = true;
                board[ind_new] = nullptr;   
            }
        }
    }

    lastMove = lastMove2;
}
