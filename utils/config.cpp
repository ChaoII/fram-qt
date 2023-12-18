#include "utils/config.h"

Config::Config() {

    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.beginGroup("Base");
    top_k_ = settings.value("top_k").toInt();
    index_file_ = settings.value("index_file").toString();
    vector_size_ = settings.value("vector_size").toInt();
    max_face_num_ = settings.value("max_face_num").toInt();
    rec_interval_ = settings.value("rec_interval").toFloat();
    rec_threshold_ = settings.value("rec_threshold").toFloat();
    record_interval_ = settings.value("record_interval").toInt();
    model_dir_ = settings.value("model_dir").toString();
    face_recognition_thread_num_ = settings.value("face_recognition_thread_num").toInt();
    socket_port_ = settings.value("socket_port").toInt();
    is_write_log_ = settings.value("is_write_log").toBool();
    log_file_ = settings.value("log_file").toString();
    gateway_ = settings.value("gateway").toString();
    password_ = settings.value("password").toString();
    settings.endGroup();

    settings.beginGroup("Camera");
    camera_type_ = settings.value("camera_type").toInt();
    camera_index_ = settings.value("camera_index").toInt();
    frame_width_ = settings.value("frame_width").toInt();
    frame_height_ = settings.value("frame_height").toInt();
    settings.endGroup();

    settings.beginGroup("Display");
    is_frameless_ = settings.value("is_frameless").toBool();
    display_off_interval_ = settings.value("display_off_interval").toInt();
    settings.endGroup();
}

const QString &Config::get_indexFile() const {
    return index_file_;
}

int Config::get_topK() const {
    return top_k_;
}

int Config::get_vectorSize() const {
    return vector_size_;
}

int Config::get_maxFaceNum() const {
    return max_face_num_;
}

float Config::get_recInterval() const {
    return rec_interval_;
}

float Config::get_recThreshold() const {
    return rec_threshold_;
}

int Config::get_recordInterval() const {
    return record_interval_;
}

const QString &Config::get_modelDir() const {
    return model_dir_;
}

int Config::get_faceRecognitionThreadNum() const {
    return face_recognition_thread_num_;
}

int Config::get_socketPort() const {
    return socket_port_;
}

bool Config::get_framelessStatus() const {
    return is_frameless_;
}

bool Config::get_isWriteLog() const {
    return is_write_log_;
}

QString Config::get_logFile() const {
    return log_file_;
}

QString Config::get_gateway() const {
    return gateway_;
}

int Config::get_cameraIndex() const {
    return camera_index_;
}

Config::CameraType Config::get_cameraType() const {
    return cameraTypeIndexToCameraType(camera_type_);
}

int Config::get_cameraTypeIndex() const {
    return camera_type_;
}

int Config::get_frameWidth() const {
    return frame_width_;
}

int Config::get_frameHeight() const {
    return frame_height_;
}

int Config::get_displayOffInterval() const {
    return display_off_interval_;
}

QString Config::get_administratorPassword() const {
    return password_;
}

void Config::updateSettings(const QString &index_file, int vector_size, int max_face_num, double rec_interval,
                            double rec_threshold, int record_interval, int face_recognition_thread_num, int socket_port,
                            bool is_write_log, const QString &log_file, const QString &gateway, int top_k,
                            const QString &model_dir, int camera_type, int camera_index, int frame_width,
                            int frame_height, bool is_frameless, int display_off_interval) {
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.beginGroup("Base");
    settings.setValue("top_k", top_k);
    settings.setValue("index_file", index_file);
    settings.setValue("vector_size", vector_size);
    settings.setValue("max_face_num", max_face_num);
    settings.setValue("rec_interval", rec_interval);
    settings.setValue("rec_threshold", rec_threshold);
    settings.setValue("record_interval", record_interval);
    settings.setValue("model_dir", model_dir);
    settings.setValue("face_recognition_thread_num", face_recognition_thread_num);
    settings.setValue("socket_port", socket_port);
    settings.setValue("is_write_log", is_write_log);
    settings.setValue("log_file", log_file);
    settings.setValue("gateway", gateway);
    settings.setValue("password", password_);
    settings.endGroup();

    settings.beginGroup("Camera");
    settings.setValue("camera_type", camera_type);
    settings.setValue("camera_index", camera_index);
    settings.setValue("frame_width", frame_width);
    settings.setValue("frame_height", frame_height);
    settings.endGroup();

    settings.beginGroup("Display");
    settings.setValue("is_frameless", is_frameless);
    settings.setValue("display_off_interval", display_off_interval);
    settings.endGroup();
}


