//
// Created by aichao on 2022/5/12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FaceRecThread.h" resolved

#include "include/facerecthread.h"


FaceRecThread::FaceRecThread(std::shared_ptr<SeetaFace> seetaface_ptr) {

    _seetaface_ptr = seetaface_ptr;
    _threshold = 0.6;

}

void FaceRecThread::init_params(const cv::Mat &frame, const SeetaTrackingFaceInfo &face_info) {

    _frame = frame;
    _face_info = face_info;
}

void FaceRecThread::run() {

    SeetaRect face_rect = _face_info.pos;
    auto points = _seetaface_ptr->face_marker(_frame, face_rect);
    //人脸活体检测
    auto status = _seetaface_ptr->face_anti_spoofing(_frame, face_rect, points);

    if (status == Status::SPOOF) {
        FaceRecRet ret{"-1", "攻击人脸", 0.0};
        FaceInfoWrap info{-1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzzz"), ret};
        face_rec_signal(info);
    } else {
        //人脸识别
        auto ret = _seetaface_ptr->face_recognition(_frame, points);
        if (ret.score < _threshold) {
            FaceInfoWrap info{0, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), ret};
            face_rec_signal(info);
        } else {
            FaceInfoWrap info{1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), ret};
            face_rec_signal(info);
        }
    }
}


FaceRecThread::~FaceRecThread() {


}
