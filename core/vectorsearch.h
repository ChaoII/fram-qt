#ifndef VECTORSEARCH_H
#define VECTORSEARCH_H

#include <QtWidgets>
#include <cstring>
#include <faiss/Index.h>
#include <faiss/index_io.h>
#include <map>
#include <vector>
#include  "common/config.h"

struct SearchResult {
    std::vector<qint64> I;
    std::vector<float> D;
    int return_k;
};

class VectorSearch {

private:
    VectorSearch();

    VectorSearch(const VectorSearch &);

    VectorSearch &operator=(const VectorSearch &);

public:
    ///
    /// \brief getInstance 获取静态实例
    /// \return
    ///
    static VectorSearch *getInstance();

    ///
    /// \brief load_index_file 如果存在文件那么加载index文件
    /// \return
    ///
    bool load_index_file();
    ///
    /// \brief add_feature 添加索引
    /// \param index 索引index
    /// \param feature 向量
    ///
    void add_features(const QVector<qint64>& ids, float* features);
    ///
    /// \brief search 向量查找
    /// \param feature 特征向量
    /// \param query_number 查询数
    /// \return SearchResult结构体
    ///
    const SearchResult &search(float *feature, int query_number);
    ///
    /// \brief build_index 通过外部数据创建index，index为HNSW32
    /// \param libs QMap<qint64, FaceInfo>
    ///
    void create_index(QMap<qint64, FaceInfo> libs);
    ///
    /// \brief get_index_length 获取index长度
    /// \return
    ///
    const int get_index_length();
    ///
    /// \brief save_index 保存index文件
    ///
    void save_index();
    ///
    /// \brief clear_feature 重置index
    ///
    void clear_feature();
    ///
    /// \brief delete_instance 删除实例
    ///
    void delete_instance();

    void build_index();

    void delete_ids();

private:
    /// pInstance 全局静态实例
    static VectorSearch *pInstance;
    /// 线程锁
    static QMutex mutex_;
    faiss::Index *index_ = nullptr;
    std::vector<float> D_;
    std::vector<qint64> I_;
    SearchResult result_;
};

#endif // VECTORSEARCH_H
