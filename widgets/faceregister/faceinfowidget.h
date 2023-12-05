
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "core/seetaface.h"
#include "registerwidget.h"
#include "customwidget/pagingwidget.h"


namespace Ui {
    class FaceInfoWidget;
}

class FaceInfoWidget : public QWidget {
Q_OBJECT

public:
    explicit FaceInfoWidget(QWidget *parent = nullptr);

    void updateRegisterFrame(const QImage &img);

    void updateRegisterWidget();

    ~FaceInfoWidget() override;


private:

    void updatePageSize();

    void updateTable(int page);

    void updatePagingAndTable();

signals:

    void face_back_signal();

private slots:


    void handleRegisterFinished();

    void on_tb_register_clicked();

    void on_tb_refresh_clicked();

    void on_tb_back_clicked();

    void on_tb_delete_clicked();

    void on_tb_selectAll_clicked();

    void on_tb_unselectAll_clicked();

    void on_tb_revertSelect_clicked();


private:
    Ui::FaceInfoWidget *ui;

    int hint_page_size_;

    PagingWidget *paging_;

    RegisterWidget *register_widget_;

};

