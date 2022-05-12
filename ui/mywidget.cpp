//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWidget.h" resolved

#include "mywidget.h"
#include "ui_MyWidget.h"
#include <QPainter>


MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);
    _face_info = nullptr;
    seeta_face_thread = new SeetaFaceThread();
    record_thread = new RecordThread();
    connect(seeta_face_thread, &SeetaFaceThread::img_send_signal, this, &MyWidget::update_frame,
            Qt::DirectConnection);
    connect(seeta_face_thread, &SeetaFaceThread::face_rec_signal, this, &MyWidget::on_face_rec);

    connect(seeta_face_thread, &SeetaFaceThread::attend_record_signal, this, &MyWidget::on_save_record);
    seeta_face_thread->start();
}


MyWidget::~MyWidget() {
    delete ui;
}

void MyWidget::update_frame(QImage qimg) {
    _img = qimg.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void MyWidget::on_face_rec(FaceInfoWrap rec_info) {

    if (rec_info.code == -1) {
        qDebug() << "攻击人脸";
    } else if (rec_info.code == 0) {

        qDebug() << "unknown";
    } else {
//        qDebug() << rec_info.time;
//        qDebug() << rec_info.ret.name;
//        qDebug() << rec_info.ret.score;
    }
}

void MyWidget::on_save_record(QVector<FaceInfoWrap> records) {

    record_thread->update_info(records);
    if (!record_thread->isRunning()) {
        record_thread->start();
        qDebug() << "---------------------------";
    }
}

void MyWidget::paintEvent(QPaintEvent *event) {
    if (_img.isNull()) return;
    QPainter painter(this);
    painter.drawImage(0, 0, _img);
}


