#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "SeetaFace.h"

namespace Ui {
class HistoryWidget;
}

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);
    ~HistoryWidget();

private:
    void update_table(int page);

private:
    Ui::HistoryWidget *ui;

    QStandardItemModel* model;

    int page_size_ = 30;
};

#endif // HISTORYWIDGET_H
