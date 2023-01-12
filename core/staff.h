#ifndef STAFF_H
#define STAFF_H
#include <QtWidgets>
#include <QxOrm.h>


class Staff
{
public:
    qint64 id;
    QString uid;
    QString name;
    QByteArray picture;
    QByteArray feature;
};

QX_REGISTER_PRIMARY_KEY(Staff,qint64)
QX_REGISTER_HPP_EXPORT_DLL(Staff,qx::trait::no_base_class_defined, 0)



#endif // STAFF_H
