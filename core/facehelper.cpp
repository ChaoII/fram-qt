#include "core/facehelper.h"
#include "common/sqlhelper.h"
#include "common/utility.h"
#include "core/vectorsearch.h"

QMutex FaceHelper::mutex_;
FaceHelper *FaceHelper::face_helpher_instance_ = nullptr;
FaceHelper::FaceHelper() {
    fastdeploy::RuntimeOption option;
    option.UseOrtBackend();
    option.SetCpuThreadNum(1);
    face_det_ = new fastdeploy::vision::facedet::SCRFD(
        "../models/scrfd_2.5g_bnkps_shape640x640.onnx", "", option);
    face_rec_ = new fastdeploy::vision::faceid::ArcFace(
        "../models/ms1mv3_arcface_r50.onnx", "", option);
    QFileInfo file_info(FACE_IMAGE_DIR);
    if (!file_info.exists()) {
        QDir dir;
        dir.mkdir(file_info.absoluteFilePath());
    }
    initial_facelibs();
}

void FaceHelper::initial_facelibs() {
    QFile file(FACE_LIBS);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "file" << FACE_LIBS << "open failed";
        return;
    }
    QDataStream in(&file);
    in >> face_libs_;
    file.close();

    QFileInfo file_info(DB_FILE);
    if (file_info.exists()) {
        return;
    }
    // 创建人脸信息表
    QVector<QString> tb_face_info;
    tb_face_info << "id"
                 << "INTEGER PRIMARY KEY";
    tb_face_info << "uid"
                 << "int";
    tb_face_info << "name"
                 << "varchar";
    tb_face_info << "picture"
                 << "varchar";
    tb_face_info << "updatetime"
                 << "varchar";
    SqlHelper::getInstance()->createTable(FACE_INFO_TABLE, tb_face_info);
    // 创建打卡记录表
    QVector<QString> tb_attend;
    tb_attend << "id"
              << "INTEGER PRIMARY KEY autoincrement";
    tb_attend << "uid"
              << "int";
    tb_attend << "name"
              << "varchar";
    tb_attend << "attendtime"
              << "varchar";
    tb_attend << "picture"
              << "varchar";
    SqlHelper::getInstance()->createTable(ATTEND_RECORD_TABLE, tb_attend);
}

QMap<qint64, FaceInfo> FaceHelper::get_facelibs() {
    return FaceHelper::face_libs_;
}

void FaceHelper::add_database(cv::Mat img, QString uid, QString name) {

    cv::Mat roi = get_face_roi(img);
    std::vector<float> feature = get_face_feature(roi);
    qint64 uuid = Utility::get_uuid();
    FaceInfo face_info{uid.toInt(), name,
                       QVector<float>(feature.begin(), feature.end())};
    face_libs_[uuid] = face_info;

    //人脸特征写入文件
    QFile file("facelibs.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << face_libs_;
    file.close();
    //写入图片
    QString picture = FACE_IMAGE_DIR + QString::number(uuid) + "_" + uid + ".jpg";
    cv::imwrite(picture.toStdString(), img);
    VectorSearch::getInstance()->add_feature(uuid, feature.data());

    QVector<QString> records;
    records << "id" << QString::number(uuid);
    records << "uid" << uid;
    records << "name" << name;
    records << "picture" << picture;
    records << "updatetime"
            << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    SqlHelper::getInstance()->addRecord(FACE_INFO_TABLE, records);
}

void FaceHelper::delete_face(QString uid) {}

cv::Rect FaceHelper::get_face_box(cv::Mat img) {
    cv::Rect rect;
    fastdeploy::vision::FaceDetectionResult det_result;
    face_det_->Predict(&img, &det_result);
    if (!det_result.boxes.empty()) {
        auto box = det_result.boxes[0];
        rect = cv::Rect(box[0], box[1], box[2] - box[0], box[3] - box[1]);
    }
    return rect;
}

cv::Mat FaceHelper::get_face_roi(cv::Mat img) {
    cv::Mat roi;
    auto rect = get_face_box(img);
    if (!rect.empty())
        roi = img(rect).clone();
    return roi;
}

std::vector<float> FaceHelper::get_face_feature(cv::Mat img) {
    fastdeploy::vision::FaceRecognitionResult rec_result;
    face_rec_->Predict(&img, &rec_result);
    std::vector<float> feature = rec_result.embedding;
    std::vector<float> feature_normalize = Utility::L2Normalize(feature);
    return feature_normalize;
}

FaceHelper *FaceHelper::getInstance() {
    if (face_helpher_instance_ == nullptr) {
        mutex_.lock();
        if (face_helpher_instance_ == nullptr) {
            face_helpher_instance_ = new FaceHelper();
        }
        mutex_.unlock();
    }
    return face_helpher_instance_;
}

void FaceHelper::delete_instance() {
    delete face_det_;
    face_det_ = nullptr;
    delete face_rec_;
    face_rec_ = nullptr;
    delete face_helpher_instance_;
    face_helpher_instance_ = nullptr;
}
