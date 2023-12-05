#pragma once

#include <QSettings>
#include <QDebug>


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

    //Base
    [[nodiscard]] const QString &get_indexFile() const;

    [[nodiscard]] int get_topK() const;

    [[nodiscard]] int get_vectorSize() const;

    [[nodiscard]] int get_maxFaceNum() const;

    [[nodiscard]] int get_recInterval() const;

    [[nodiscard]] float get_recThreshold() const;

    [[nodiscard]] int get_recordInterval() const;

    [[nodiscard]] const QString &get_modelDir() const;

    [[nodiscard]] int get_faceRecognitionThreadNum() const;

    [[nodiscard]] int get_socketPort() const;

    [[nodiscard]] bool get_isWriteLog() const;

    [[nodiscard]] QString get_logFile() const;

    [[nodiscard]] QString get_gateway() const;

    // Camera
    [[nodiscard]] CameraType get_cameraType() const;

    [[nodiscard]] int get_cameraIndex() const;

    [[nodiscard]] int get_frameWidth() const;

    [[nodiscard]] int get_frameHeight() const;

    // Display
    [[nodiscard]] bool get_framelessStatus() const;

    [[nodiscard]] int get_displayOffInterval() const;

private:
    explicit Config();

    ~Config() override = default;

    Config(const Config &) = delete;

    Config &operator=(const Config &) = delete;

private:
    std::shared_ptr<QSettings> settings_;

    // Base
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

    bool is_write_log;
    QString log_file;
    QString gateway;

    // Camera
    int camera_type;
    int camera_index;
    int frame_width;
    int frame_height;

    //Display
    bool is_frameless;
    int display_off_interval;
};


