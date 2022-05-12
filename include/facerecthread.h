//
// Created by aichao on 2022/5/12.
//

#ifndef FRAM_FACERECTHREAD_H
#define FRAM_FACERECTHREAD_H


#include <QThread>
#include "include/struct.h"
#include "include/SeetaFace.h"
#include "include/Utils.h"
#include "include/SeetaFace.h"
#include "seeta/FaceTracker.h"
#include "seeta/FaceAntiSpoofing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FaceRecThread; }
QT_END_NAMESPACE

class FaceRecThread : public QThread {
Q_OBJECT

public:


    explicit FaceRecThread(std::shared_ptr<SeetaFace> seetaface_ptr);

    void init_params(const cv::Mat &frame, const SeetaTrackingFaceInfo &face_info);

    void run() override;

    ~FaceRecThread() override;


signals:

    void face_rec_signal(FaceInfoWrap);

private:
    float _threshold;
    cv::Mat _frame;
    std::shared_ptr<SeetaFace> _seetaface_ptr = nullptr;
    SeetaTrackingFaceInfo _face_info;
    Ui::FaceRecThread *ui;
};


#endif //FRAM_FACERECTHREAD_H
