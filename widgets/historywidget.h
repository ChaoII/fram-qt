#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "core/SeetaFace.h"


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

    QStandardItemModel* the_model;

    QItemSelectionModel* the_select;

    int page_size_ = 9;
};


