//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_SEETAFACETHREAD_H
#define FRAM_SEETAFACETHREAD_H

#include <QWidget>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "include/SeetaFace.h"
#include "include/Utils.h"
#include "include/facerecthread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SeetaFaceThread; }
QT_END_NAMESPACE


class SeetaFaceThread : public QThread {
Q_OBJECT


public:
    explicit SeetaFaceThread(QObject *parent = nullptr);

    void stop_thread(); //线程关闭外部接口

    void on_update_ret(FaceInfoWrap rec_info);

    void send_records(); // 发送打卡记录

    void run() override; //

    ~SeetaFaceThread() override;

signals:

    void img_send_signal(QImage);

    void face_rec_signal(FaceInfoWrap);

    void det_face_signal(bool detected = true);

    void attend_record_signal(QVector<FaceInfoWrap>);

private:

    bool _tstart;   //线程开启标志
    int _last_pid;  //人脸跟踪pid
    float _threshold;       // 人脸识别阈值
    int _record_interval;   // 打卡记录存储间隔s
    QDateTime _last_record_time;    // 上次打卡记录时间
    QVector<FaceInfoWrap> _records; // 打卡记录缓存
    std::shared_ptr<FaceRecThread> face_rec_ptr = nullptr;
    std::shared_ptr<SeetaFace> seetaface_ptr = nullptr;
    std::shared_ptr<cv::VideoCapture> cap = nullptr;
};


#endif //FRAM_SEETAFACETHREAD_H
