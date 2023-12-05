//
// Created by aichao on 2022/5/11.
//

#include "core/seetaface.h"
#include "models/models.h"
#include "widgets/mysplashscreen.h"

using namespace std;
using namespace seeta;

SeetaFace::SeetaFace() {

    MySplashScreen::getInstance().updateProcess("load face detect model...");
    std::string model_dir = Config::getInstance().get_modelDir().toStdString();
    int face_recognition_thread_num = Config::getInstance().get_faceRecognitionThreadNum();
    ModelSetting fd_setting;
    fd_setting.append(model_dir + "face_detector.csta");
    fd_setting.set_device(ModelSetting::CPU);
    FD_ = std::make_shared<FaceDetector>(fd_setting);
    FD_->set(seeta::FaceDetector::PROPERTY_NUMBER_THREADS, face_recognition_thread_num);
    // face_land_marker
    MySplashScreen::getInstance().updateProcess("load face landmark model...");
    ModelSetting pd_setting;
    pd_setting.append(model_dir + "face_landmarker_pts5.csta");
    pd_setting.set_device(ModelSetting::CPU);
    FL_ = std::make_shared<FaceLandmarker>(pd_setting);
    //face_recognizer
    MySplashScreen::getInstance().updateProcess("load face recognition model...");
    ModelSetting fr_setting;
    fr_setting.append(model_dir + "face_recognizer.csta");
    fr_setting.set_device(ModelSetting::CPU);
    FR_ = std::make_shared<FaceRecognizer>(fr_setting);
    FR_->set(seeta::FaceRecognizer::PROPERTY_NUMBER_THREADS, face_recognition_thread_num);
    // face_anti_spoofing
    MySplashScreen::getInstance().updateProcess("load face anti spoofing model...");
    seeta::ModelSetting fs_setting;
    fs_setting.append(model_dir + "fas_first.csta");
    fs_setting.set_device(ModelSetting::CPU);
    FS_ = std::make_shared<FaceAntiSpoofing>(fs_setting);
    FS_->set(seeta::FaceAntiSpoofing::PROPERTY_NUMBER_THREADS, face_recognition_thread_num);
    // unsupported set thread number


    MySplashScreen::getInstance().updateProcess("init face libraries...");
    initFaceDb();
    MySplashScreen::getInstance().updateProcess("init file directory...");
    initFileDir();
}

void SeetaFace::initFileDir() {
    QDir dir;
    if (!dir.exists(QDir::currentPath() + "/faces")) {
        dir.mkdir(QDir::currentPath() + "/faces");
    };
    if (!dir.exists(QDir::currentPath() + "/attend")) {
        dir.mkdir(QDir::currentPath() + "/attend");
    };
}

