//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SeetaFaceThread.h" resolved

#include "seetafacethread.h"


SeetaFaceThread::SeetaFaceThread() {


}

void SeetaFaceThread::run() {

    cv::Mat frame;
    cap = std::make_shared<cv::VideoCapture>(0);
    while (t_start) {
        if (cap->isOpened()) {
            cap->read(frame);
            cv::Mat dst = Utils::crop_img(frame);
            QImage q_img = Utils::cvMat_2_qimg(dst);
            img_send_signal(q_img);
        }
    }
}

void SeetaFaceThread::stop_thread() {
    t_start = false;
}


SeetaFaceThread::~SeetaFaceThread() {

}
