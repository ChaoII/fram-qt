//
// Created by aichao on 2022/5/13.
//

#ifndef FRAM_MAINWIDGET_H
#define FRAM_MAINWIDGET_H


#include <QWidget>
#include "mywidget.h"

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

private:

    MyWidget *attend_widget = nullptr;
    Ui::MainWidget *ui;
};


#endif //FRAM_MAINWIDGET_H
