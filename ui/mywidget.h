//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_MYWIDGET_H
#define FRAM_MYWIDGET_H

#include <QWidget>
#include "include/Utils.h"
#include "ui/seetafacethread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget {
Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);

    void show_img(QImage qimg);

    ~MyWidget() override;

private:
    SeetaFaceThread *seeta_face_thread;
    Ui::MyWidget *ui;
};


#endif //FRAM_MYWIDGET_H
