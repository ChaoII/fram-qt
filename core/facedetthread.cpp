//
// Created by aichao on 2022/5/11.
//

#include "facedetthread.h"


FaceDetThread::FaceDetThread(QObject *parent) : QObject(parent) {
    cap_ = std::make_shared<cv::VideoCapture>(0);
    cap_->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap_->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
}

void FaceDetThread::stop_thread() {
    thread_start_ = false;
}


void FaceDetThread::close_detector() {
    qDebug() << "face_det_thread...........";
    is_detect = false;
}

void FaceDetThread::open_detector() {
    is_detect = true;
}

FaceDetThread::~FaceDetThread() {
    if (cap_->isOpened()) {
        cap_->release();
    }
}

void FaceDetThread::run_detect() {
    qDebug() << "det:" << QThread::currentThreadId();
    cv::Mat frame_src;
    while (thread_start_) {
        if (cap_ && cap_->isOpened()) {
            qApp->processEvents();
            cap_->read(frame_src);
            if (frame_src.empty()) continue;
            // flip for horize
            cv::flip(frame_src, frame_src, 1);
            QRect rect;
            cv::Mat frame = Utils::crop_img(frame_src);
            if (is_detect) {
                auto faces = SeetaFace::getInstance()->face_detection(frame);
                if (!faces.empty()) {
                    //-----------发送检测到人脸的信号----------
                    rect = Utils::SRect2QRect(faces[0].pos);
                }
            }
            QImage q_img = Utils::CvMat2QImage(frame);
            emit img_send_signal(q_img, rect);
        }
    }
}



