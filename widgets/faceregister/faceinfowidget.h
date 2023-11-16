
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "core/SeetaFace.h"
#include "registerwidget.h"
#include "customwidget/pagingwidget.h"


namespace Ui {
    class FaceInfoWidget;
}

class FaceInfoWidget : public QWidget {
Q_OBJECT

public:
    explicit FaceInfoWidget(QWidget *parent = nullptr);

    void update_register_frame(const QImage &img);

    void update_register_widget();

    ~FaceInfoWidget() override;

private slots:

    void on_pb1_register_clicked();

private:
    void update_table(int page);

    void update_paging_and_table();

signals:

    void face_back_signal();

private slots:

    void handle_register_finished();

    void on_pb1_back_clicked();

    void on_pb1_delete_clicked();

    void on_pb1_selectAll_clicked();

    void on_pb1_unSelectAll_clicked();

    void on_pb1_reverseSelect_clicked();

    void on_cb1_showPicture_stateChanged(int);


private:
    Ui::FaceInfoWidget *ui;

    QStandardItemModel *the_model{};

    QItemSelectionModel *the_select{};

    int page_size_ = 9;

    PagingWidget *paging;

    RegisterWidget *register_widget;

    int selected_count = 0;
};

