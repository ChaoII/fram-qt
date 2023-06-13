#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "core/SeetaFace.h"
#include "widgets/registerwidget.h"
#include "customwidget/pagingwidget.h"


namespace Ui {
class FaceInfoWidget;
}

class FaceInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FaceInfoWidget(QWidget *parent = nullptr);

    void update_register_frame(const QImage &img);

    ~FaceInfoWidget();

private slots:
    void on_pb1_register_clicked();

private:
    void update_table(int page);

    void initial_table();

signals:
    void finished_signal();

private slots:
    void handle_register_slot();


private:
    Ui::FaceInfoWidget *ui;

    QStandardItemModel* the_model;

    QItemSelectionModel* the_select;

    int page_size_ = 9;

    PagingWidget* paging;

    RegisterWidget *register_widget;
};

#endif // HISTORYWIDGET_H
