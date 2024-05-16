#include "Player.h"

template <bool isWhite>
void Player<isWhite>::getLegalMoves(std::vector<Move >& legalMoves) {
    int pieceIndexKingChecked = -1;
    int numChecks = _king -> isChecked(_board, &pieceIndexKingChecked);
    _king -> isChecked(_board);
    _king -> updatedPinnedPieces(_board);
    _king -> getLegalMoves(_board, legalMoves);

    if (numChecks <= 1) {
        for (const auto& p : _pieces) {
            p->getLegalMoves(_board, legalMoves);
        }
    }
};

template <bool isWhite>
void Player<isWhite>::makeMove(const Move& move) {
    _board->makeMove<isWhite>(move);
};
