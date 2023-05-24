//
// Created by aichao on 2022/5/11.
//

#include "SeetaFace.h"

using namespace std;
using namespace seeta;

SeetaFace::SeetaFace() {

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
    // face_anti_spoofing
    seeta::ModelSetting fs_setting;
    fs_setting.append(model_dir + "fas_first.csta");
    fs_setting.set_device(ModelSetting::CPU);

    FD = std::make_shared<FaceDetector>(fd_setting);
    FD->set(seeta::FaceDetector::PROPERTY_NUMBER_THREADS,global_thread_nums);
    // unsupported set thread number
    FL = std::make_shared<FaceLandmarker>(pd_setting);
    FR = std::make_shared<FaceRecognizer>(fr_setting);
    FR->set(seeta::FaceRecognizer::PROPERTY_NUMBER_THREADS,global_thread_nums);
    FS = std::make_shared<FaceAntiSpoofing>(fs_setting);
    FS->set(seeta::FaceAntiSpoofing::PROPERTY_NUMBER_THREADS,global_thread_nums);
}


void SeetaFace::create_face_libs(const QString &json_path) {
    qInfo() << "==================start register face library===================";
    QFile json_file(json_path);
    if (!json_file.open(QIODevice::ReadWrite)) {
        qWarning() << "json file open failed.";
        return;
    }
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
            qWarning() << "注册人脸失败【Name:" << qPrintable(name) << "】";
        }
        qInfo() << "注册人脸成功【Name:" << qPrintable(name) << "】";
    }
    qInfo() << "==================face register successfully!===================";
}

bool SeetaFace::create_face_lib(const QString &image_path, const QString &name, const QString &staff_id) {

    shared_ptr<float> feature(new float[FR->GetExtractFeatureSize()]);
    cv::Mat image = cv::imread(image_path.toStdString());
    bool rt = extract_feature(image, feature.get());
    if (!rt) {
        return false;
    }
    FaceLibInfo face_lib_info{name, staff_id, feature};
    face_lib.emplace_back(face_lib_info);
    return true;
}


bool SeetaFace::extract_feature(cv::Mat &img, float *feature) {
    SeetaImageData data = Utils::CvMat2Simg(img);
    auto faces = FD->detect(data);
    if (faces.size <= 0) {
        return false;
    }
    SeetaPointF points[5];
    FL->mark(data, faces.data[0].pos, points);
    FR->Extract(data, points, feature);
    return true;
}

std::vector<SeetaFaceInfo> SeetaFace::face_detection(cv::Mat &img)
{
    SeetaImageData data = Utils::CvMat2Simg(img);
    auto faces_ = FD->detect(data);
    std::vector<SeetaFaceInfo> faces;
    for(int i=0;i<faces_.size;i++){
        faces.push_back(faces_.data[i]);
    }
    // 排序，将人脸由大到小进行排列
    std::partial_sort(faces.begin(),faces.begin() + 1,faces.end(),
                      [](SeetaFaceInfo a, SeetaFaceInfo b){
        return a.pos.width>b.pos.width;
    });
    return faces;
}

QPair<int,float> SeetaFace::face_recognition(cv::Mat &img, std::vector<SeetaPointF> points) {
    SeetaImageData data = Utils::CvMat2Simg(img);
    unique_ptr<float[]> feature(new float[FR->GetExtractFeatureSize()]);
    FR->Extract(data, points.data(), feature.get());
    int target_index = -1;
    float max_sim = 0;
    for (size_t index = 0; index < face_lib.size(); ++index) {
        auto &face_lb_info = face_lib[index];
        float current_sim = FR->CalculateSimilarity(feature.get(), face_lb_info.feature.get());
        if (current_sim > max_sim) {
            max_sim = current_sim;
            target_index = index;
        }
    }
    //    if (max_sim > m_threshold) {
    //        faceRecRet.face_id = face_lib[target_index].staff_id;
    //        faceRecRet.name = face_lib[target_index].name;
    //        faceRecRet.score = max_sim;
    //    } else {
    //        faceRecRet.face_id = "-1";
    //        faceRecRet.name = "unknown";
    //        faceRecRet.score = max_sim;
    //    }
    return {target_index,max_sim};
}

std::vector<SeetaPointF> SeetaFace::face_marker(cv::Mat &img,
                                                const SeetaRect &rect) {
    SeetaImageData data = Utils::CvMat2Simg(img);
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

Status SeetaFace::face_anti_spoofing(cv::Mat &img, const SeetaRect &rect,
                                     std::vector<SeetaPointF> points) {
    SeetaImageData data = Utils::CvMat2Simg(img);
    return FS->Predict(data, rect, points.data());
}



