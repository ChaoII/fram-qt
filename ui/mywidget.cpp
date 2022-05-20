//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWidget.h" resolved

#include "mywidget.h"
#include "ui_MyWidget.h"
#include <QPainter>
#include <QTimer>


MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {

    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    seeta_face_thread = new SeetaFaceThread(this);
    record_thread = new RecordThread(this);
    auto timer = new QTimer(this);


    connect(seeta_face_thread, &SeetaFaceThread::img_send_signal, this, &MyWidget::update_frame);
    connect(seeta_face_thread, &SeetaFaceThread::face_rec_signal, this, &MyWidget::on_face_rec);
    connect(seeta_face_thread, &SeetaFaceThread::attend_record_signal, this, &MyWidget::on_save_record);
    connect(seeta_face_thread, &SeetaFaceThread::det_face_signal, this, &MyWidget::on_det_face);
    connect(timer, &QTimer::timeout, this, &MyWidget::on_update_time);

    timer->setInterval(1000);
    timer->start();
}


MyWidget::~MyWidget() {

    std::cout << "----mywidget---" << std::endl;
    delete ui;
}

void MyWidget::update_frame(QImage qimg) {


    _img = qimg.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void MyWidget::on_face_rec(FaceInfoWrap rec_info) {

    QString attend_time = rec_info.time.split(" ")[1].split(".")[0];

    if (rec_info.code == -1) {
        ui->lb_name->setText("攻击人脸");
        ui->lb_attend_time->setText(attend_time);
        ui->lb_pic->setPixmap(QPixmap(":img/signin_fail.png"));
        ui->widget->setStyleSheet("#widget{background-color: rgba(255, 0, 0,40);}");
        ui->widget_info->setStyleSheet("#widget_info{background-color: rgba(255, 0, 0,0);}");

    } else if (rec_info.code == 0) {

        ui->lb_name->setText("未知");
        ui->lb_attend_time->setText(attend_time);
        ui->lb_pic->setPixmap(QPixmap(":img/signin_fail.png"));
        ui->widget->setStyleSheet("#widget{background-color: rgba(255, 0, 0,0);}");
        ui->widget_info->setStyleSheet("#widget_info{background-color: rgba(255, 0, 0,40);}");

    } else {
        ui->lb_name->setText(rec_info.ret.name);
        ui->lb_attend_time->setText(attend_time);
        ui->lb_pic->setPixmap(QPixmap(":img/signin_success.png"));
        ui->widget->setStyleSheet("#widget{background-color: rgba(255, 0, 0,0);}");
        ui->widget_info->setStyleSheet("#widget_info{background-color: rgba(0, 255, 0, 40);}");
    }

}

void MyWidget::on_det_face(bool detected) {
    ui->widget_info->setVisible(detected);
    if (!detected) {
        ui->widget->setStyleSheet("#widget{background-color: rgba(255, 0, 0,0);}");
    }
}

void MyWidget::on_save_record(QVector<FaceInfoWrap> records) {

    record_thread->update_info(records);
    if (!record_thread->isRunning()) {
        record_thread->start();
        qInfo() << "-------------- saved split line --------------------";
    }
}

void MyWidget::on_update_time() {
    ui->lb_cur_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MyWidget::closeEvent(QCloseEvent *event) {

    seeta_face_thread->stop_thread();
    seeta_face_thread->wait();
    seeta_face_thread->deleteLater();
}

void MyWidget::paintEvent(QPaintEvent *event) {
    if (_img.isNull()) return;
    QPainter painter(this);
    painter.drawImage(0, 0, _img);

}


void MyWidget::start_thread() {
    if (seeta_face_thread && !seeta_face_thread->isRunning()) {
        seeta_face_thread->start();
    }
}
