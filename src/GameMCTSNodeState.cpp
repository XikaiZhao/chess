#include "GameMCTSNodeState.h"
#include "BoardWrapper.h"

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
  return BoardWrapper::getScore(_board, _gameStatus);
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
  if (std::abs(score) > 400.0f) 
    score = (score > 0) ? score -5.0f : score + 5.0f;
  return score;
}


