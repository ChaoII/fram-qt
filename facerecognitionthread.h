#ifndef FACERECOGNITIONTHREAD_H
#define FACERECOGNITIONTHREAD_H

#include <QThread>
#include "SeetaFace.h"

using Status = seeta::FaceAntiSpoofing::Status;

class FaceRecognitionThread : public QObject
{
    Q_OBJECT
public:
    explicit FaceRecognitionThread(QObject *parent = nullptr);

    void update_params(const cv::Mat &img, const SeetaRect &rect);

    void update_params(const QImage &img, const QRect &rect);

signals:
    void face_rec_signal(FaceInfoWrap);

public slots:
    void face_recognition(const QImage &img, const QRect &rect);

private:
    float threshold_ = 0.6;

};

#endif // FACERECOGNITIONTHREAD_H
