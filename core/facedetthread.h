//
// Created by aichao on 2022/5/11.
//

#pragma once

#include <QWidget>
#include "core/SeetaFace.h"
#include "utils/utils.h"


class FaceDetThread : public QObject {
Q_OBJECT

public:
    explicit FaceDetThread(QObject *parent = nullptr);

    ~FaceDetThread() override;

signals:

    void img_send_signal(QImage, QRect);

public slots:

    ///运行检测线程，包括解码+人脸检测
    void run_detect();

    ///停止检测线程，停止解码+人脸检测
    void stop_thread();

    ///关闭人脸检测器仅做视频解码
    void close_detector();

    ///开启人脸检测器，解码+人脸检测
    void open_detector();


private:
    bool is_detect = true;
    bool thread_start_ = true;   //线程开启标志
    std::shared_ptr<cv::VideoCapture> cap_ = nullptr;
};

