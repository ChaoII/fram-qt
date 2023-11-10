#pragma once

#include <QSettings>
#include <mutex>
#include <QDebug>

//// 向量索引topk
//#define TOP_K 5
////  faiss 向量索引文件名
//#define INDEX_FILE "faceindex.vec"
//// 人脸特征向量长度
//#define VECTOR_SIZE 1024
//// 單次注册人脸时允许录入 的最大人脸数
//#define MAX_FACE 3
//// 人脸识别算法间隔(ms)
//#define REC_INTERVAL 500
//// 人脸识别阈值
//#define REC_THRESHOLD 0.6
//// 打卡缓存时间(s)
//#define RECORD_INTERVAL 5


class Config : public QObject {
public:
    static Config &getInstance() {
        static Config config;
        return config;
    };

    void init_settings();

    const QString &get_index_file() const;

    int get_top_k() const;

    int get_vector_size() const;

    int get_max_face_num() const;

    int get_rec_interval() const;

    float get_rec_threshold() const;

    int get_record_interval() const;

    const QString &get_model_dir() const;

    int get_face_recognition_thread_num() const;

    int get_socket_port() const;

private:
    explicit Config();

    ~Config() = default;

    Config(const Config &) = delete;

    Config &operator=(const Config &) = delete;

private:
    std::shared_ptr<QSettings> settings;
    QString index_file;
    int top_k;
    int vector_size;
    int max_face_num;
    int rec_interval;
    float rec_threshold;
    int record_interval;
    QString model_dir;
    int face_recognition_thread_num;
    int socket_port;
};


