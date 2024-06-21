#include "GameMCTSNodeState.h"
#include "BoardWrapper.h"
#include "nnue/NNUE.h"

void GameMCTSNodeState::makeMove(Move move)
{
  _board.makeMove(move);
  _parentMove = move;
  _legalMovesGenerated = false;
}

float GameMCTSNodeState::randomSimulate()
{
  if (!_legalMovesGenerated) 
    getPossibleMoves(); 

#ifdef USE_NNUE
  if (_gameStatus == GameStatus::CHECKMATE) 
    return (_board.isWhiteTurn()) ? -1.5f : 1.5f;
  else if (_gameStatus == GameStatus::DRAW) 
    return 0.0f;
  else {
    float score = _nnue->evaluate(_board.getBoard(), _board.getKingIndex(true), _board.getKingIndex(false), _board.isWhiteTurn());
    score = 1.0f/(1.0f + std::exp(-0.036f * score)); // sigmoid
    score = 2.0f * score - 1.0f; // scale to [-1, 1]
    return score;
  }
#else
  return BoardWrapper::getScore(_board, _gameStatus);
#endif
}

bool GameMCTSNodeState::isTerminal()
{
  if (!_legalMovesGenerated) 
    getPossibleMoves();
  return (_gameStatus > 0);
}

int GameMCTSNodeState::getCurrentPlayer()
{
  if (_board.isWhiteTurn()) return 1;
  else return -1;
}

const std::vector<Move>&  GameMCTSNodeState::getPossibleMoves()
{
  if (!_legalMovesGenerated) {
    std::vector<Move>().swap(_allValidMoves); // clear the vector
    if (_board.isWhiteTurn()) 
      _gameStatus = BoardWrapper::getGameStatus<true>(_board, _allValidMoves);
    else 
      _gameStatus = BoardWrapper::getGameStatus<false>(_board, _allValidMoves);
    _legalMovesGenerated = true;
  }
  return _allValidMoves;
}

int GameMCTSNodeState::getPossibleMovesCount()
{
  if (!_legalMovesGenerated) 
    getPossibleMoves();
  return _allValidMoves.size();
}

float GameMCTSNodeState::adjustScore(float score)
{
#ifdef USE_NNUE
  if (std::abs(score) > 1.0f) 
    score = (score > 0) ? score - 0.01f : score + 0.01f;
#else
  if (std::abs(score) > 400.0f) 
    score = (score > 0) ? score -5.0f : score + 5.0f;
#endif  
  return score;

}


