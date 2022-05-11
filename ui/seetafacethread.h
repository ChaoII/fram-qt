//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_SEETAFACETHREAD_H
#define FRAM_SEETAFACETHREAD_H

#include <QWidget>
#include <QThread>
#include "include/Utils.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SeetaFaceThread; }
QT_END_NAMESPACE

class SeetaFaceThread : public QThread {
Q_OBJECT


public:
    explicit SeetaFaceThread();

    void stop_thread();

    virtual void run() override;

    ~SeetaFaceThread() override;

signals:

    void img_send_signal(QImage);

private:
    std::shared_ptr<cv::VideoCapture> cap = nullptr;
    bool t_start = true;
};


#endif //FRAM_SEETAFACETHREAD_H
