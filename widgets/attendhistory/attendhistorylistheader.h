#pragma once

#include <QWidget>

namespace Ui {
    class AttendHistoryListHeader;
}

class AttendHistoryListHeader : public QWidget {
Q_OBJECT

public:
    explicit AttendHistoryListHeader(QWidget *parent = nullptr);

    ~AttendHistoryListHeader();

private:
    Ui::AttendHistoryListHeader *ui;
};

