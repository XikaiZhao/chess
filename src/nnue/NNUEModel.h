#ifndef NNUEMODEL_H
#define NNUEMODEL_H

#include "Params.h"
#include <cstdio>
#include <chrono>
#include <stdexcept>

class NNUEModel {
public:
    static NNUEModel* getInstance() {
        static NNUEModel instance;
        return &instance;
    }

    float* getEmbedding() { return embedding; }
    float* getLayer1() { return layer1; }
    float* getLayer1Bias() { return layer1Bias; }
    float* getLayer2() { return layer2; }
    float* getLayer2Bias() { return layer2Bias; }
    float* getLayer3() { return layer3; }
    float* getLayer3Bias() { return layer3Bias; }

private:
    NNUEModel() { loadModel();}
    ~NNUEModel() {
        printf("Destructor called NNUEModel\n");
        if (embedding != nullptr) delete[] embedding;
        if (layer1 != nullptr) delete[] layer1;
        if (layer1Bias != nullptr) delete[] layer1Bias;
        if (layer2 != nullptr) delete[] layer2;
        if (layer2Bias != nullptr) delete[] layer2Bias;
        if (layer3 != nullptr) delete[] layer3;
        if (layer3Bias != nullptr) delete[] layer3Bias;
    }

    NNUEModel(NNUEModel const&) = delete; // Prevent copying
    NNUEModel& operator=(NNUEModel const&) = delete; // Prevent assignment

    void readFromFile(std::string filename, float* arr, int size) {
        printf("NNUEModel::Reading file %s\n", filename.c_str());
        FILE* fp = fopen(filename.c_str(), "rb");
        if (fp == nullptr) 
            throw std::runtime_error("Error opening file\n");
        int res = fread(arr, sizeof(float), size, fp);
        if (res != size) 
            throw std::runtime_error("Error reading file\n");
        fclose(fp);
    }

    void loadModel() {
        printf("NNUEModel::Loading model...\n");
        auto start = std::chrono::high_resolution_clock::now();
        embedding = new float[EMBEDDING_ROWS * EMBEDDING_COLS];
        layer1 = new float[LAYER1_ROWS * LAYER1_COLS];
        layer1Bias = new float[LAYER1_ROWS];
        layer2 = new float[LAYER2_ROWS * LAYER2_COLS];
        layer2Bias = new float[LAYER2_ROWS];
        layer3 = new float[LAYER3_ROWS * LAYER3_COLS];
        layer3Bias = new float[LAYER3_ROWS];

        // Load the model from the file
        readFromFile("../src/nnue/embedding.weight_torch.Size([40961, 1025]).bin", embedding, EMBEDDING_ROWS * EMBEDDING_COLS);
        readFromFile("../src/nnue/layer1.weight_torch.Size([8, 2048]).bin", layer1, LAYER1_ROWS * LAYER1_COLS);
        readFromFile("../src/nnue/layer1.bias_torch.Size([8]).bin", layer1Bias, LAYER1_ROWS);
        readFromFile("../src/nnue/layer2.weight_torch.Size([32, 8]).bin", layer2, LAYER2_ROWS * LAYER2_COLS);
        readFromFile("../src/nnue/layer2.bias_torch.Size([32]).bin", layer2Bias, LAYER2_ROWS);
        readFromFile("../src/nnue/layer3.weight_torch.Size([1, 32]).bin", layer3, LAYER3_ROWS * LAYER3_COLS);
        readFromFile("../src/nnue/layer3.bias_torch.Size([1]).bin", layer3Bias, LAYER3_ROWS);

        for (int i = 0; i < 10; i++) {
            printf("embedding[%d] = %f\n", i, layer2[i]);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        printf("NNUEModel::Model loaded, time elapsed = %f s\n", elapsed.count() / 1000.0);
    }

    float *embedding=nullptr;
    float *layer1=nullptr, *layer1Bias=nullptr;
    float *layer2=nullptr, *layer2Bias=nullptr;
    float *layer3=nullptr, *layer3Bias=nullptr;

};

#endif // NNUEModel_H