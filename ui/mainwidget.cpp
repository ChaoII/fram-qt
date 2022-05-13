//
// Created by aichao on 2022/5/13.
//

#include <QMetaObject>
#include "mainwidget.h"
#include "ui_MainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    QMetaObject::connectSlotsByName(this);

    attend_widget = new MyWidget();

}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::on_attend_clicked() {

    attend_widget->show();

}
