//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_MYWIDGET_H
#define FRAM_MYWIDGET_H

#include <QWidget>
#include "Utils.h"
#include "facerecognitionthread.h"
#include "seetafacethread.h"
#include "recordthread.h"
#include <QMutex>
#include <QMutexLocker>


QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE


class MyWidget : public QWidget {
Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);

    void update_frame(QImage qimg,QRect rect);

    void on_face_rec(FaceInfoWrap rec_info);

    void on_save_record(QVector<FaceInfoWrap> records);

    void on_det_face(bool detected);

    void on_update_time();

    void start_thread();

    void closeEvent(QCloseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    ~MyWidget() override;

signals:
    void send_img_signal(QImage qimg,QRect rect);
private:
    QImage _img;
    QDateTime last_rec_time = QDateTime::currentDateTime();
    QThread worker_thread;
    RecordThread *record_thread = nullptr;
    SeetaFaceThread *seeta_face_thread = nullptr;
    FaceRecognitionThread* face_recognition_thread = nullptr;
    Ui::MyWidget *ui;
};


#endif //FRAM_MYWIDGET_H
