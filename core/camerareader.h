#ifndef CAMERAREADER_H
#define CAMERAREADER_H

#include <QtWidgets>
#include <opencv2/opencv.hpp>

class CameraReaderThread : public QThread {
    Q_OBJECT
public:
    explicit CameraReaderThread(QSize screen_size, int camera_index=0);
    void run() override;
signals:
    void decodeimage(cv::Mat&);

private:
    cv::VideoCapture *cap =nullptr;
    QSize screen_size_;
};

#endif // CAMERAREADER_H
