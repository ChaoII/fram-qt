//
// Created by aichao on 2022/5/11.
//

#include "core/SeetaFace.h"
#include "models/models.h"

using namespace std;
using namespace seeta;

SeetaFace::SeetaFace() {

    std::string model_dir = Config::getInstance()->get_model_dir().toStdString();
    int face_recognition_thread_num = Config::getInstance()->get_face_recognition_thread_num();
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
    FD->set(seeta::FaceDetector::PROPERTY_NUMBER_THREADS,face_recognition_thread_num);
    // unsupported set thread number
    FL = std::make_shared<FaceLandmarker>(pd_setting);
    FR = std::make_shared<FaceRecognizer>(fr_setting);
    FR->set(seeta::FaceRecognizer::PROPERTY_NUMBER_THREADS,face_recognition_thread_num);
    FS = std::make_shared<FaceAntiSpoofing>(fs_setting);
    FS->set(seeta::FaceAntiSpoofing::PROPERTY_NUMBER_THREADS,face_recognition_thread_num);
    init_face_db();
    init_file_dir();
}

void SeetaFace::init_file_dir()
{
    QDir dir;

    if(!dir.exists(QDir::currentPath()+"/faces")){
        dir.mkdir(QDir::currentPath()+"/faces");
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

    QFileInfo fileInfo(Config::getInstance()->getIndex_file().toStdString().c_str());
    if(!fileInfo.exists()){
        build_face_index_from_db();
    }
}

bool SeetaFace::add_face(cv::Mat &img, const QString &uid, const QString &name)
{
    unique_ptr<float[]> feature(new float[FR->GetExtractFeatureSize()]);
    bool ret= SeetaFace::getInstance()->extract_feature(img, feature.get());
    if(!ret) return false;
    qint64 uuid = Utils::get_uuid();
    QDir dir(QDir::currentPath()+"/faces/"+uid);
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
    staff.register_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
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
    QSqlError sql_error = qx::dao::fetch_by_query<Staff>(query, staff);
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
        return a.pos.width > b.pos.width;
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

bool SeetaFace::delete_face_by_ids(const std::vector<int64_t>& ids)
{
    QVariantList variant_ids;
    for(auto &id: ids){
       variant_ids.append(QVariant::fromValue(id));
    }
    qx_query query;
    query.where("index_id").in(variant_ids);
    QVector<Staff> staffs;
    qx::dao::fetch_by_query(query, staffs);
    for(auto& staff: staffs){
        QFileInfo file_info(staff.pic_url);
        if(file_info.exists()){
            file_info.dir().remove(file_info.fileName());
            if(file_info.dir().entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot).count()<=0){
                file_info.dir().removeRecursively();
                qDebug()<<"清空文件夹:"<<file_info.dir().absolutePath();
            }
            qDebug()<<staff.uid<<staff.name<<file_info.fileName();
        }
    }
    QSqlError sql_error = qx::dao::delete_by_query<Staff>(query);
    if(sql_error.isValid()){
        qDebug()<<"delete data error , details:"<<sql_error.text();
        return false;
    }
    size_t nums = VectorSearch::getInstance()->remove_index(ids);
    if(nums != ids.size()){
        qDebug()<<"delete failed";
        return false;
    }
    return true;
}





