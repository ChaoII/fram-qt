#include "historywidget.h"
#include "ui_historywidget.h"
#include "pagingwidget.h"

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);
    ui->tb_history->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    model = new QStandardItemModel();
    ui->tb_history->setModel(model);
    model->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"工号"<<"打卡时间"<<"图片");
    auto rs = SeetaFace::getInstance()->get_attend_info(50, 0);
    for(int i=0;i<rs.size();i++){
        QStandardItem *item0 = new QStandardItem(rs.at(i).name);
        QStandardItem *item1 = new QStandardItem(rs.at(i).uid);
        QStandardItem *item2 = new QStandardItem(rs.at(i).attend_time);
        QStandardItem *item3 = new QStandardItem(rs.at(i).pic_url);
        model->setItem(i,0,item0);
        model->setItem(i,1,item1);
        model->setItem(i,2,item2);
        model->setItem(i,3,item3);
    }
    int num = SeetaFace::getInstance()->get_attend_num();
    PagingWidget* paging = new PagingWidget();
    paging->set_params(0,5,num);
    ui->page_layout->addWidget(paging);
}



HistoryWidget::~HistoryWidget()
{
    delete ui;
}
