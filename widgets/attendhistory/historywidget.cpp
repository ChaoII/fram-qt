#include "historywidget.h"
#include "ui_historywidget.h"
#include "widgets/attendhistory/attendinfoitemwidget.h"


HistoryWidget::HistoryWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::HistoryWidget) {
    ui->setupUi(this);
    page_size_ = (height() - 40) / 100;
    setAttribute(Qt::WA_StyledBackground, true);
    int num = SeetaFace::getInstance().getQueryNum<Attend>();

    ui->paging->initPage(num, 0, page_size_);
    ui->paging->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    updatePagingAndTable();
    connect(ui->paging, &PagingWidget::pageChanged, this, &HistoryWidget::updateTable);
}

HistoryWidget::~HistoryWidget() {
    delete ui;
}

void HistoryWidget::updateTable(int page) {
    QVector<AttendInfoItemWidget *> attendInfoItemWidgets;
    auto rs = SeetaFace::getInstance().getQueryInfo<Attend>(page_size_, page - 1);
    for (int i = 0; i < rs.size(); i++) {
        auto attendItem = new AttendInfoItemWidget();
        attendItem->setItem(QString::number(i + 1), rs.at(i).name, rs.at(i).uid, rs.at(i).attend_time,
                            rs.at(i).pic_url);
        attendInfoItemWidgets.push_back(attendItem);
    }
    this->ui->attendHistoryList->addItemWidgets(attendInfoItemWidgets);
}


void HistoryWidget::on_tb_back_clicked() {
    emit historyBackSignal();
}

void HistoryWidget::updatePagingAndTable() {
    int num = SeetaFace::getInstance().getQueryNum<Attend>();
    ui->paging->initPage(num, 0, page_size_);
    updateTable(0);

}

void HistoryWidget::updateHistoryWidget() {
    updatePagingAndTable();
}


