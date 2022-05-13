//
// Created by aichao on 2022/5/11.
//

#include "include/seetafacethread.h"


SeetaFaceThread::SeetaFaceThread() {

    _last_pid = -1;
    _threshold = 0.6;
    _last_record_time = QDateTime::currentDateTime();
    seetaface_ptr = std::make_shared<SeetaFace>();
    seetaface_ptr->create_face_libs("templates/static/facelib/facelib.json");
    face_rec_ptr = std::make_shared<FaceRecThread>(seetaface_ptr);

    connect(face_rec_ptr.get(), &FaceRecThread::face_rec_signal, this, &SeetaFaceThread::on_update_ret);

}

void SeetaFaceThread::run() {
    cv::Mat frame;
    cap = std::make_shared<cv::VideoCapture>(0);
    cap->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    while (t_start) {
        if (cap->isOpened()) {
            cap->read(frame);
            if (frame.empty()) continue;
            send_records();
            cv::Mat src_img = frame.clone();
            auto tracker_info = seetaface_ptr->face_track(frame);
            if (tracker_info.size > 0) {
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
            }
            cv::Mat dst = Utils::crop_img(frame);
            QImage q_img = Utils::cvMat_2_qimg(dst);
            img_send_signal(q_img);
        }
    }
}

void SeetaFaceThread::stop_thread() {
    t_start = false;
}

void SeetaFaceThread::on_update_ret(FaceInfoWrap rec_info) {

    face_rec_signal(rec_info);
    if (rec_info.code == 1) {
        _records.emplace_back(rec_info);
    }
}

void SeetaFaceThread::send_records() {

    QDateTime cur_time = QDateTime::currentDateTime();
    if (_last_record_time.secsTo(cur_time) > 5) {

        if (!_records.empty()) {
            attend_record_signal(_records);
            _records.clear();
        }
        _last_record_time = cur_time;
    }
}

SeetaFaceThread::~SeetaFaceThread() {

}


