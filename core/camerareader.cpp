#include "core/camerareader.h"
#include "common/utility.h"

CameraReaderThread::CameraReaderThread(QSize screen_size, int camera_index) {

    cap = new cv::VideoCapture(camera_index);
    this->screen_size_ = screen_size;
}

void CameraReaderThread::run() {

    cv::Mat frame;
    while (!QThread::currentThread()->isInterruptionRequested()) {
        cap->read(frame);
        if (frame.empty()) {
            continue;
        }
        cv::Mat crop_img = Utility::fit_screen(frame, this->screen_size_);
        emit decodeimage(crop_img);
    }
    cap->release();
    delete cap;
    cap = nullptr;
}
