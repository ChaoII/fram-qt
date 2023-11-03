//
// Created by aichao on 2022/5/11.
//

#pragma once

#include <QWidget>
#include <QThread>
#include "core/SeetaFace.h"
#include "utils/Utils.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SeetaFaceThread; }
QT_END_NAMESPACE


class FaceDetThread : public QThread {
Q_OBJECT


public:
    explicit FaceDetThread(QObject *parent = nullptr);

    void stop_thread(); //线程关闭外部接口

    void close_detect();

    void open_detect();

    void run() override; //

    ~FaceDetThread() override;

signals:

    void img_send_signal(QImage, QRect);

private:
    bool is_detect = true;
    bool thread_start_ = true;   //线程开启标志
    std::shared_ptr<cv::VideoCapture> cap_ = nullptr;
};

