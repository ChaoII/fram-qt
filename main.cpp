#include "ui/widget/widget.h"
#include "plugin/plugin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    //注册orm
    Plugin::register_orm();
    Plugin::register_facehelper();
    Plugin::register_vectorhelper();
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
