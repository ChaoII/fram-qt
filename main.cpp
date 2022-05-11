//
// Created by aichao on 2022/5/11.
//

#include <QApplication>
#include "ui/mywidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto widget = MyWidget();
    widget.show();
    return QApplication::exec();
}