void SeetaFace::initFaceDb() {
    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName("data.db");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(false);
    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlBoundValuesOnError(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlBoundValues(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlBoundValuesOnError(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlOnlySlowQueriesDatabase(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlOnlySlowQueriesTotal(false);
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(false);


    QSqlError staff_error = qx::dao::create_table<Staff>();
    if (staff_error.isValid()) {
        qDebug() << staff_error.text();
    }

    QSqlError attend_error = qx::dao::create_table<Attend>();
    if (attend_error.isValid()) {
        qDebug() << attend_error.text();
    }

    QFileInfo fileInfo(Config::getInstance().get_indexFile().toStdString().c_str());
    if (!fileInfo.exists()) {
        buildFaceIndexFromDb();
    }
}

bool SeetaFace::addFace(cv::Mat &img, const QString &uid, const QString &name) {
    unique_ptr<float[]> feature(new float[FR_->GetExtractFeatureSize()]);
    bool ret = SeetaFace::getInstance().extractFeature(img, feature.get());
    if (!ret) return false;
    qint64 uuid = utils::getUuid();
    QDir dir("./faces/" + uid);
    if (!dir.exists()) {
        dir.mkdir(dir.absolutePath());
    }
    QString file_path = dir.relativeFilePath("./faces/" + uid + "/" + QString::number(uuid) + ".jpg");
    qDebug() << file_path;
    Staff staff;
    staff.index_id = uuid;
    staff.uid = uid;
    staff.name = name;
    staff.pic_url = file_path;
    staff.feature = utils::floatArray2QByteArray(feature.get(), FR_->GetExtractFeatureSize());
    staff.register_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    auto sql_error = qx::dao::insert(staff);
    if (sql_error.isValid()) {
        qDebug() << "新增人脸失败，name:" << name << "uid:" << uid << "失败原因：" << sql_error.text();
        return false;
    }
    cv::imwrite(file_path.toStdString(), img);
    VectorSearch::getInstance().addFeatures({uuid}, feature.get());
    qDebug() << "新增人脸成功，name:" << name << "uid:" << uid;
    return true;
}

bool SeetaFace::deleteFace(const QString &uid) {
    Staff staff;
    qx_query query;
    query.where("uid").isEqualTo(uid);
    QSqlError sql_error = qx::dao::delete_by_query<Staff>(query);
    if (sql_error.isValid()) {
        qDebug() << "删除人脸失败，失败原因" << sql_error.text();
        return false;
    }
    qDebug() << "删除人脸成功，uid:" << uid;
    return true;
}

void SeetaFace::buildFaceIndexFromDb() {
    std::vector<Staff> staffs;
    QSqlError sql_error = qx::dao::fetch_all(staffs);
    if (sql_error.isValid()) {
        return;
    }
    std::vector<int64_t> ids;
    std::vector<float> features;
    for (auto &staff: staffs) {
        ids.push_back(staff.index_id);
        auto feature = utils::QByteArray2Vector<float>(staff.feature);
        features.insert(features.end(), feature.begin(), feature.end());
    }
    VectorSearch::getInstance().addFeatures(ids, features.data());
}

QString SeetaFace::getFacePathFromIndexId(qint64 index_id) {
    Staff staff;
    qx_query query;
    query.where("index_id").isEqualTo(index_id);
    QSqlError sql_error = qx::dao::fetch_by_query<Staff>(query, staff);
    if (sql_error.isValid()) {
        qDebug() << "file is not existed";
        return "";
    }
    return staff.pic_url;
}

Staff SeetaFace::getFaceInfoFromIndexId(qint64 index_id) {
    Staff staff;
    qx_query query;
    query.where("index_id").isEqualTo(index_id);
    QSqlError sql_error = qx::dao::fetch_by_query<Staff>(query, staff);
    if (sql_error.isValid()) {
        return {};
    }
    std::vector feature = utils::QByteArray2Vector<float>(staff.feature);
    return staff;
}

bool SeetaFace::updateFace(cv::Mat &img, const QString &uid, const QString &name) {
    Staff staff;
    staff.uid = uid;
    staff.name = name;
    qx_query query;
    unique_ptr<float[]> feature(new float[FR_->GetExtractFeatureSize()]);
    SeetaFace::getInstance().extractFeature(img, feature.get());
    staff.feature = utils::floatArray2QByteArray(feature.get(), FR_->GetExtractFeatureSize());
    query.where("uid").isEqualTo(uid);
    auto sql_error = qx::dao::update_by_query<Staff>(query, staff);
    if (sql_error.isValid()) {
        qDebug() << "更新人脸失败，失败原因" << sql_error.text();
        return false;
    }
    qDebug() << "更新人脸成功，name:" << name << "uid:" << uid;
    return true;
}

bool SeetaFace::extractFeature(cv::Mat &img, float *feature) {

    SeetaImageData data = utils::cvMatToSImg(img);
    auto faces = FD_->detect(data);
    if (faces.size <= 0) {
        return false;
    }
    SeetaPointF points[5];
    FL_->mark(data, faces.data[0].pos, points);
    FR_->Extract(data, points, feature);
    return true;
}

std::vector<SeetaFaceInfo> SeetaFace::faceDetection(cv::Mat &img) {
    SeetaImageData data = utils::cvMatToSImg(img);
    auto faces_ = FD_->detect(data);
    std::vector<SeetaFaceInfo> faces;
    for (int i = 0; i < faces_.size; i++) {
        faces.push_back(faces_.data[i]);
    }
    // 排序，将人脸由大到小进行排列
    std::partial_sort(faces.begin(), faces.begin() + 1, faces.end(),
                      [](SeetaFaceInfo a, SeetaFaceInfo b) {
                          return a.pos.width > b.pos.width;
                      });
    return faces;
}

QPair<int64_t, float> SeetaFace::faceRecognition(cv::Mat &img, std::vector<SeetaPointF> points) {
    SeetaImageData data = utils::cvMatToSImg(img);
    unique_ptr<float[]> feature(new float[FR_->GetExtractFeatureSize()]);
    FR_->Extract(data, points.data(), feature.get());
    SearchResult result = VectorSearch::getInstance().search(feature.get(), 3);
    return {result.I[0], result.D[0]};
}

std::vector<SeetaPointF> SeetaFace::faceMarker(cv::Mat &img, const SeetaRect &rect) {
    SeetaImageData data = utils::cvMatToSImg(img);
    int point_nums = FL_->number();
    std::vector<SeetaPointF> points(point_nums);
    FL_->mark(data, rect, points.data());
    return points;
}

bool SeetaFace::faceQualityAuthorize(cv::Mat &img) {
    shared_ptr<float> feature(new float[FR_->GetExtractFeatureSize()]);
    bool rt = extractFeature(img, feature.get());
    if (!rt) {
        return false;
    }
    return true;
}

Status SeetaFace::faceAntiSpoofing(cv::Mat &img, const SeetaRect &rect, std::vector<SeetaPointF> points) {
    SeetaImageData data = utils::cvMatToSImg(img);
    return FS_->Predict(data, rect, points.data());
}

bool SeetaFace::deleteFaceByIds(const std::vector<int64_t> &ids) {
    QVariantList variant_ids;
    for (auto &id: ids) {
        variant_ids.append(QVariant::fromValue(id));
    }
    qx_query query;
    query.where("index_id").in(variant_ids);
    QVector<Staff> staffs;
    qx::dao::fetch_by_query(query, staffs);
    for (auto &staff: staffs) {
        QFileInfo file_info(staff.pic_url);
        if (file_info.exists()) {
            file_info.dir().remove(file_info.fileName());
            if (file_info.dir().entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot).count() <= 0) {
                file_info.dir().removeRecursively();
                qDebug() << "清空文件夹:" << file_info.dir().absolutePath();
            }
            qDebug() << staff.uid << staff.name << file_info.fileName();
        }
    }
    QSqlError sql_error = qx::dao::delete_by_query<Staff>(query);
    if (sql_error.isValid()) {
        qDebug() << "delete data error , details:" << sql_error.text();
        return false;
    }
    size_t nums = VectorSearch::getInstance().removeIndex(ids);
    if (nums != ids.size()) {
        qDebug() << "delete failed";
        return false;
    }
    return true;
}





