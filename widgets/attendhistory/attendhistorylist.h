#pragma once

#include <QWidget>
#include "attendinfoitemwidget.h"
#include "attendhistorylistheader.h"

namespace Ui {
    class AttendHistoryList;
}

class AttendHistoryList : public QWidget {
Q_OBJECT
private:
    void deleteLayoutWidget();

public:
    explicit AttendHistoryList(QWidget *parent = nullptr);

    void addItemWidgets(const QVector<AttendInfoItemWidget *> &attendInfoItemWidgets);

    ~AttendHistoryList();

private:
    Ui::AttendHistoryList *ui;
};

