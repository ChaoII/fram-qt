#ifndef ATTEND_H
#define ATTEND_H


#include <QtWidgets>
#include <QxOrm.h>

///
/// \brief 打卡信息表
///
class Attend
{
public:
    long id;
    QString uid;
    QString name;
    QDateTime attend_time;
};

QX_REGISTER_HPP_EXPORT_DLL(Attend, qx::trait::no_base_class_defined, 0)

#endif // ATTEND_H
