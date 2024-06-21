#ifndef NNUE_H
#define NNUE_H

#include <map>
#include <string>
#include "NNUEModel.h"

class NNUE
{
public:
    NNUE() {
        nnueModel = NNUEModel::getInstance();
        embeddingOutput = new float[EMBEDDING_COLS * 2];
        layer1Output = new float[LAYER1_ROWS];
        layer2Output = new float[LAYER2_ROWS];
        layer3Output = new float[LAYER3_ROWS];
    }
    ~NNUE() {
        if (embeddingOutput != nullptr) delete[] embeddingOutput;
        if (layer1Output != nullptr) delete[] layer1Output;
        if (layer2Output != nullptr) delete[] layer2Output;
        if (layer3Output != nullptr) delete[] layer3Output;
    }
    
    // return the score for the current player, > 0 means white is winning, < 0 means black is winning
    // input boardstring has 64 characters, each character represents a square on the board, from a1 to h8
    float evaluate(const std::string& boardString, int whiteKingIndex, int blackKinIndex, bool isWhiteTurn);

private:
    static std::map<char, int> piece_to_idx;

    NNUEModel* nnueModel = nullptr;

    float *embeddingOutput = nullptr, *layer1Output = nullptr, *layer2Output = nullptr, *layer3Output = nullptr;

    // compute the input for the neural network from the embedding matrix and return pqst value
    float computeEmbedding(float *input, int index);

    void computeLayer(const float *input, const float *layer, const float *bias, int row_count, int col_count, float *output);
};



#endif // NNUE_H