#include "attendhistorylist.h"
#include "ui_attendhistorylist.h"


AttendHistoryList::AttendHistoryList(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AttendHistoryList) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
}

AttendHistoryList::~AttendHistoryList() {
    delete ui;
}

void AttendHistoryList::addItemWidgets(const QVector<AttendInfoItemWidget *> &attendInfoItemWidgets) {
    deleteLayoutWidget();
    auto header = new AttendHistoryListHeader();
    header->setObjectName("header");
    header->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                          "font-weight: bold; background-color:rgba(100,100,100,60)}");
    this->layout()->addWidget(header);
    for (int i = 0; i < attendInfoItemWidgets.size(); i++) {
        if (i % 2 == 0) {
            attendInfoItemWidgets[i]->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                                                    "background-color:rgba(160,160,160,40)}");
        } else {
            attendInfoItemWidgets[i]->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                                                    "background-color:rgba(200,200,200,40)}");
        }
        this->layout()->addWidget(attendInfoItemWidgets[i]);
    }
    auto spacer = new QSpacerItem(20, 57, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->layout()->addItem(spacer);
}

void AttendHistoryList::deleteLayoutWidget() {
    QLayoutItem *item;
    while ((item = this->layout()->takeAt(0)) != nullptr) {
        delete item->widget();  // 删除控件
        delete item;            // 删除布局项
    }
}

