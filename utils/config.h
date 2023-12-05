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
    enum class CameraType {
        USB,
        MIPI,
        None
    };

    static CameraType cameraTypeIndexToCameraType(int camera_type_index) {
        switch (camera_type_index) {
            case 0:
                return CameraType::USB;
            case 1:
                return CameraType::MIPI;
            default:
                return CameraType::None;
        }
    }

public:
    static Config &getInstance() {
        static Config config;
        return config;
    };

    [[nodiscard]] const QString &get_index_file() const;

    [[nodiscard]] int get_top_k() const;

    [[nodiscard]] int get_vector_size() const;

    [[nodiscard]] int get_max_face_num() const;

    [[nodiscard]] int get_rec_interval() const;

    [[nodiscard]] float get_rec_threshold() const;

    [[nodiscard]] int get_record_interval() const;

    [[nodiscard]] const QString &get_model_dir() const;

    [[nodiscard]] int get_face_recognition_thread_num() const;

    [[nodiscard]] int get_socket_port() const;

    [[nodiscard]] bool get_framelessStatus() const;

    [[nodiscard]] bool get_is_write_log() const;

    [[nodiscard]] QString get_log_file() const;

    [[nodiscard]] QString get_gateway() const;

    // camera
    [[nodiscard]] CameraType get_camera_type() const;

    [[nodiscard]] int get_camera_index() const;

    [[nodiscard]] int get_frame_width() const;

    [[nodiscard]] int get_frame_height() const;

private:
    explicit Config();

    ~Config() override = default;

    Config(const Config &) = delete;

    Config &operator=(const Config &) = delete;

private:
    std::shared_ptr<QSettings> settings;

    // base
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

    bool is_frameless;
    bool is_write_log;
    QString log_file;
    QString gateway;

    // camera
    int camera_type;
    int camera_index;
    int frame_width;
    int frame_height;
};


