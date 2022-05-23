//
// Created by aichao on 2022/5/11.
//

#include "include/seetafacethread.h"


SeetaFaceThread::SeetaFaceThread(QObject *parent) : QThread(parent) {

    // do some initialized

    _tstart = true;
    _last_pid = -1;
    _threshold = 0.6;
    _record_interval = 60;
    _last_record_time = QDateTime::currentDateTime();
    seetaface_ptr = std::make_shared<SeetaFace>();
    seetaface_ptr->create_face_libs("templates/static/facelib/facelib.json");
    face_rec_ptr = std::make_shared<FaceRecThread>(seetaface_ptr, this);
    connect(face_rec_ptr.get(), &FaceRecThread::face_rec_signal, this, &SeetaFaceThread::on_update_ret);
    cap = std::make_shared<cv::VideoCapture>(0);
    cap->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, 480);

}

/*!
 * 函数核心方法，重载函数，线程函数，开启线程，读取摄像头帧进行人脸检测，跟踪，活体检测，识别
 */
void SeetaFaceThread::run() {
    cv::Mat frame;
    while (_tstart) {
        if (cap && cap->isOpened()) {
            cap->read(frame);
            if (frame.empty()) continue;
            //发送打卡记录存储信号
            send_records();
            cv::Mat src_img = frame.clone();
            auto tracker_info = seetaface_ptr->face_track(frame);

            if (tracker_info.size > 0) {
                //-----------发送检测到人脸的信号----------
                emit det_face_signal(true);
                auto face_info = tracker_info.data[0];
                cv::rectangle(frame,
                              cv::Rect(face_info.pos.x, face_info.pos.y, face_info.pos.width, face_info.pos.height),
                              cv::Scalar(255, 255, 0), 1, cv::LINE_AA);

                int cur_pid = tracker_info.data[0].PID;
                if (_last_pid != cur_pid) {
                    //执行人脸识别
                    face_rec_ptr->init_params(src_img, face_info);
                    if (!face_rec_ptr->isRunning()) {
                        face_rec_ptr->start();
                    }
                }
            } else {
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

void SeetaFaceThread::stop_thread() {
    _tstart = false;
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
        _records.emplace_back(rec_info);
    }
}

/*!
 * 有条件的发送人脸识别记录，当当前时间和上一次记录的时间差大于5秒时，发送打卡记录
 */
void SeetaFaceThread::send_records() {

    QDateTime cur_time = QDateTime::currentDateTime();
    if (_last_record_time.secsTo(cur_time) > _record_interval) {

        if (!_records.empty()) {
            // --------------发送打卡记录去主线程，主线程通过开启新的线程去存储--------------
            emit attend_record_signal(_records);
            // 打卡机记录存储后清空缓存
            _records.clear();
        }
        _last_record_time = cur_time;
    }
}

SeetaFaceThread::~SeetaFaceThread() {

    if (cap->isOpened()) cap->release();
    face_rec_ptr->quit();
    face_rec_ptr->wait();
    face_rec_ptr->deleteLater();
    std::cout << "---seetafacethread--" << std::endl;

}


