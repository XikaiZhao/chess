#ifndef MCTREESEARCH_H
#define MCTREESEARCH_H

#include <memory>
#include <vector>
#include <random>
#include "Timer.h"
//#include "MCTSNodeState.h"

// Monte Carlo tree search
template <typename MoveType, typename NodeStateType>
class MCTreeSearch {
  public:
    MCTreeSearch() : _time_limits(0){};
    MCTreeSearch(float time_limits) : _time_limits(time_limits) {};

    ~MCTreeSearch() {
      if (_root != nullptr) {
        for (size_t i = 0; i < _root->children.size(); i++) { 
          if (_root->children[i] != nullptr) {
            delete _root->children[i];  
            _root->children[i] = nullptr;
          }
        }
      }
      std::vector<Node*>().swap(_root->children);
      if (_root != nullptr) {
        delete _root;
        _root = nullptr;
      }
    }
    
    MoveType search(const NodeStateType& state);

  private:
    class Node
    {
      public:
        Node* parent;
        std::vector<Node*> children;

        NodeStateType state; // every node has its own copy of the state
        
        MoveType parent_move; // last move made to get to current state
        double score = 0.0; 
        int num_visited = 0;
        bool fully_expanded = false;
        int num_children_visited = 0;

        Node(MoveType parent_move, Node* root) : parent(root), parent_move(parent_move) {
          if (root != nullptr) {
            state = root->state;
            state.makeMove(parent_move);
            children.resize(state.getPossibleMovesCount(), nullptr);
            num_children_visited = 0;
          }
          else {
            throw std::runtime_error("Root node must be initialized with a state");
          }
        };

        Node(const NodeStateType& state_) : parent(nullptr) {
            state = state_;
            children.resize(state.getPossibleMovesCount(), nullptr);
            num_children_visited = 0;
        };

        ~Node() {
          parent = nullptr;
          for(size_t i = 0; i < children.size(); i++) {
            if (children[i] != nullptr) {
              delete children[i];
              children[i] = nullptr;
            }
          }
          std::vector<Node*>().swap(children);
        }
    };

    MoveType getBestMove(Node* node);
    Node* getBestChild(Node* node);

    Node* select(Node* node);
    float simulate(Node* node);
    void backpropagate(Node* node, float score);

    void printRoute(Node *node_);

    Node* _root;

    Timer _timer;
    float _time_limits; // time limit for each search
    size_t _num_simulations;

    std::random_device device;
    std::mt19937 _rng = std::mt19937(device());
    
    bool _print = true;
};

#endif //MCTS_H
