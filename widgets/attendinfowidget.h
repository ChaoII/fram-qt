#ifndef ATTENDINFOWIDGET_H
#define ATTENDINFOWIDGET_H

#include <QWidget>

namespace Ui {
    class AttendInfoWidget;
}

class AttendInfoWidget : public QWidget {
Q_OBJECT

public:
    explicit AttendInfoWidget(QWidget *parent = nullptr);

    ~AttendInfoWidget();

    void
    setAttendInfo(const QString &name,
                  const QString &time,
                  const QPixmap &picAttendStatus,
                  const QPixmap &picLibrary,
                  const QPixmap &picCurrent);


private:
    Ui::AttendInfoWidget *ui;
};

#endif // ATTENDINFOWIDGET_H
