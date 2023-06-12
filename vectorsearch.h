#ifndef VECTORSEARCH_H
#define VECTORSEARCH_H
#include <iostream>
#include <mutex>
#include <vector>
#include <faiss/Index.h>
#include "config.h"
#include "Utils.h"

class VectorSearch
{

public:
    static VectorSearch* getInstance(){
        if(pInstance == nullptr){
            std::lock_guard<std::mutex> lock(mutex_);
            if(pInstance == nullptr){
                pInstance = new VectorSearch();
            }
        }
        return pInstance;
    }

    void index_size();

    void create_index();

    bool load_index();

    void save_index();

    void reset_index();

    void add_features(const std::vector<int64_t> &ids, float* features);

    SearchResult search(float *feature, int query_number);


private:

    VectorSearch();

    VectorSearch(const VectorSearch &);

    VectorSearch& operator=(const VectorSearch &);

private:

    inline static VectorSearch* pInstance = nullptr;

    inline static std::mutex mutex_;

    faiss::Index *index_;

    std::vector<int64_t> I_;

    std::vector<float> D_;
};

#endif // VECTORSEARCH_H
