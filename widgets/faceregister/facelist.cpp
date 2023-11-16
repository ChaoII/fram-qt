#include "facelist.h"
#include "ui_facelist.h"


FaceList::FaceList(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceList) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
}

FaceList::~FaceList() {
    delete ui;
}

void FaceList::addItemWidgets(const QVector<FaceListItem *> &faceListItems) {
    deleteLayoutWidget();
    auto header = new FaceListHeader();
    header->setObjectName("header");
    header->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                          "font-weight: bold; background-color:rgba(100,100,100,60)}");
    this->layout()->addWidget(header);
    for (int i = 0; i < faceListItems.size(); i++) {
        if (i % 2 == 0) {
            faceListItems[i]->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                                                    "background-color:rgba(160,160,160,40)}");
        } else {
            faceListItems[i]->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                                                    "background-color:rgba(200,200,200,40)}");
        }
        this->layout()->addWidget(faceListItems[i]);
    }
    auto spacer = new QSpacerItem(20, 57, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->layout()->addItem(spacer);
}

void FaceList::deleteLayoutWidget() {
    QLayoutItem *item;
    while ((item = this->layout()->takeAt(0)) != nullptr) {
        delete item->widget();  // 删除控件
        delete item;            // 删除布局项
    }
}

