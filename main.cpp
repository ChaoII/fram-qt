#include "ui/widget/widget.h"
#include "plugin/plugin.h"
#include <QApplication>

#pragma warning(disable:4996)

int main(int argc, char *argv[])
{
    // close this code for test
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    //注册orm
    Plugin::register_orm();
    Plugin::register_facehelper();
    Plugin::register_vectorhelper();
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
