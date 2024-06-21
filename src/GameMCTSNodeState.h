#ifndef GAMEMCTSSTATE_H
#define GAMEMCTSSTATE_H

#include <memory>
#include "mcts/MCTSNodeState.h"
#include "Defs.h"
#include "Board.h"
#include "BoardWrapper.h"
#include "nnue/NNUE.h"

class GameMCTSNodeState : public MCTSNodeState<Move>
{
  public:
    GameMCTSNodeState() {}
    GameMCTSNodeState(const Board& board) : _board(board) {}
    GameMCTSNodeState(const Board&_board, const std::vector<Move>& allValidMoves) : _board(_board) {
      assert(allValidMoves.size() > 0);
      _allValidMoves = allValidMoves;
      _legalMovesGenerated = true;
      _gameStatus = GameStatus::NONE;
    }

    virtual void makeMove(Move move) override;
    virtual bool isTerminal() override;
    virtual int getCurrentPlayer() override;
    virtual float randomSimulate() override;
    virtual float adjustScore(float score) override;
    virtual const std::vector<Move>& getPossibleMoves() override ; 
    virtual int getPossibleMovesCount() override;
    virtual void printState() override {
      std::cout<< "GameMCTSNodeState: " << std::endl;
      _board.printBoard();
      std::cout << _board.getBoard() << std::endl;
      std::cout << "Game status: " << _gameStatus << std::endl;
    }

    GameMCTSNodeState& operator=(const GameMCTSNodeState& other) {
      _board = other._board;
      _gameStatus = GameStatus::NONE;
      _legalMovesGenerated = false;
#ifdef USE_NNUE
      _nnue = other._nnue;
#endif
      return *this;
    }

    const std::string& getBoard() const {
      return _board.getBoard();
    }

#ifdef USE_NNUE
    void setNNUE(NNUE* nnue) {
      _nnue = nnue;
    }
#endif

  private:
    Board _board;
    GameStatus _gameStatus = GameStatus::NONE;
    bool _legalMovesGenerated = false;

#ifdef USE_NNUE
    NNUE* _nnue = nullptr;
#endif
};

#endif //GAMEMCTSSTATE_H
