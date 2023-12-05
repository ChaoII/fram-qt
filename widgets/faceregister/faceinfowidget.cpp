#include "faceinfowidget.h"
#include "ui_faceinfowidget.h"
#include "facelistitem.h"
#include <QVBoxLayout>

FaceInfoWidget::FaceInfoWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceInfoWidget) {
    ui->setupUi(this);
    hint_page_size_ = (this->height() - 40) / 100;
    register_widget_ = new RegisterWidget();

//    register_widget->setStyleSheet("#frame{border:3px solid;}");


    layout()->addWidget(register_widget_);
    register_widget_->setVisible(false);
    connect(register_widget_, &RegisterWidget::registerFinishedSignal, this,
            &FaceInfoWidget::handleRegisterFinished);
    paging_ = new PagingWidget();
    updatePagingAndTable();
    layout()->addWidget(paging_);
    connect(paging_, &PagingWidget::pageChanged, this, &FaceInfoWidget::updateTable);
}

void FaceInfoWidget::updateRegisterFrame(const QImage &img) {
    if (register_widget_ && register_widget_->isVisible()) {
        register_widget_->updateFrame(img);
    }
}

FaceInfoWidget::~FaceInfoWidget() {
    delete ui;
}

void FaceInfoWidget::updateTable(int page) {
    auto Staffs = SeetaFace::getInstance().getQueryInfo<Staff>(hint_page_size_, page - 1);
    this->ui->faceList->addItemWidgets(Staffs);
}


void FaceInfoWidget::updatePagingAndTable() {
    int num = SeetaFace::getInstance().getQueryNum<Staff>();
    paging_->initPage(num, 0, hint_page_size_);
    updateTable(0);
}

void FaceInfoWidget::handleRegisterFinished() {
    register_widget_->setVisible(false);
    paging_->setVisible(true);
    updatePageSize();
    updatePagingAndTable();
}


void FaceInfoWidget::on_tb_register_clicked() {
    paging_->setVisible(false);
    register_widget_->setVisible(true);
    updatePageSize();
    updatePagingAndTable();
}


void FaceInfoWidget::on_tb_back_clicked() {
    emit face_back_signal();
}

void FaceInfoWidget::on_tb_delete_clicked() {
    auto index_ids = this->ui->faceList->getCheckedIndexIds();
    SeetaFace::getInstance().deleteFaceByIds(index_ids);
    updatePagingAndTable();
}


void FaceInfoWidget::updateRegisterWidget() {
    updatePagingAndTable();
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
    hint_page_size_ = (ui->faceList->height() - 40) / 100;
}

void FaceInfoWidget::on_tb_refresh_clicked() {
    updatePagingAndTable();
}




