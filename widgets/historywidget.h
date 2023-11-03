#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "core/SeetaFace.h"
#include "customwidget/pagingwidget.h"


namespace Ui {
    class HistoryWidget;
}

class HistoryWidget : public QWidget {
Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);

    void update_table(int page);

    ~HistoryWidget();

signals:

    void history_back_signal();

private slots:

    void on_pb_back_clicked();

private:
    Ui::HistoryWidget *ui;

    QStandardItemModel *the_model;

    QItemSelectionModel *the_select;

    int page_size_ = 9;

    PagingWidget *paging = nullptr;
};


