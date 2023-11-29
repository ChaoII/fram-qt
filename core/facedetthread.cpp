//
// Created by aichao on 2022/5/11.
//

#include "facedetthread.h"


FaceDetThread::FaceDetThread(QObject *parent) : QObject(parent) {

#ifdef __LINUX__
    cap_ = std::make_shared<cv::VideoCapture>("v4l2src device=/dev/video11 ! video/x-raw,format=NV12,width=640,"
                                             "height=480,framerate=30/1 ! appsink",cv::CAP_GSTREAMER);
#else
    cap_ = std::make_shared<cv::VideoCapture>(Config::getInstance().get_camera_index());
    cap_->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap_->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
#endif
}

void FaceDetThread::stop_thread() {
    qDebug() << "= stop decode";
    thread_start_ = false;
}


void FaceDetThread::close_detector() {
    qDebug() << "= close face detector";
    is_detect = false;
}

void FaceDetThread::open_detector() {
    qDebug() << "= open face detector";
    is_detect = true;
}

FaceDetThread::~FaceDetThread() {
    if (cap_->isOpened()) {
        cap_->release();
    }
    qDebug() << "= destroy FaceDetThread, thread id:" << QThread::currentThreadId();
}

void FaceDetThread::run_detect() {
    qDebug() << "= enter decode thread, thread id is: " << QThread::currentThreadId();
    cv::Mat frame_src;
    while (thread_start_) {
        qApp->processEvents();
        if (cap_ && cap_->isOpened()) {
            QThread::msleep(20);
            cap_->read(frame_src);
#ifdef __LINUX__
            cv::cvtColor(frame_src, frame_src, cv::COLOR_YUV2BGR_NV12);
#endif
            if (frame_src.empty()) continue;
            // flip for horizontal
            cv::flip(frame_src, frame_src, 1);
            QRect rect;
            cv::Mat frame = Utils::crop_img(frame_src);
            if (is_detect) {
                auto faces = SeetaFace::getInstance().face_detection(frame);
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



