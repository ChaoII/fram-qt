//
// Created by aichao on 2022/5/11.
//

#include "seetafacethread.h"
#include <QtConcurrent/QtConcurrent>


SeetaFaceThread::SeetaFaceThread(QObject *parent) : QThread(parent) {

    last_record_time_ = QDateTime::currentDateTime();
    SeetaFace::getInstance()->create_face_libs("templates/static/facelib/facelib.json");
    cap_ = std::make_shared<cv::VideoCapture>(0);
    cap_->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap_->set(cv::CAP_PROP_FRAME_HEIGHT, 480);

}

/*!
 * 函数核心方法，重载函数，线程函数，开启线程，读取摄像头帧进行人脸检测，跟踪，活体检测，识别
 */
void SeetaFaceThread::run() {
    cv::Mat frame;
    while (thread_start_) {
        if (cap_ && cap_->isOpened()) {
            cap_->read(frame);
            if (frame.empty()) continue;
            //发送打卡记录存储信号
            send_records();
            cv::Mat src_img = frame.clone();
            auto faces = SeetaFace::getInstance()->face_detection(frame);
            if (faces.size > 0) {
                //-----------发送检测到人脸的信号----------
                emit det_face_signal(true);
                auto face_info = faces.data[0];
                cv::rectangle(frame,
                              cv::Rect(face_info.pos.x, face_info.pos.y,
                                       face_info.pos.width, face_info.pos.height),
                                       cv::Scalar(255, 255, 0), 1, cv::LINE_AA);
                face_rec(src_img,face_info);
            }
            else {
                //----------发送人脸离开信号-------------
                det_face_signal(false);
            }
            cv::Mat dst = Utils::crop_img(frame);
            QImage q_img = Utils::cvMat_2_qimg(dst);
            // ----------发送图像去主线程------------
            img_send_signal(q_img);
        }
    }
}

void SeetaFaceThread::face_rec(cv::Mat &img, const SeetaFaceInfo &face_info){

    auto face_rect = face_info.pos;
    auto points = SeetaFace::getInstance()->face_marker(img, face_rect);
    //人脸活体检测
//    auto status = SeetaFace::getInstance()->face_anti_spoofing(img, face_rect, points);
    auto status = Status::REAL;
    if (status == Status::SPOOF) {
        FaceRecRet ret{"-1", "攻击人脸", 0.0};
        FaceInfoWrap info{-1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzzz"), ret};
        qCritical() << "攻击人脸";
        emit face_rec_signal(info);
    } else {
        //人脸识别
        auto ret = SeetaFace::getInstance()->face_recognition(img, points);
        if (ret.score < threshold_) {
            FaceInfoWrap info{0, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), ret};
            emit face_rec_signal(info);
            qWarning() << "未知人脸: " << "score: " << qPrintable(QString::asprintf("%.2f", ret.score));
        } else {
            FaceInfoWrap info{1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), ret};
            emit face_rec_signal(info);
            qInfo() << "打卡成功 name: " << qPrintable(ret.name) << "score: "
                    << qPrintable(QString::asprintf("%.2f", ret.score));
        }
    }
}

void SeetaFaceThread::stop_thread() {
    thread_start_ = false;
}

/*!
 * 更新人脸识别结果:设置全局人脸识别结果，当接收到人脸识别线程后计算的识别结果后，执行该方法：
 * 1、发送人脸识别结果去主线程
 * 2、然后将识别的结果更新到缓存中
 * @param rec_info
 */
void SeetaFaceThread::on_update_ret(FaceInfoWrap rec_info) {
    // 发送人懒识别信号
    emit face_rec_signal(rec_info);
    if (rec_info.code == 1) {
        records_.push_back(rec_info);
    }
}

/*!
 * 有条件的发送人脸识别记录，当当前时间和上一次记录的时间差大于5秒时，发送打卡记录
 */
void SeetaFaceThread::send_records() {

    QDateTime cur_time = QDateTime::currentDateTime();
    if (last_record_time_.secsTo(cur_time) > record_interval_) {

        if (!records_.empty()) {
            // --------------发送打卡记录去主线程，主线程通过开启新的线程去存储--------------
            emit attend_record_signal(records_);
            // 打卡机记录存储后清空缓存
            records_.clear();
        }
        last_record_time_ = cur_time;
    }
}

SeetaFaceThread::~SeetaFaceThread() {
    if (cap_->isOpened()){
        cap_->release();
    }
}



