#pragma once

#include <iostream>
#include <mutex>
#include <vector>
#include <faiss/Index.h>
#include "utils/config.h"
#include "utils/utils.h"

class VectorSearch {

public:
    static VectorSearch &getInstance() {
        static VectorSearch vectorSearch;
        return vectorSearch;
    }

    void createIndex();

    bool loadIndex();

    void saveIndex();

    void resetIndex();

    size_t removeIndex(const std::vector<int64_t> &ids);

    void addFeatures(const std::vector<int64_t> &ids, float *features);

    SearchResult search(float *feature, int query_number);


private:

    VectorSearch();

    ~VectorSearch() = default;

    VectorSearch(const VectorSearch &);

    VectorSearch &operator=(const VectorSearch &);

private:

    faiss::Index *index_;

    std::vector<int64_t> I_;

    std::vector<float> D_;
};
