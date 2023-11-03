//
// Created by aichao on 2022/5/13.
//

#include <QMetaObject>
#include "mainwidget.h"
#include "ui_mainwidget.h"


MainWidget::MainWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose);
    QMetaObject::connectSlotsByName(this);
    attend_widget = new MyWidget();
}

void MainWidget::on_attend_clicked() {

    attend_widget->show();
    attend_widget->run();
    hide();
}

void MainWidget::on_setting_clicked() {

}

MainWidget::~MainWidget() {
    std::cout << "mainwidget" << std::endl;
    if (attend_widget != nullptr) {
        delete attend_widget;
        attend_widget = nullptr;
    }
    delete ui;
}




