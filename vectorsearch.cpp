#include "vectorsearch.h"
#include <QFileInfo>
#include <faiss/index_io.h>
#include <faiss/index_factory.h>
#include <faiss/IndexIDMap.h>



void VectorSearch::create_index()
{
    auto index = faiss::index_factory(VECTOR_SIZE,"HNSW32",faiss::METRIC_INNER_PRODUCT);
    index_ = new faiss::IndexIDMap(index);
    save_index();
}

void VectorSearch::save_index()
{
    faiss::write_index(index_,INDEX_FILE);
}

void VectorSearch::reset_index()
{
    index_->reset();
}

bool VectorSearch::load_index()
{
    QFileInfo fileInfo(INDEX_FILE);
    if(!fileInfo.isFile()){
        qDebug()<<"index file is not existed";
        return false;
    }
    index_ = faiss::read_index(INDEX_FILE);
    return true;
}

void VectorSearch::add_features(const std::vector<int64_t> &ids, float *features)
{
    int feature_num = ids.size();
    index_->add_with_ids(feature_num, features, ids.data());
    save_index();
}

SearchResult VectorSearch::search(float *feature, int query_number)
{
    D_.resize(TOP_K * query_number);
    I_.resize(TOP_K * query_number);
    index_->search(query_number, feature, TOP_K, D_.data(), I_.data());
    return SearchResult{I_, D_, TOP_K};
}

VectorSearch::VectorSearch()
{
    if(!load_index()){
        create_index();
    }
}
