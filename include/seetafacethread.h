//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_SEETAFACETHREAD_H
#define FRAM_SEETAFACETHREAD_H

#include <QWidget>
#include <QThread>
#include "include/SeetaFace.h"
#include "include/Utils.h"
#include "include/facerecthread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SeetaFaceThread; }
QT_END_NAMESPACE

class SeetaFaceThread : public QThread {
Q_OBJECT


public:
    explicit SeetaFaceThread();

    void stop_thread();

    void on_update_ret(FaceInfoWrap rec_info);

    void send_records();

    void run() override;

    ~SeetaFaceThread() override;

signals:

    void img_send_signal(QImage);

    void face_rec_signal(FaceInfoWrap);

    void det_face_signal(bool);

    void attend_record_signal(QVector<FaceInfoWrap>);

private:

    int _last_pid;
    float _threshold;
    QDateTime _last_record_time;
    QVector<FaceInfoWrap> _records;
    std::shared_ptr<FaceRecThread> face_rec_ptr = nullptr;
    std::shared_ptr<SeetaFace> seetaface_ptr = nullptr;
    std::shared_ptr<cv::VideoCapture> cap = nullptr;
    bool t_start = true;
};


#endif //FRAM_SEETAFACETHREAD_H
