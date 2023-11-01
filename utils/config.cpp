#include "utils/config.h"
#include <QFileInfo>

Config *Config::getInstance() {
    // 外层判断避免加锁的开销
    if (config == nullptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (config == nullptr) {
            config = new Config();
        }
    }
    return config;
}

void Config::deleteInstance() {
    delete config;
    config = nullptr;
    delete settings;
    settings = nullptr;
}

void Config::init_settings() {
    // 向量索引topk
    settings->beginGroup("Base");
    settings->setValue("top_k", 5);
    //  faiss 向量索引文件名
    settings->setValue("index_file", "faceindex.vec");
    // 人脸特征向量长度
    settings->setValue("vector_size", 1024);
    // 单次注册人脸时允许录入 的最大人脸数
    settings->setValue("max_face_num", 3);
    // 人脸识别算法间隔(ms)
    settings->setValue("rec_interval", 500);
    // 人脸识别阈值
    settings->setValue("rec_threshold", 0.6);
    // 打卡缓存时间(s)
    settings->setValue("record_interval", 5);
    // 模型路径
    settings->setValue("model_dir", "./models");
    // 人脸识别线程数，建议为1
    settings->setValue("face_recognition_thread_num", 1);
    settings->endGroup();
}


Config::Config() {

    QFileInfo file_info("setting.ini");
    settings = new QSettings("setting.ini", QSettings::IniFormat);
    if (!file_info.exists()) {
        init_settings();
    }
    settings->beginGroup("Base");
    top_k = settings->value("top_k").toInt();
    index_file = settings->value("index_file").toString();
    vector_size = settings->value("vector_size").toInt();
    max_face_num = settings->value("max_face_num").toInt();
    rec_interval = settings->value("rec_interval").toInt();
    rec_threshold = settings->value("rec_threshold").toFloat();
    record_interval = settings->value("record_interval").toInt();
    model_dir = settings->value("model_dir").toString();
    face_recognition_thread_num = settings->value("face_recognition_thread_num").toInt();

    settings->endGroup();
}

const QString &Config::getIndex_file() const {
    return index_file;
}

int Config::getTop_k() const {
    return top_k;
}

int Config::getVector_size() const {
    return vector_size;
}

int Config::getMax_face_num() const {
    return max_face_num;
}

int Config::getRec_interval() const {
    return rec_interval;
}

float Config::getRec_threshold() const {
    return rec_threshold;
}

int Config::getRecord_interval() const {
    return record_interval;
}

const QString &Config::get_model_dir() const {
    return model_dir;
}

int Config::get_face_recognition_thread_num() const {
    return face_recognition_thread_num;
}
