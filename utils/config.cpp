#include "utils/config.h"

Config::Config() {

    settings_ = std::make_shared<QSettings>("setting.ini", QSettings::IniFormat);
    settings_->beginGroup("Base");
    top_k = settings_->value("top_k").toInt();
    index_file = settings_->value("index_file").toString();
    vector_size = settings_->value("vector_size").toInt();
    max_face_num = settings_->value("max_face_num").toInt();
    rec_interval = settings_->value("rec_interval").toInt();
    rec_threshold = settings_->value("rec_threshold").toFloat();
    record_interval = settings_->value("record_interval").toInt();
    model_dir = settings_->value("model_dir").toString();
    face_recognition_thread_num = settings_->value("face_recognition_thread_num").toInt();
    socket_port = settings_->value("socket_port").toInt();
    is_write_log = settings_->value("is_write_log").toBool();
    log_file = settings_->value("log_file").toString();
    gateway = settings_->value("gateway").toString();
    settings_->endGroup();

    settings_->beginGroup("Camera");
    camera_type = settings_->value("camera_type").toInt();
    camera_index = settings_->value("camera_index").toInt();
    frame_width = settings_->value("frame_width").toInt();
    frame_height = settings_->value("frame_height").toInt();
    settings_->endGroup();

    settings_->beginGroup("Display");
    is_frameless = settings_->value("is_frameless").toBool();
    display_off_interval = settings_->value("display_off_interval").toInt();
    settings_->endGroup();
}

const QString &Config::get_indexFile() const {
    return index_file;
}

int Config::get_topK() const {
    return top_k;
}

int Config::get_vectorSize() const {
    return vector_size;
}

int Config::get_maxFaceNum() const {
    return max_face_num;
}

int Config::get_recInterval() const {
    return rec_interval;
}

float Config::get_recThreshold() const {
    return rec_threshold;
}

int Config::get_recordInterval() const {
    return record_interval;
}

const QString &Config::get_modelDir() const {
    return model_dir;
}

int Config::get_faceRecognitionThreadNum() const {
    return face_recognition_thread_num;
}

int Config::get_socketPort() const {
    return socket_port;
}

bool Config::get_framelessStatus() const {
    return is_frameless;
}

bool Config::get_isWriteLog() const {
    return is_write_log;
}

QString Config::get_logFile() const {
    return log_file;
}

QString Config::get_gateway() const {
    return gateway;
}

int Config::get_cameraIndex() const {
    return camera_index;
}

Config::CameraType Config::get_cameraType() const {
    return cameraTypeIndexToCameraType(camera_type);
}

int Config::get_frameWidth() const {
    return frame_width;
}

int Config::get_frameHeight() const {
    return frame_height;
}

int Config::get_displayOffInterval() const {
    return display_off_interval;
}
