#pragma once

#include <QWidget>

namespace Ui {
    class AttendInfoItemWidget;
}

class AttendInfoItemWidget : public QWidget {
Q_OBJECT

public:
    explicit AttendInfoItemWidget(QWidget *parent = nullptr);

    void setItem(const QString &number,
                 const QString &name,
                 const QString &staffId,
                 const QString &attendTime,
                 const QString &pictureUrl);

    ~AttendInfoItemWidget();

private:
    Ui::AttendInfoItemWidget *ui;
};

