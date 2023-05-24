#include "facerecognitionthread.h"

FaceRecognitionThread::FaceRecognitionThread(QObject *parent)
    : QObject{parent}
{

}
void FaceRecognitionThread::update_params(const cv::Mat &img, const SeetaRect &rect){
//    img_ = img;
//    rect_ = rect;
}

void FaceRecognitionThread::update_params(const QImage &img, const QRect &rect){
//    img_ = Utils::qimg_2_cvMat(img);
//    rect_ = Utils::QRect2SRect(rect);
}

void FaceRecognitionThread::face_recognition(const QImage &img, const QRect &rect)
{
    qDebug()<<"识别程id："<<QThread::currentThreadId();
    auto img_ = Utils::QImage2CvMat(img);
    auto rect_ = Utils::QRect2SRect(rect);
    auto points = SeetaFace::getInstance()->face_marker(img_, rect_);
    //人脸活体检测
    auto status = Status::REAL;
    status = SeetaFace::getInstance()->face_anti_spoofing(img_, rect_, points);
    if (status == Status::SPOOF) {
        FaceRecRet ret{"-1", "攻击人脸", 0.0};
        FaceInfoWrap info{-1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzzz"), ret};
        qCritical() << "攻击人脸";
        emit face_rec_signal(info);
    } else {
        //人脸识别
        auto ret = SeetaFace::getInstance()->face_recognition(img_, points);
//        if (ret.second < threshold_) {
//            FaceInfoWrap info{0, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), {}};
//            emit face_rec_signal(info);
//            qWarning() << "未知人脸: " << "score: " << qPrintable(QString::asprintf("%.2f", ret.second));
//        } else {
//            FaceInfoWrap info{1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), {}};
//            emit face_rec_signal(info);
//            qInfo() << "打卡成功 name: " << qPrintable(ret.name) << "score: "
//                    << qPrintable(QString::asprintf("%.2f", ret.score));
//        }
    }
}

