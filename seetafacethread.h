//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_SEETAFACETHREAD_H
#define FRAM_SEETAFACETHREAD_H

#include <QWidget>
#include <QThread>
#include "SeetaFace.h"
#include "Utils.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SeetaFaceThread; }
QT_END_NAMESPACE


class SeetaFaceThread : public QThread {
Q_OBJECT


public:
    explicit SeetaFaceThread(QObject *parent = nullptr);

    void stop_thread(); //线程关闭外部接口

    void on_update_ret(FaceInfoWrap rec_info);

    static void face_rec(cv::Mat &img, const SeetaRect &face_info);

    void send_records(); // 发送打卡记录

    void run() override; //

    ~SeetaFaceThread() override;

signals:

    void img_send_signal(QImage, QRect);

    void face_rec_signal(FaceInfoWrap);

    void det_face_signal(bool detected = true);

    void attend_record_signal(QVector<FaceInfoWrap>);

private:
    bool thread_start_ = true;   //线程开启标志
    float threshold_ = 0.6;       // 人脸识别阈值
    int record_interval_ = 60;   // 打卡记录存储间隔s
    QDateTime last_record_time_;    // 上次打卡记录时间
    QVector<FaceInfoWrap> records_; // 打卡记录缓存
    std::shared_ptr<cv::VideoCapture> cap_ = nullptr;
};


#endif //FRAM_SEETAFACETHREAD_H
