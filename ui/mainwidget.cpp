//
// Created by aichao on 2022/5/13.
//

#include <QMetaObject>
#include "mainwidget.h"
#include "ui_MainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose);
    QMetaObject::connectSlotsByName(this);
    attend_widget = new MyWidget();
}

void MainWidget::on_attend_clicked() {

    attend_widget->show();
//    attend_widget->start_thread();
    this->hide();
}

MainWidget::~MainWidget() {

    std::cout << "mainwidget" << std::endl;
    delete ui;
}


