#ifndef BOARDWRAPPER_H
#define BOARDWRAPPER_H
#include "Board.h"
#include "Piece.h"
#include "King.h"

/*
 * This is a wrapper class for the Board class. It provides a static method to get the game status.
*/

class BoardWrapper {
public:
    static GameStatus getGameStatus(const Board& board, std::vector<Move>& legalMoves) {
        if (board.isWhiteTurn()) {
            return getGameStatus<true>(board, legalMoves);
        } else {
            return getGameStatus<false>(board, legalMoves);
        }
    };
    
    template <bool isWhite> // isWhite is the current player
    static GameStatus getGameStatus(const Board& board, std::vector<Move>& legalMoves) {
        assert(board.isWhiteTurn() == isWhite);

        int pieceIndexKingChecked = -1;
        King<isWhite> king(board.getKingIndex(isWhite));
        int numChecks = king.isChecked(board.getBoard(), &pieceIndexKingChecked);
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

        if (legalMoves.size() == 0) {
            if (numChecks > 0) {
                //std::cout << (isWhite ? "White" : "Black") << " is in checkmate." << std::endl;
                return GameStatus::CHECKMATE;
            }
            else {
                //std::cout << "Stalemate." << std::endl;
                return GameStatus::DRAW;
            }
        }
        return GameStatus::NONE;
    };

    static float getScore (const Board& board, GameStatus gameStatus) {
        if (gameStatus == GameStatus::CHECKMATE) 
            return (board.isWhiteTurn()? -500.0f : 500.0f);
        else if (gameStatus == GameStatus::DRAW) 
            return 0.0f;
        else {
            const std::string& boardStr = board.getBoard();
            float score = 0.0f;
            for (int i = 0; i < nele; i++) {
                score += getPieceValue(boardStr[i]);
            }
            return score;
        }
    };

    static float getPieceValue(char type) {
        switch (type) {
            case 'P': return 1.0f;
            case 'p': return -1.0f;
            case 'N': return 3.0f;
            case 'n': return -3.0f;
            case 'B': return 3.5f;
            case 'b': return -3.5f;
            case 'R': return 5.0f;
            case 'r': return -5.0f;
            case 'Q': return 10.0f;
            case 'q': return -10.0f;
            default: return 0;
        }
    }
};

#endif //BOARDWRAPPER_H