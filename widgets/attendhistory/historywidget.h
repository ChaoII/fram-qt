#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "core/SeetaFace.h"
#include "customwidget/pagingwidget.h"
#include "widgets/attendhistory/attendhistorylist.h"


namespace Ui {
    class HistoryWidget;
}

class HistoryWidget : public QWidget {
Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);

    void updateHistoryWidget();

    ~HistoryWidget() override;

signals:

    void historyBackSignal();

private:

    void updateTable(int page);

    void updatePagingAndTable();

private slots:

    void on_tb_back_clicked();

private:
    Ui::HistoryWidget *ui;

    int page_size_ = 9;

};


