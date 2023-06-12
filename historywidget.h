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
    Ui::HistoryWidget *ui;

    QStandardItemModel* model;
};

#endif // HISTORYWIDGET_H
