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

    [[nodiscard]] float get_recInterval() const;

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

    [[nodiscard]] int get_cameraTypeIndex() const;

    [[nodiscard]] int get_cameraIndex() const;

    [[nodiscard]] int get_frameWidth() const;

    [[nodiscard]] int get_frameHeight() const;

    // Display
    [[nodiscard]] bool get_framelessStatus() const;

    [[nodiscard]] int get_displayOffInterval() const;

    [[nodiscard]] QString get_administratorPassword() const;

    void updateSettings(const QString &index_file, int vector_size, int max_face_num, double rec_interval,
                        double rec_threshold, int record_interval, int face_recognition_thread_num, int socket_port,
                        bool is_write_log, const QString &log_file, const QString &gateway, int top_k,
                        const QString &model_dir, int camera_type, int camera_index, int frame_width,
                        int frame_height, bool is_frameless, int display_off_interval);

private:
    explicit Config();

    ~Config() override = default;

    Config(const Config &) = delete;

    Config &operator=(const Config &) = delete;

private:
    // Base
    QString index_file_;
    int top_k_;
    int vector_size_;
    int max_face_num_;
    float rec_interval_;
    float rec_threshold_;
    int record_interval_;
    QString model_dir_;
    int face_recognition_thread_num_;
    int socket_port_;
    bool is_write_log_;
    QString log_file_;
    QString gateway_;
    QString password_;

    // Camera
    int camera_type_;
    int camera_index_;
    int frame_width_;
    int frame_height_;

    //Display
    bool is_frameless_;
    int display_off_interval_;
};


