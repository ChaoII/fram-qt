#include "faceinfowidget.h"
#include "ui_faceinfowidget.h"
#include "facelistitem.h"
#include <QVBoxLayout>

FaceInfoWidget::FaceInfoWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceInfoWidget) {
    ui->setupUi(this);
    ui->cb1_showPicture->setCheckState(Qt::Unchecked);
    page_size_ = (height() - 40) / 100;
    register_widget = new RegisterWidget();
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
    QVector<FaceListItem *> faceListItems;
    auto rs = SeetaFace::getInstance().get_query_info<Staff>(page_size_, page - 1);
    for (int i = 0; i < rs.size(); i++) {
        auto attendItem = new FaceListItem();
        attendItem->setItem(QString::number(i + 1),
                            QString::number(rs.at(i).index_id),
                            rs.at(i).name, rs.at(i).uid,
                            rs.at(i).register_time,
                            rs.at(i).pic_url);
        faceListItems.push_back(attendItem);
    }
    this->ui->faceList->addItemWidgets(faceListItems);
}


void FaceInfoWidget::update_paging_and_table() {
    int num = SeetaFace::getInstance().get_query_num<Staff>();
    paging->initPage(num, 0, page_size_);
    update_table(0);
}

void FaceInfoWidget::handle_register_finished() {
    register_widget->setVisible(false);
    paging->setVisible(true);
    update_paging_and_table();
}


void FaceInfoWidget::on_pb1_register_clicked() {
    paging->setVisible(false);
    register_widget->setVisible(true);
    update_paging_and_table();
}


void FaceInfoWidget::on_pb1_back_clicked() {
    emit face_back_signal();
}

void FaceInfoWidget::on_pb1_delete_clicked() {
    std::vector<int64_t> index_ids;
    // 删除行需要从后往前删除，不然直接报错
    int count = the_model->rowCount();
    for (int i = count - 1; i >= 0; i--) {
        if (Qt::Checked == the_model->item(i, 0)->checkState()) {
            qint64 index_id = the_model->item(i, 2)->text().toLongLong();
            index_ids.push_back(index_id);
            the_model->removeRow(i);
        }
    }
    SeetaFace::getInstance().delete_face_by_ids(index_ids);
    update_paging_and_table();
}


void FaceInfoWidget::update_register_widget() {
    update_paging_and_table();
}

void FaceInfoWidget::on_pb1_selectAll_clicked() {
    int rowCount = the_model->rowCount();
    for (int i = 0; i < rowCount; i++) {
        if (Qt::Unchecked == the_model->item(i, 0)->checkState()) {
            the_model->item(i, 0)->setCheckState(Qt::Checked);
        }
    }
}

void FaceInfoWidget::on_pb1_unSelectAll_clicked() {
    int rowCount = the_model->rowCount();
    for (int i = 0; i < rowCount; i++) {
        if (Qt::Checked == the_model->item(i, 0)->checkState()) {
            the_model->item(i, 0)->setCheckState(Qt::Unchecked);
        }
    }
}

void FaceInfoWidget::on_pb1_reverseSelect_clicked() {
    int rowCount = the_model->rowCount();
    for (int i = 0; i < rowCount; i++) {
        if (Qt::Checked == the_model->item(i, 0)->checkState()) {
            the_model->item(i, 0)->setCheckState(Qt::Unchecked);
        } else if (Qt::Unchecked == the_model->item(i, 0)->checkState()) {
            the_model->item(i, 0)->setCheckState(Qt::Checked);
        }
    }
}

void FaceInfoWidget::on_cb1_showPicture_stateChanged(int s) {

    update_paging_and_table();
}




