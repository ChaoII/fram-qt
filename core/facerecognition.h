#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <QMap>
#include <QThread>
#include "common/config.h"
#include <opencv2/opencv.hpp>

class FaceRecognitinThread : public QThread {
    Q_OBJECT
public:
    explicit FaceRecognitinThread() = default;
    ~FaceRecognitinThread() = default;
    void set_frame(cv::Mat mat);
    void run() override;
signals:
    void rec_result(const FaceResult&);

private:
    cv::Mat cur_frame_;
};

#endif // FACERECOGNITION_H
