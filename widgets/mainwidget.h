//
// Created by aichao on 2022/5/13.
//

#pragma once


#include <QWidget>
#include "mywidget.h"
#include "registerwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget {
Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

    ~MainWidget() override;

private slots:

    void on_attend_clicked();

    void on_setting_clicked();

private:

    MyWidget *attend_widget = nullptr;

    Ui::MainWidget *ui;
};


