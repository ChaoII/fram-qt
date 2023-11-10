#pragma once

#include <iostream>
#include <mutex>
#include <vector>
#include <faiss/Index.h>
#include "utils/config.h"
#include "utils/Utils.h"

class VectorSearch {

public:
    static VectorSearch &getInstance() {
        static VectorSearch vectorSearch;
        return vectorSearch;
    }

    void create_index();

    bool load_index();

    void save_index();

    void reset_index();

    size_t remove_index(const std::vector<int64_t> &ids);

    void add_features(const std::vector<int64_t> &ids, float *features);

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
