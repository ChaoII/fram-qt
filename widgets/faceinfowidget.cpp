#include "faceinfowidget.h"
#include "ui_faceinfowidget.h"

#include <QCheckBox>

FaceInfoWidget::FaceInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceInfoWidget)
{
    ui->setupUi(this);
    initial_table();

//    setAttribute(Qt::WA_DeleteOnClose);
    register_widget = new RegisterWidget();
    layout()->addWidget(register_widget);
    register_widget->setVisible(false);
    connect(register_widget,&RegisterWidget::register_finished_signal,this,&FaceInfoWidget::handle_register_slot);

    int num = SeetaFace::getInstance()->get_query_num<Staff>();
    paging = new PagingWidget();
    paging->initPage(num, 0, page_size_);
    update_table(0);
    layout()->addWidget(paging);
    connect(paging, &PagingWidget::pageChanged, this, &FaceInfoWidget::update_table);
}

void FaceInfoWidget::update_register_frame(const QImage &img)
{
    if(register_widget && register_widget->isVisible()){
        register_widget->update_frame(img);
    }
}

FaceInfoWidget::~FaceInfoWidget()
{
    delete ui;
}

void FaceInfoWidget::update_table(int page)
{
    the_model->clear();
    the_model->setHorizontalHeaderLabels(QStringList()<<""<<"编号"<<"ID"<<"姓名"<<"工号"<<"图片");

    // the start index is 0 so sql squery must -1
    auto rs = SeetaFace::getInstance()->get_query_info<Staff>(page_size_, page-1);
    for(int i=0; i <rs.size(); i++){
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
//        ui->tb_staff->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
        ui->tb_staff->setColumnWidth(0, 20);
        ui->tb_staff->setColumnWidth(1, 100);
        ui->tb_staff->setColumnWidth(3, 100);
        ui->tb_staff->setColumnWidth(4, 60);
        ui->tb_staff->setColumnWidth(5, 60);

        QStandardItem *item0 = new QStandardItem();
        item0->setCheckable(true);

        QStandardItem *item1 = new QStandardItem(QString::number(i+1));
        QStandardItem *item2 = new QStandardItem(QString::number(rs.at(i).index_id));
        QStandardItem *item3 = new QStandardItem(rs.at(i).name);
        QStandardItem *item4 = new QStandardItem(rs.at(i).uid);
        the_model->setItem(i, 0, item0);
        the_model->setItem(i, 1, item1);
        the_model->setItem(i, 2, item2);
        the_model->setItem(i, 3, item3);
        the_model->setItem(i, 4, item4);

        ui->tb_staff->setRowHeight(i, 100);
        QLabel* label = new QLabel();
        label->resize(60,100);
        label->setPixmap(QPixmap(rs.at(i).pic_url).scaled(label->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->tb_staff->setIndexWidget(the_model->index(i, 5), label);
    }
    ui->tb_staff->selectRow(0);
}

void FaceInfoWidget::initial_table()
{
    //    ui->tb_history->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tb_staff->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tb_staff->horizontalHeader()->setMinimumHeight(54);
        ui->tb_staff->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tb_staff->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tb_staff->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏垂直滚动条
        ui->tb_staff->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏水平滚动条
        ui->tb_staff->setMinimumWidth(5);
        ui->tb_staff->setAlternatingRowColors(true);
        // 关闭行号
        ui->tb_staff->verticalHeader()->setHidden(true);
        the_model = new QStandardItemModel();
        the_select = new QItemSelectionModel();
        the_model->setHorizontalHeaderLabels(QStringList()<<""<<"编号"<<"ID"<<"姓名"<<"工号"<<"图片");
        ui->tb_staff->setModel(the_model);
}

void FaceInfoWidget::handle_register_slot()
{
    if(register_widget){
        register_widget->hide();
    }
    paging->setVisible(true);
//    emit finished_signal();
}



void FaceInfoWidget::on_pb1_register_clicked()
{
    paging->setVisible(false);
    register_widget->setVisible(true);

}

