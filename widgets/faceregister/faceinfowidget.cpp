#include "faceinfowidget.h"
#include "ui_faceinfowidget.h"
#include "facelistitem.h"
#include <QVBoxLayout>

FaceInfoWidget::FaceInfoWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceInfoWidget) {
    ui->setupUi(this);
    hitPageSize = (this->height() - 40) / 100;
    register_widget = new RegisterWidget();

//    register_widget->setStyleSheet("#frame{border:3px solid;}");


    layout()->addWidget(register_widget);
    register_widget->setVisible(false);
    connect(register_widget, &RegisterWidget::register_finished_signal, this,
            &FaceInfoWidget::handle_register_finished);
    paging = new PagingWidget();
    update_paging_and_table();
    layout()->addWidget(paging);
    connect(paging, &PagingWidget::pageChanged, this, &FaceInfoWidget::update_table);
}

void FaceInfoWidget::update_register_frame(const QImage &img) {
    if (register_widget && register_widget->isVisible()) {
        register_widget->update_frame(img);
    }
}

FaceInfoWidget::~FaceInfoWidget() {
    delete ui;
}

void FaceInfoWidget::update_table(int page) {
    auto Staffs = SeetaFace::getInstance().get_query_info<Staff>(hitPageSize, page - 1);
    this->ui->faceList->addItemWidgets(Staffs);
}


void FaceInfoWidget::update_paging_and_table() {
    int num = SeetaFace::getInstance().get_query_num<Staff>();
    paging->initPage(num, 0, hitPageSize);
    update_table(0);
}

void FaceInfoWidget::handle_register_finished() {
    register_widget->setVisible(false);
    paging->setVisible(true);
    updatePageSize();
    update_paging_and_table();
}


void FaceInfoWidget::on_tb_register_clicked() {
    paging->setVisible(false);
    register_widget->setVisible(true);
    updatePageSize();
    update_paging_and_table();
}


void FaceInfoWidget::on_tb_back_clicked() {
    emit face_back_signal();
}

void FaceInfoWidget::on_tb_delete_clicked() {
    auto index_ids = this->ui->faceList->getCheckedIndexIds();
    SeetaFace::getInstance().delete_face_by_ids(index_ids);
    update_paging_and_table();
}


void FaceInfoWidget::update_register_widget() {
    update_paging_and_table();
}

void FaceInfoWidget::on_tb_selectAll_clicked() {
    ui->faceList->selectAll();
}

void FaceInfoWidget::on_tb_unselectAll_clicked() {
    ui->faceList->unSelectAll();
}

void FaceInfoWidget::on_tb_revertSelect_clicked() {
    ui->faceList->reverseSelectAll();
}


void FaceInfoWidget::updatePageSize() {
    hitPageSize = (ui->faceList->height() - 40) / 100;
}

void FaceInfoWidget::on_tb_refresh_clicked() {
    update_paging_and_table();
}




