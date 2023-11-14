#include "historywidget.h"
#include "ui_historywidget.h"


HistoryWidget::HistoryWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::HistoryWidget) {
    ui->setupUi(this);
//    ui->tb_history->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tb_history->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tb_history->horizontalHeader()->setMinimumHeight(30);
    ui->tb_history->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_history->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tb_history->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏垂直滚动条
    ui->tb_history->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏水平滚动条
    ui->tb_history->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tb_history->setMinimumWidth(5);
    ui->tb_history->setAlternatingRowColors(true);
    // 关闭行号
    ui->tb_history->verticalHeader()->setHidden(true);
    the_model = new QStandardItemModel();
    the_select = new QItemSelectionModel();
    the_model->setHorizontalHeaderLabels(QStringList() << "编号" << "姓名" << "工号" << "打卡时间" << "图片");
    ui->tb_history->setModel(the_model);
    int num = SeetaFace::getInstance().get_query_num<Attend>();
    paging = new PagingWidget();
    paging->initPage(num, 0, page_size_);
    update_table(0);
    layout()->addWidget(paging);
    connect(paging, &PagingWidget::pageChanged, this, &HistoryWidget::update_table);
}

HistoryWidget::~HistoryWidget() {
    delete ui;
}

void HistoryWidget::update_table(int page) {
    the_model->clear();
    the_model->setHorizontalHeaderLabels(QStringList() << "编号" << "姓名" << "工号" << "打卡时间" << "图片");

    // the start index is 0 so sql squery must -1
    auto rs = SeetaFace::getInstance().get_query_info<Attend>(page_size_, page - 1);
    for (int i = 0; i < rs.size(); i++) {
        ui->tb_history->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tb_history->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
        ui->tb_history->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
//        ui->tb_history->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
        ui->tb_history->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
        ui->tb_history->setColumnWidth(0, 100);
        ui->tb_history->setColumnWidth(1, 100);
        ui->tb_history->setColumnWidth(2, 100);
        ui->tb_history->setColumnWidth(4, 60);

        QStandardItem *item1 = new QStandardItem(QString::number(i + 1));
        QStandardItem *item2 = new QStandardItem(rs.at(i).name);
        QStandardItem *item3 = new QStandardItem(rs.at(i).uid);
        QStandardItem *item4 = new QStandardItem(QString(rs.at(i).attend_time));
        the_model->setItem(i, 0, item1);
        the_model->setItem(i, 1, item2);
        the_model->setItem(i, 2, item3);
        the_model->setItem(i, 3, item4);
        // 添加图片
        ui->tb_history->setRowHeight(i, 100);
        QLabel *label = new QLabel();
        label->resize(60, 100);
        label->setPixmap(QPixmap(rs.at(i).pic_url).scaled(label->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->tb_history->setIndexWidget(the_model->index(i, 4), label);
    }
    ui->tb_history->selectRow(0);
}


void HistoryWidget::on_pb_back_clicked() {
    emit history_back_signal();
}

void HistoryWidget::update_paging() {

    int num = SeetaFace::getInstance().get_query_num<Attend>();
    paging->initPage(num, 0, page_size_);
    update_table(0);

}

void HistoryWidget::update_history_widget() {
    update_paging();
}

