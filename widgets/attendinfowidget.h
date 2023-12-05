#pragma once

#include <QWidget>

namespace Ui {
    class AttendInfoWidget;
}

class AttendInfoWidget : public QWidget {
Q_OBJECT

public:
    explicit AttendInfoWidget(QWidget *parent = nullptr);

    ~AttendInfoWidget();

    void setAttendInfo(const QString &name,
                       const QString &time,
                       const QPixmap &pic_attend_status,
                       const QPixmap &pic_library,
                       const QPixmap &pic_current);

private:
    Ui::AttendInfoWidget *ui;
};
