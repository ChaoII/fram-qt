//
// Created by aichao on 2022/5/11.
//

#include "facedetthread.h"


FaceDetThread::FaceDetThread(QObject *parent) : QThread(parent) {

    cap_ = std::make_shared<cv::VideoCapture>(0);
    cap_->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap_->set(cv::CAP_PROP_FRAME_HEIGHT, 480);

}

/*!
 * 函数核心方法，重载函数，线程函数，开启线程，读取摄像头帧进行人脸检测，跟踪，活体检测，识别
 */
void FaceDetThread::run() {
    cv::Mat frame_src;
    while (thread_start_) {
        if (cap_ && cap_->isOpened()) {
            cap_->read(frame_src);
            if (frame_src.empty()) continue;
            // flip for horize
            cv::flip(frame_src,frame_src, 1);
            //发送打卡记录存储信号
            QRect rect;
            cv::Mat frame = Utils::crop_img(frame_src);
            if(is_detect){
                auto faces = SeetaFace::getInstance()->face_detection(frame);
                if (faces.size() > 0) {
                    //-----------发送检测到人脸的信号----------
                    rect = Utils::SRect2QRect(faces[0].pos);
                }
            }
            QImage q_img = Utils::CvMat2QImage(frame);
            emit img_send_signal(q_img,rect);
        }
    }
}


void FaceDetThread::stop_thread() {
    thread_start_ = false;
}


void FaceDetThread::close_detect()
{
    is_detect = false;
}

void FaceDetThread::open_detect()
{
    is_detect = true;
}

FaceDetThread::~FaceDetThread() {
    if (cap_->isOpened()){
        cap_->release();
    }
}



