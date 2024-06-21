#include "NNUE.h"
#include <cstring>
#include <stdexcept>

std::map<char, int> NNUE::piece_to_idx = {{'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'p', 5}, {'n', 6}, {'b', 7}, {'r', 8}, {'q', 9}};

float NNUE::computeEmbedding(float* input, int index) {
    float* embedding = nnueModel->getEmbedding();
    for (int i = 0; i < EMBEDDING_COLS-1; i++) {
        input[i] += embedding[i + index * EMBEDDING_COLS];
    }

    return embedding[EMBEDDING_COLS-1 + index * EMBEDDING_COLS];
}

void NNUE::computeLayer(const float* input, const float* layer, const float* bias, int row_count, int col_count, float* output) {
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            float inputVal = input[j];
            inputVal = inputVal < -1 ? -1 : (inputVal > 1 ? 1 : inputVal);
            output[i] += inputVal * layer[j + i * col_count];
        }
        output[i] += bias[i];
    }
}

float NNUE::evaluate(const std::string& boardString, int whiteKingIndex, int blackKingIndex, bool isWhiteTurn) {
    // compute input from embedding matrix
    // halfkp_idx = piece_index + (piece_to_idx + king_index * 10) * 64
    // change board string to fenString order 
    whiteKingIndex = whiteKingIndex%8 + 8*(7-whiteKingIndex/8); 
    blackKingIndex = blackKingIndex%8 + 8*(7-blackKingIndex/8);
    memset(embeddingOutput, 0, LAYER1_COLS * sizeof(float));
    float pqst = 0.0f;
    int whiteOffset = isWhiteTurn ? 0 : EMBEDDING_COLS-1;
    int blackOffset = isWhiteTurn ? EMBEDDING_COLS-1 : 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char c = boardString[j+8*i];
            if (c != '-' && c != 'K' && c != 'k') {
                if (c >= 'A' && c <= 'Z') {
                    int ind = j+8*(7-i) + 64 * (10 * whiteKingIndex + piece_to_idx[c]);
                    pqst += computeEmbedding(embeddingOutput + whiteOffset, ind);
                } else if (c >= 'a' && c <= 'z') {
                    int ind = j+8*(7-i) + 64 * (10 * blackKingIndex + piece_to_idx[c]);
                    pqst -= computeEmbedding(embeddingOutput + blackOffset, ind);
                }
                else {
                    throw std::runtime_error("Invalid character in board string");
                }
            }
        }
    }

    // compute layer1
    memset(layer1Output, 0, LAYER1_ROWS * sizeof(float));
    computeLayer(embeddingOutput, nnueModel->getLayer1(), nnueModel->getLayer1Bias(), LAYER1_ROWS, LAYER1_COLS, layer1Output);

    // compute layer2
    memset(layer2Output, 0, LAYER2_ROWS * sizeof(float));
    computeLayer(layer1Output, nnueModel->getLayer2(), nnueModel->getLayer2Bias(), LAYER2_ROWS, LAYER2_COLS, layer2Output);

    // compute layer3
    memset(layer3Output, 0, LAYER3_ROWS * sizeof(float));
    computeLayer(layer2Output, nnueModel->getLayer3(), nnueModel->getLayer3Bias(), LAYER3_ROWS, LAYER3_COLS, layer3Output);

    return isWhiteTurn ? layer3Output[0]+pqst/2.0f : -layer3Output[0]+pqst/2.0f;
}