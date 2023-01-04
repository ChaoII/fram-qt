#include "core/vectorsearch.h"
#include <cstdio>
#include <faiss/IndexIDMap.h>
#include <faiss/index_factory.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <sys/stat.h>
#include <unistd.h>

VectorSearch *VectorSearch::pInstance = nullptr;

VectorSearch::VectorSearch() {
    // IndexProcess
    if(!load_index_file()){
        build_index();
    };
    this->I_.resize(TOP_K * MAX_QUERY);
    this->D_.resize(TOP_K * MAX_QUERY);
}

QMutex VectorSearch::mutex_;

VectorSearch *VectorSearch::getInstance() {
    if (pInstance == nullptr) { // double check
        mutex_.lock();
        //用lock实现线程安全，用资源管理类，实现异常安全
        //使用资源管理类，在抛出异常的时候，资源管理类对象会被析构，析构总是发生的无论是因为异常抛出还是语句块结束。
        if (pInstance == nullptr) {
            pInstance = new VectorSearch();
        }
        mutex_.unlock();
    }
    return pInstance;
}


bool VectorSearch::load_index_file() {
    QFileInfo fileInfo(INDEX_FILE);
    if (!fileInfo.isFile()){
        qDebug("index file path is not existed!");
        return false;
    }
    this->index_ = faiss::read_index(INDEX_FILE, 0);
    return true;
}

// doing search
const SearchResult &VectorSearch::search(float *feature, int query_number) {
    this->D_.resize(TOP_K * query_number);
    this->I_.resize(TOP_K * query_number);
    this->index_->search(query_number, feature, TOP_K, D_.data(), I_.data());
    this->result_.return_k = TOP_K;
    this->result_.D = this->D_;
    this->result_.I = this->I_;
    return this->result_;
}

const int VectorSearch::get_index_length() { return this->index_->ntotal; }

void VectorSearch::add_feature(qint64 index, float *feature) {
    this->index_->add_with_ids(1, feature, &index);
    save_index();
}

void VectorSearch::save_index() {
    //  save_file_name 为无后缀的文件名字，如 vector、vector_new 等
    faiss::write_index(this->index_, INDEX_FILE);
}


void VectorSearch::clear_feature() { this->index_->reset(); }


void VectorSearch::delete_instance() {
    delete index_;
    index_ = nullptr;
    delete pInstance;
    pInstance = nullptr;
}

void VectorSearch::build_index()
{
    auto index =
        faiss::index_factory(VECTOR_DIMENSION, "HNSW32", faiss::METRIC_INNER_PRODUCT);
    this->index_ = new faiss::IndexIDMap(index);
    save_index();
}

void VectorSearch::create_index(QMap<qint64, FaceInfo> libs) {

    if(this->index_ == nullptr){
        build_index();
    }
    if (!libs.empty()) {
        auto keys = libs.keys();
        for (auto &key : keys) {
            float *feature = libs[key].feature.data();
            this->index_->add_with_ids(1, feature, &key);
        }
    }
    save_index();
}
