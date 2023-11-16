#include "attendhistorylistheader.h"
#include "ui_attendhistorylistheader.h"

AttendHistoryListHeader::AttendHistoryListHeader(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AttendHistoryListHeader) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

AttendHistoryListHeader::~AttendHistoryListHeader() {
    delete ui;
}
