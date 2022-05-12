//
// Created by aichao on 2022/5/11.
//

#include "include/SeetaFace.h"

using namespace std;
using namespace seeta;

SeetaFace::SeetaFace() {

    model_dir = string("/Users/aichao/Documents/GitHub/index/model/");
    m_threshold = 0.6;

    ModelSetting fd_setting;
    fd_setting.append(model_dir + "face_detector.csta");
    fd_setting.set_device(ModelSetting::CPU);
    // face_land_marker
    ModelSetting pd_setting;
    pd_setting.append(model_dir + "face_landmarker_pts5.csta");
    pd_setting.set_device(ModelSetting::CPU);
    //face_recognizer
    ModelSetting fr_setting;
    fr_setting.append(model_dir + "face_recognizer.csta");
    fr_setting.set_device(ModelSetting::CPU);
    //face_tractor
    ModelSetting ft_setting;
    ft_setting.append(model_dir + "face_detector.csta");
    ft_setting.set_device(ModelSetting::CPU);
    // face_anti_spoofing
    seeta::ModelSetting fs_setting;
    fs_setting.append(model_dir + "fas_first.csta");
    fs_setting.set_device(ModelSetting::CPU);

    FD = std::make_shared<FaceDetector>(fd_setting);
    FL = std::make_shared<FaceLandmarker>(pd_setting);
    FR = std::make_shared<FaceRecognizer>(fr_setting);
    FT = std::make_shared<FaceTracker>(ft_setting, 640, 480);
    FS = std::make_shared<FaceAntiSpoofing>(fs_setting);


}


void SeetaFace::create_face_libs(const QString &json_path) {

    QFile json_file(json_path);
    if (!json_file.open(QIODevice::ReadWrite)) {

        qDebug() << "json file open failed.";
        return;
    }
    qDebug() << "file open successfully!";
    QByteArray data = json_file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray json_array = doc.array();
    for (auto face_info: json_array) {
        QJsonObject json = face_info.toObject();
        QString file_path = json.value("file_path").toString();
        QString name = json.value("name").toString();
        QString staff_id = json.value("staff_id").toString();
        bool ret = create_face_lib(file_path, name, staff_id);
        if (!ret) {
            qDebug() << "添加人脸库失败【Name:" << name << "】";
        }
        qDebug() << "添加人脸成功【Name:" << name << "】";
    }
}

bool SeetaFace::create_face_lib(const QString &image_path, const QString &name, const QString &staff_id) {

    shared_ptr<float> feature(new float[FR->GetExtractFeatureSize()]);
    cv::Mat image = cv::imread(image_path.toStdString());
    bool rt = extract_feature(image, feature.get());
    if (!rt) {
        return false;
    }
    FaceLibInfo face_lib_info{name, staff_id, feature};
//    feature_db.emplace_back(pair<string, shared_ptr<float>>(json_name, feature));
    face_lib.emplace_back(face_lib_info);
    return true;

}

bool SeetaFace::extract_feature(cv::Mat &img, float *feature) {
    SeetaImageData data = Utils::cvMat_2_img(img);
    auto faces = FD->detect(data);
    if (faces.size <= 0) {
        return false;
    }
    SeetaPointF points[5];
    FL->mark(data, faces.data[0].pos, points);
    FR->Extract(data, points, feature);
    return true;
}

SeetaTrackingFaceInfoArray SeetaFace::face_track(cv::Mat &img) {

    SeetaImageData data = Utils::cvMat_2_img(img);
    SeetaTrackingFaceInfoArray out = FT->Track(data);
    return out;
}


FaceRecRet SeetaFace::face_recognition(cv::Mat &img, std::vector<SeetaPointF> points) {
    SeetaImageData data = Utils::cvMat_2_img(img);
    unique_ptr<float[]> feature(new float[FR->GetExtractFeatureSize()]);
    FR->Extract(data, points.data(), feature.get());
    FaceRecRet faceRecRet;
    int64_t target_index = -1;
    float max_sim = 0;
    for (int64_t index = 0; index < face_lib.size(); ++index) {
        auto &face_lb_info = face_lib[index];
        float current_sim = FR->CalculateSimilarity(feature.get(), face_lb_info.feature.get());
        if (current_sim > max_sim) {
            max_sim = current_sim;
            target_index = index;
        }
    }
    if (max_sim > m_threshold) {
        faceRecRet.face_id = face_lib[target_index].staff_id;
        faceRecRet.name = face_lib[target_index].name;
        faceRecRet.score = max_sim;
    } else {
        faceRecRet.face_id = "-1";
        faceRecRet.name = "unknown";
        faceRecRet.score = max_sim;
    }
    return faceRecRet;
}

std::vector<SeetaPointF> SeetaFace::face_marker(cv::Mat &img, SeetaRect &rect) {
    SeetaImageData data = Utils::cvMat_2_img(img);
    int point_nums = FL->number();
    std::vector<SeetaPointF> points(point_nums);
    FL->mark(data, rect, points.data());
    return points;
}

bool SeetaFace::face_quality_authorize(cv::Mat &img) {
    shared_ptr<float> feature(new float[FR->GetExtractFeatureSize()]);
    bool rt = extract_feature(img, feature.get());
    if (!rt) {
        return false;
    }
    return true;
}

Status SeetaFace::face_anti_spoofing(cv::Mat &img, SeetaRect &rect, std::vector<SeetaPointF> points) {
    SeetaImageData data = Utils::cvMat_2_img(img);
    return FS->Predict(data, rect, points.data());
}


