//
// Created by aichao on 2022/5/11.
//

#include "SeetaFace.h"
#include "models.h"

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
    init_face_db();
    init_file_dir();
}

void SeetaFace::init_file_dir()
{
    QDir dir;

    if(!dir.exists(QDir::currentPath()+"/face")){
        dir.mkdir(QDir::currentPath()+"/face");
    };
    if(!dir.exists(QDir::currentPath()+"/attend")){
        dir.mkdir(QDir::currentPath()+"/attend");
    };
}

void SeetaFace::init_face_db()
{
    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName("data.db");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(false);
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(false);
    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlBoundValues(false);

    qx::QxSqlDatabase::getSingleton()->setTraceSqlBoundValuesOnError(false);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(false);





    QSqlError staff_error = qx::dao::create_table<Staff>();
    if(staff_error.isValid()){
        qDebug()<<staff_error.text();
    }

    QSqlError attend_error = qx::dao::create_table<Attend>();
    if(attend_error.isValid()){
        qDebug()<<attend_error.text();
    }

    QFileInfo fileInfo(INDEX_FILE);
    if(!fileInfo.exists()){
        build_face_index_from_db();
    }
}

bool SeetaFace::add_face(cv::Mat &img, const QString &uid, const QString &name)
{
    unique_ptr<float[]> feature(new float[FR->GetExtractFeatureSize()]);
    SeetaFace::getInstance()->extract_feature(img, feature.get());
    qint64 uuid = Utils::get_uuid();
    QDir dir(QDir::currentPath()+"/face/"+uid);
    if(!dir.exists()){
        dir.mkdir(dir.absolutePath());
    }
    QString file_path = dir.absoluteFilePath(QString::number(uuid)+".jpg");
    qDebug()<<file_path;
    Staff staff;
    staff.index_id = uuid;
    staff.uid = uid;
    staff.name = name;
    staff.pic_url = file_path;
    staff.feature = Utils::floatArray2QByteArray(feature.get(), FR->GetExtractFeatureSize());
    auto sql_error = qx::dao::insert(staff);
    if(sql_error.isValid()){
        qDebug()<<"新增人脸失败，name:"<<name<<"uid:"<<uid<<"失败原因："<<sql_error.text();
        return false;
    }
    cv::imwrite(file_path.toStdString(), img);
    VectorSearch::getInstance()->add_features({uuid}, feature.get());
    qDebug()<<"新增人脸成功，name:"<<name<<"uid:"<<uid;
    return true;
}

bool SeetaFace::delete_face(const QString &uid)
{
    Staff staff;
    qx_query query;
    query.where("uid").isEqualTo(uid);
    QSqlError sql_error = qx::dao::delete_by_query<Staff>(query);
    if(sql_error.isValid()){
        qDebug()<<"删除人脸失败，失败原因"<<sql_error.text();
        return false;
    }
    qDebug()<<"删除人脸成功，uid:"<<uid;
    return true;
}

void SeetaFace::build_face_index_from_db()
{
    std::vector<Staff> staffs;
    QSqlError sql_error = qx::dao::fetch_all(staffs);
    if(sql_error.isValid()){
        return ;
    }
    std::vector<int64_t> ids;
    std::vector<float> features;
    for (auto &staff: staffs){
        ids.push_back(staff.index_id);
        auto feature = Utils::QByteArray2Vector<float>(staff.feature);
        features.insert(features.end(),feature.begin(),feature.end());
    }
    VectorSearch::getInstance()->add_features(ids,features.data());
}

QString SeetaFace::get_facepath_from_index_id(qint64 index_id){
    Staff staff;
    qx_query query;
    query.where("index_id").isEqualTo(index_id);
    QSqlError sql_error = qx::dao::fetch_by_query<Staff>(query,staff);
    if(sql_error.isValid()){
        qDebug()<<"file is not existed";
        return "";
    }
    return staff.pic_url;
}

Staff SeetaFace::get_faceinfo_from_index_id(qint64 index_id)
{
    Staff staff;
    qx_query query;
    query.where("index_id").isEqualTo(index_id);
    QSqlError sql_error = qx::dao::fetch_by_query<Staff>(query,staff);
    if(sql_error.isValid()){
        return {};
    }
    std::vector feature = Utils::QByteArray2Vector<float>(staff.feature);
    return staff;
}

bool SeetaFace::update_face(cv::Mat &img, const QString &uid, const QString &name)
{
    Staff staff;
    staff.uid=uid;
    staff.name=name;
    qx_query query;
    unique_ptr<float[]> feature(new float[FR->GetExtractFeatureSize()]);
    SeetaFace::getInstance()->extract_feature(img, feature.get());
    staff.feature = Utils::floatArray2QByteArray(feature.get(), FR->GetExtractFeatureSize());
    query.where("uid").isEqualTo(uid);
    auto sql_error = qx::dao::update_by_query<Staff>(query,staff);
    if(sql_error.isValid()){
        qDebug()<<"更新人脸失败，失败原因"<<sql_error.text();
        return false;
    }
    qDebug()<<"更新人脸成功，name:"<<name<<"uid:"<<uid;
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

QPair<int64_t, float> SeetaFace::face_recognition(cv::Mat &img,
                                                 std::vector<SeetaPointF> points) {
    SeetaImageData data = Utils::CvMat2Simg(img);
    unique_ptr<float[]> feature(new float[FR->GetExtractFeatureSize()]);
    FR->Extract(data, points.data(), feature.get());
    SearchResult result = VectorSearch::getInstance()->search(feature.get(), 3);
    return {result.I[0],result.D[0]};
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

QVector<Attend> SeetaFace::get_attend_info(int row_num_pre_page, int current_page_index)
{
    QVector<Attend> attends;
    qx_query query;
    int start_row = current_page_index * row_num_pre_page;
    query.orderDesc("attend_time").limit(row_num_pre_page,start_row);
    QSqlError sql_error= qx::dao::fetch_by_query(query,attends);
    if(sql_error.isValid()){
        qDebug()<<"select data error ,details:"<<sql_error.text();
    }

    return attends;
}

int SeetaFace::get_attend_num()
{
    qx_query query;
    int ret = qx::dao::count<Attend>(query);
    return ret;
}



