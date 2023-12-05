#include "utils/config.h"
#include <QFileInfo>


Config::Config() {

    settings = std::make_shared<QSettings>("setting.ini", QSettings::IniFormat);
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
    socket_port = settings->value("socket_port").toInt();
    is_write_log = settings->value("is_write_log").toBool();
    log_file = settings->value("log_file").toString();
    gateway = settings->value("gateway").toString();
    settings->endGroup();

    settings->beginGroup("Camera");
    camera_type = settings->value("camera_type").toInt();
    camera_index = settings->value("camera_index").toInt();
    frame_width = settings->value("frame_width").toInt();
    frame_height = settings->value("frame_height").toInt();
    settings->endGroup();

    settings->beginGroup("Display");
    is_frameless = settings->value("is_frameless").toBool();
    display_off_interval = settings->value("display_off_interval").toInt();
    settings->endGroup();

}

const QString &Config::get_index_file() const {
    return index_file;
}

int Config::get_top_k() const {
    return top_k;
}

int Config::get_vector_size() const {
    return vector_size;
}

int Config::get_max_face_num() const {
    return max_face_num;
}

int Config::get_rec_interval() const {
    return rec_interval;
}

float Config::get_rec_threshold() const {
    return rec_threshold;
}

int Config::get_record_interval() const {
    return record_interval;
}

const QString &Config::get_model_dir() const {
    return model_dir;
}

int Config::get_face_recognition_thread_num() const {
    return face_recognition_thread_num;
}

int Config::get_socket_port() const {
    return socket_port;
}

bool Config::get_framelessStatus() const {
    return is_frameless;
}

bool Config::get_is_write_log() const {
    return is_write_log;
}

QString Config::get_log_file() const {
    return log_file;
}

QString Config::get_gateway() const {
    return gateway;
}

int Config::get_camera_index() const {
    return camera_index;
}

Config::CameraType Config::get_camera_type() const {
    return cameraTypeIndexToCameraType(camera_type);
}

int Config::get_frame_width() const {
    return frame_width;
}

int Config::get_frame_height() const {
    return frame_height;
}

int Config::get_displayOffInterval() const {
    return display_off_interval;
}
