#include "vectorsearch.h"
#include <QFileInfo>
#include <faiss/index_io.h>
#include <faiss/index_factory.h>
#include <faiss/IndexIDMap.h>
#include <faiss/Index.h>
#include <faiss/IndexFlat.h>



void VectorSearch::create_index()
{
    auto index = faiss::index_factory(Config::getInstance()->getVector_size(), "Flat", faiss::METRIC_INNER_PRODUCT);
    index_ = new faiss::IndexIDMap(index);
    save_index();
}

void VectorSearch::save_index()
{
    faiss::write_index(index_, Config::getInstance()->getIndex_file().toStdString().c_str());
}

void VectorSearch::reset_index()
{
    index_->reset();
}

size_t VectorSearch::remove_index(const std::vector<int64_t> &ids)
{
    size_t num = index_->remove_ids(faiss::IDSelectorArray{ids.size(), ids.data()});
    save_index();
    return num;
}

bool VectorSearch::load_index()
{
    QFileInfo fileInfo(Config::getInstance()->getIndex_file().toStdString().c_str());
    if(!fileInfo.isFile()){
        qDebug()<<"index file is not existed";
        return false;
    }
    index_ = faiss::read_index(Config::getInstance()->getIndex_file().toStdString().c_str());
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
    D_.resize(Config::getInstance()->getTop_k() * query_number);
    I_.resize(Config::getInstance()->getTop_k() * query_number);
    index_->search(query_number, feature, Config::getInstance()->getTop_k(), D_.data(), I_.data());
    return SearchResult{I_, D_, Config::getInstance()->getTop_k()};
}

VectorSearch::VectorSearch()
{
    if(!load_index()){
        create_index();
    }
}
