//
// Created by aichao on 2022/5/11.
//

#include "facedetthread.h"


FaceDetThread::FaceDetThread(QObject *parent) : QObject(parent) {
#ifdef __linux__
    QString v4l2_pipeline;
    if (Config::getInstance().get_cameraType() == Config::CameraType::MIPI) {
        v4l2_pipeline = QString(
                "v4l2src device=/dev/video%1 ! video/x-raw, format=NV12, width=%2, height=%3, framerate=30/1 ! appsink")
                .arg(Config::getInstance().get_cameraIndex())
                .arg(Config::getInstance().get_frameWidth())
                .arg(Config::getInstance().get_frameHeight());
    } else if (Config::getInstance().get_cameraType() == Config::CameraType::USB) {
        v4l2_pipeline = QString(
                "v4l2src device=/dev/video%1 ! image/jpeg, width=%2, height=%3, framerate=30/1 ! mppjpegdec ! videoconvert ! appsink")
                .arg(Config::getInstance().get_cameraIndex())
                .arg(Config::getInstance().get_frameWidth())
                .arg(Config::getInstance().get_frameHeight());
    }
    qDebug() << v4l2_pipeline;
    cap_ = std::make_shared<cv::VideoCapture>(v4l2_pipeline.toStdString(), cv::CAP_GSTREAMER);
#else
    cap_ = std::make_shared<cv::VideoCapture>(Config::getInstance().get_cameraIndex());
    cap_->set(cv::CAP_PROP_FRAME_WIDTH, Config::getInstance().get_frameWidth());
    cap_->set(cv::CAP_PROP_FRAME_HEIGHT, Config::getInstance().get_frameHeight());
#endif
}

void FaceDetThread::stopThread() {
    qDebug() << "= stop decode";
    thread_start_ = false;
}


void FaceDetThread::closeDetector() {
    qDebug() << "= close face detector";
    is_detect = false;
}

void FaceDetThread::openDetector() {
    qDebug() << "= open face detector";
    is_detect = true;
}

FaceDetThread::~FaceDetThread() {
    if (cap_->isOpened()) {
        cap_->release();
    }
    qDebug() << "= destroy FaceDetThread, thread id:" << QThread::currentThreadId();
}

void FaceDetThread::runDetect() {
    qDebug() << "= enter decode thread, thread id is: " << QThread::currentThreadId();
    cv::Mat frame_src;
    while (thread_start_) {
        qApp->processEvents();
        if (cap_ && cap_->isOpened()) {
            QThread::msleep(20);
            cap_->read(frame_src);
#ifdef __linux__
            if (Config::getInstance().get_cameraType() == Config::CameraType::MIPI) {
                cv::cvtColor(frame_src, frame_src, cv::COLOR_YUV2BGR_NV12);
            }
#endif
            if (frame_src.empty()) continue;
            // flip for horizontal
            cv::flip(frame_src, frame_src, 1);
            QRect rect;
            cv::Mat frame = utils::cropImg(frame_src);
            if (is_detect) {
                auto faces = SeetaFace::getInstance().faceDetection(frame);
                if (!faces.empty()) {
                    //-----------发送检测到人脸的信号----------
                    rect = utils::sRectToQRect(faces[0].pos);
                }
            }
            QImage q_img = utils::cvMat2QImage(frame);
            emit imgSendSignal(q_img, rect);
        }
    }
}

bool FaceDetThread::getDetectorStatus() {
    return is_detect;
}



