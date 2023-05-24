#ifndef MODELS_H
#define MODELS_H
#include <QxOrm.h>

class Staff{
public:
    qint64 id;
    QString uid;
    QString name;
    QString pic_url;
    QByteArray feature;
};

QX_REGISTER_PRIMARY_KEY(Staff,qint64)
QX_REGISTER_HPP_EXPORT_DLL(Staff,qx::trait::no_base_class_defined,0)
typedef std::shared_ptr<Staff> StaffPtr;

class Attend{
public:
    long id;
    QString uid;
    QString name;
    QString pic_url;
    QDateTime attend_time;
};
QX_REGISTER_HPP_EXPORT_DLL(Attend,qx::trait::no_base_class_defined,0)






#endif // MODELS_H
