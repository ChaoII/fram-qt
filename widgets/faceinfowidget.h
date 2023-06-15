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

    void update_paging();

signals:
    void face_back_signal();

private slots:

    void handle_register_finished();

    void on_pb1_back_clicked();

    void on_pb1_delete_clicked();

    void on_tb_staff_clicked(const QModelIndex &index);

private:
    Ui::FaceInfoWidget *ui;

    QStandardItemModel* the_model;

    QItemSelectionModel* the_select;

    int page_size_ = 9;

    PagingWidget* paging;

    RegisterWidget *register_widget;
};

#endif // HISTORYWIDGET_H
