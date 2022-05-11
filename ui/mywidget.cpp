//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWidget.h" resolved

#include "mywidget.h"
#include "ui_MyWidget.h"
#include <QTimer>


MyWidget::MyWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);
    seeta_face_thread = new SeetaFaceThread();
    connect(seeta_face_thread, &SeetaFaceThread::img_send_signal, this, &MyWidget::show_img);
    seeta_face_thread->start();
}


MyWidget::~MyWidget() {
    delete ui;
}

void MyWidget::show_img(QImage qimg) {
    QPixmap pic = QPixmap::fromImage(qimg.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label->setPixmap(pic);

}
