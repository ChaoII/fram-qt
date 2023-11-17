
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


private:

    void updatePageSize();

    void update_table(int page);

    void update_paging_and_table();

signals:

    void face_back_signal();

private slots:


    void handle_register_finished();

    void on_tb_register_clicked();

    void on_tb_refresh_clicked();

    void on_tb_back_clicked();

    void on_tb_delete_clicked();

    void on_tb_selectAll_clicked();

    void on_tb_unselectAll_clicked();

    void on_tb_revertSelect_clicked();


private:
    Ui::FaceInfoWidget *ui;

    int hitPageSize;

    PagingWidget *paging;

    RegisterWidget *register_widget;

};

