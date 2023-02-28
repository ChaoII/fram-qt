#include "core/camerareader.h"
#include "common/utility.h"

CameraReaderThread::CameraReaderThread(QSize screen_size, int camera_index) {

    qDebug()<<screen_size;
    cap = new cv::VideoCapture(camera_index);
    cap->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    this->screen_size_ = screen_size;
}

void CameraReaderThread::run() {
    while (!QThread::currentThread()->isInterruptionRequested()) {
        cv::Mat frame;
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
