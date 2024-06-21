#include "MCTS.h"
#include <cmath>
#include <stdlib.h>

template<typename MoveType, typename NodeStateType>
MoveType MCTreeSearch<MoveType, NodeStateType>::search(const NodeStateType& state)
{
    _num_simulations = 0;
    _timer.start();
    _root = new Node(state);
    while (_timer.elapsedSeconds() < _time_limits) {
      Node* selected_node = select(_root);
      float score = simulate(selected_node);
      backpropagate(selected_node, score);
    }
    _timer.stop();
    printf("time elapsed %5.4fs, num_simulations in this search = %lu\n", _timer.elapsedMilliseconds()/1000, _num_simulations);
    
    if (_print) {
      std::cout << "Printing MCTS best route" << std::endl;
      printRoute(_root);
    }

    return getBestMove(_root);
}

template<typename MoveType, typename NodeStateType>
MoveType MCTreeSearch<MoveType, NodeStateType>::getBestMove(Node* node)
{
  int maxNumVisited = 0;
  MoveType bestMove;
  for(const auto& child : node->children) {
    //printf("parent_move = %d, num_visited = %d, score = %f\n", child->parent_move, child->num_visited, child->score);
    if (child->num_visited > maxNumVisited) {
      maxNumVisited = child->num_visited;
      bestMove = child->parent_move; 
    }
  }
  return bestMove;
}

template<typename MoveType, typename NodeStateType>
typename MCTreeSearch<MoveType, NodeStateType>::Node* MCTreeSearch<MoveType, NodeStateType>::getBestChild(Node* node)
{
  // Upper Confidence Bound
#if 1
  Node* child = node->children[0];
  int numBestChildren = 1;
  float maxScore = node->state.getCurrentPlayer()*child->score/child->num_visited 
              + 0.3 * sqrt(log(node->num_visited/child->num_visited));
  
  float currentPlayer = node->state.getCurrentPlayer();
  for (int i = 1; i < node->num_children_visited; i++) {
    child = node->children[i];
    float childScore = currentPlayer*child->score/child->num_visited 
              + 0.3 * sqrt(log(node->num_visited/child->num_visited));
    if (childScore > maxScore) {
      maxScore = childScore;
      numBestChildren = 1;
      std::swap(node->children[i], node->children[0]);
    }
    else if (childScore == maxScore) {
      std::swap(node->children[i], node->children[numBestChildren]);
      numBestChildren++;
    }
  }
  
  std::uniform_int_distribution<> distr(0, numBestChildren-1);
  return node->children[distr(_rng)];

#else
  float maxScore = -1e10;
  std::vector<int> bestChildrenIndex;
  bestChildrenIndex.reserve(node->num_children_visited);
  for (size_t i = 0; i < node->children.size(); i++) {
    Node* child = node->children[i];
    float childScore = node->state.getCurrentPlayer()*child->score/child->num_visited 
              + 2.0 * sqrt(log(node->num_visited/child->num_visited));
    if (childScore > maxScore) {
      maxScore = childScore;
      bestChildrenIndex.clear();
      bestChildrenIndex.push_back(i);
    }
    else if (childScore == maxScore) 
      bestChildrenIndex.push_back(i);
  }

  std::uniform_int_distribution<> distr(0, bestChildrenIndex.size()-1);
  return node->children[bestChildrenIndex[distr(_rng)]];
#endif
}

template<typename MoveType, typename NodeStateType>
typename MCTreeSearch<MoveType, NodeStateType>::Node* MCTreeSearch<MoveType, NodeStateType>::select(Node* node) 
{
  if (!node->state.isTerminal()) {
    if (node->fully_expanded) {
      Node* best_child = getBestChild(node);
      return select(best_child);
    }
    else {
      const std::vector<MoveType>& moves = node->state.getPossibleMoves();
      Node* new_child_node = new Node(moves[node->num_children_visited], node);
      node->children[node->num_children_visited++] = new_child_node;
      node->fully_expanded = (node->state.getPossibleMovesCount() == node->num_children_visited);
      return new_child_node;
    }
  }
  return node;
}

template<typename MoveType, typename NodeStateType>
float MCTreeSearch<MoveType, NodeStateType>::simulate(Node* node)
{
  _num_simulations++;
  return node->state.randomSimulate();
}

template<typename MoveType, typename NodeStateType>
void MCTreeSearch<MoveType, NodeStateType>::backpropagate(Node* node, float score)
{
  node->score += score; 
  node->num_visited++;
  if(node -> parent != nullptr) 
    backpropagate(node->parent, node->state.adjustScore(score));
}

template<typename MoveType, typename NodeStateType>
void MCTreeSearch<MoveType, NodeStateType>::printRoute(Node* node_) {
    int depth = 0;
    auto node = node_;
    while (node != nullptr) {
      Node* best_child = nullptr;
      int maxNumVisited = 0;
      //std::cout << "depth = " << depth << std::endl;

      for(const auto& child : node->children) {
        if (child == nullptr) break;;
    //printf("parent_move = %d, num_visited = %d, score = %f\n", child->parent_move, child->num_visited, child->score);
        if (child->num_visited > maxNumVisited) {
           maxNumVisited = child->num_visited;
           best_child = child; 
        }
      }
      if (best_child != nullptr) {
        printf("%d, parent_move = %s, num_visited = %d, score = %f, eval = %f\n", depth, best_child->parent_move.toString().data(), best_child->num_visited, best_child->score, best_child->score/best_child->num_visited  );
        //best_child->state.printState();
      }
      depth++;
      node = best_child;
    }
}
/////////////////////////////////////////////////////////////////////////
