#include "faceinfowidget.h"
#include "ui_faceinfowidget.h"

#include <QCheckBox>
#include <QVBoxLayout>


#define NO_PICTURE_PAGE_SIZE  22;
#define PICTURE_PAGE_SIZE  9;

FaceInfoWidget::FaceInfoWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceInfoWidget) {
    ui->setupUi(this);
    ui->cb1_showPicture->setCheckState(Qt::Unchecked);
    page_size_ = NO_PICTURE_PAGE_SIZE;

    initial_table();
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
    the_model->clear();
    the_model->setHorizontalHeaderLabels(
            QStringList() << "" << "编号" << "ID" << "姓名" << "工号" << "注册时间" << "图片");

    // the start index is 0 so sql squery must -1
    auto rs = SeetaFace::getInstance().get_query_info<Staff>(page_size_, page - 1);
    for (int i = 0; i < rs.size(); i++) {
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
//        ui->tb_staff->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
//        ui->tb_staff->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
        ui->tb_staff->setColumnWidth(0, 0);
        ui->tb_staff->setColumnWidth(1, 60);
        // ui->tb_staff->setColumnWidth(5, 100);
        ui->tb_staff->setColumnWidth(3, 60);
        ui->tb_staff->setColumnWidth(4, 60);
//        ui->tb_staff->setColumnWidth(5, 100);
        ui->tb_staff->setColumnWidth(6, 60);

        auto item0 = new QStandardItem();
        item0->setCheckable(true);
        auto item1 = new QStandardItem(QString::number(i + 1));
        auto item2 = new QStandardItem(QString::number(rs.at(i).index_id));
        auto item3 = new QStandardItem(rs.at(i).name);
        auto item4 = new QStandardItem(rs.at(i).uid);
        auto item5 = new QStandardItem(rs.at(i).register_time);
        the_model->setItem(i, 0, item0);
        the_model->setItem(i, 1, item1);
        the_model->setItem(i, 2, item2);
        the_model->setItem(i, 3, item3);
        the_model->setItem(i, 4, item4);
        the_model->setItem(i, 5, item5);

        if (ui->cb1_showPicture->checkState() == Qt::Checked) {
            ui->tb_staff->setRowHeight(i, 100);
            auto label = new QLabel();
            label->resize(60, 100);
            label->setPixmap(
                    QPixmap(rs.at(i).pic_url).scaled(label->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->tb_staff->setIndexWidget(the_model->index(i, 6), label);
        } else {
            ui->tb_staff->setRowHeight(i, 40);
            auto item6 = new QStandardItem(rs.at(i).pic_url);
            the_model->setItem(i, 6, item6);
        }
    }
    ui->tb_staff->selectRow(0);
}

void FaceInfoWidget::initial_table() {
    //    ui->tb_history->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tb_staff->setStyleSheet("QTableView::item:selected { background-color: None; }");
    ui->tb_staff->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tb_staff->horizontalHeader()->setMinimumHeight(30);
    ui->tb_staff->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_staff->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tb_staff->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏垂直滚动条
    ui->tb_staff->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏水平滚动条
    ui->tb_staff->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tb_staff->setMinimumWidth(5);
    ui->tb_staff->setAlternatingRowColors(true);
    // 关闭行号
    ui->tb_staff->verticalHeader()->setHidden(true);
    the_model = new QStandardItemModel();
    the_select = new QItemSelectionModel();
    the_model->setHorizontalHeaderLabels(
            QStringList() << "" << "编号" << "ID" << "姓名" << "工号" << "注册时间" << "图片");
    ui->tb_staff->setModel(the_model);
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
    if (s == 0) {
        page_size_ = NO_PICTURE_PAGE_SIZE;
    } else {
        page_size_ = PICTURE_PAGE_SIZE;
    }
    update_paging_and_table();
}




