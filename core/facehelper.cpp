#include "core/facehelper.h"
#include "common/utility.h"
#include "core/staff.h"
#include "core/vectorsearch.h"

QMutex FaceHelper::mutex_;
FaceHelper *FaceHelper::face_helpher_instance_ = nullptr;
FaceHelper::FaceHelper() {
    fastdeploy::RuntimeOption option;
    option.UseOrtBackend();
    option.SetCpuThreadNum(1);
    face_det_ = new fastdeploy::vision::facedet::SCRFD(
        "./models/scrfd_2.5g_bnkps_shape640x640.onnx", "", option);
    face_rec_ = new fastdeploy::vision::faceid::ArcFace(
        "./models/ms1mv3_arcface_r50.onnx", "", option);
    QFileInfo file_info(FACE_IMAGE_DIR);
    if (!file_info.exists()) {
        QDir dir;
        dir.mkdir(file_info.absoluteFilePath());
    }
    QFileInfo index_info(INDEX_FILE);
    if (!index_info.exists()) {
        refresh_faceindex();
    }
}

void FaceHelper::refresh_faceindex() {
    QVector<StaffPtr> staffs;
    QSqlError sql_error = qx::dao::fetch_all(staffs);
    if(!sql_error.isValid()){
        QVector<qint64> ids;
        QVector<float> all_data_temp;
        for (auto &staff : staffs) {
            qint64 id = staff->id;
            QString uid = staff->uid;
            QString name = staff->name;
            QVector<float> feature = Utility::byteArray2Vectorf(staff->feature);
            ids.push_back(id);
            all_data_temp.append(feature);
        }
        VectorSearch::getInstance()->add_features(ids, all_data_temp.data());
    }
}

QMap<qint64, FaceInfo> FaceHelper::get_facelibs()
{
    return m_facelibs;
}

FaceInfo FaceHelper::get_face_info_from_id(qint64 id)
{
    FaceInfo face_info;
    StaffPtr staff(new Staff());
    qx_query query;
    query.where("id").isEqualTo(id);
    QSqlError sql_error = qx::dao::fetch_by_query(query,staff);
    if(!sql_error.isValid()){
        face_info.uid = staff->uid;
        face_info.name = staff->name;
        face_info.feature = Utility::byteArray2Vectorf(staff->feature);
    }
    return face_info;
}

void FaceHelper::add_database(cv::Mat img, const QString &uid,
                              const QString &name) {

    cv::Mat roi = get_face_roi(img);
    std::vector<float> feature = get_face_feature(roi);
    qint64 uuid = Utility::get_uuid();
    //写入索引文件
    VectorSearch::getInstance()->add_features({uuid}, feature.data());
    // 写入数据库
    StaffPtr staff;
    staff.reset(new Staff());
    staff->id = uuid;
    staff->uid = uid;
    staff->name = name;
    staff->picture = Utility::mat2ByteArray(img);
    staff->feature = Utility::vectorf2ByteArray(
        QVector<float>(feature.begin(), feature.end()));
    qx::dao::insert(staff);
}

void FaceHelper::delete_face(const QString &uid) {
    // 删除数据库
    StaffPtr staff(new Staff());
    qx_query query;
    query.where("uid").isEqualTo(uid);
    QSqlError sql_error = qx::dao::delete_by_query<Staff>(query);
    if(!sql_error.isValid()){
        // 删除图片
        // 更新索引
        refresh_faceindex();
    }
}

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

std::vector<float> FaceHelper::get_face_feature(const cv::Mat &img) {
    fastdeploy::vision::FaceRecognitionResult rec_result;
    face_rec_->Predict(img, &rec_result);
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
