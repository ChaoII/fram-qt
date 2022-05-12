//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_MYWIDGET_H
#define FRAM_MYWIDGET_H

#include <QWidget>
#include "include/Utils.h"
#include "include/seetafacethread.h"
#include "include/recordthread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget {
Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);

    void update_frame(QImage qimg);

    void on_face_rec(FaceInfoWrap rec_info);

    void on_save_record(QVector<FaceInfoWrap> records);

    void paintEvent(QPaintEvent *event) override;

    ~MyWidget() override;

private:
    QImage _img;
    RecordThread *record_thread;
    SeetaTrackingFaceInfo *_face_info;
    SeetaFaceThread *seeta_face_thread;
    Ui::MyWidget *ui;
};


#endif //FRAM_MYWIDGET_H
