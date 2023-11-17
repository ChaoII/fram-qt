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

void FaceList::addItemWidgets(const QVector<Staff> &staffs) {
    checkedIndexIds.clear();
    deleteLayoutWidget();
    auto header = new FaceListHeader();
    header->setObjectName("header");
    header->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                          "font-weight: bold; background-color:rgba(100,100,100,60)}");
    this->layout()->addWidget(header);

    for (int i = 0; i < staffs.size(); i++) {
        auto faceItem = new FaceListItem();
        faceItem->setItem(QString::number(i + 1),
                          QString::number(staffs.at(i).index_id),
                          staffs.at(i).name, staffs.at(i).uid,
                          staffs.at(i).register_time,
                          staffs.at(i).pic_url);
        connect(faceItem, &FaceListItem::checkBoxStatusChanged_signal, this, &FaceList::on_getCheckBoxStatusChange);
        if (i % 2 == 0) {
            faceItem->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                                    "background-color:rgba(160,160,160,40)}");
        } else {
            faceItem->setStyleSheet("QWidget{border:1px solid rgb(200,200,200);\n"
                                    "background-color:rgba(200,200,200,40)}");
        }
        this->layout()->addWidget(faceItem);
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

void FaceList::on_getCheckBoxStatusChange(bool isChecked, const QString &indexId) {

    if (isChecked) {
        // 将ID添加到集合
        checkedIndexIds.push_back(indexId.toLongLong());
    } else {
        // 将ID从集合中删除
        auto it = std::find(checkedIndexIds.begin(), checkedIndexIds.end(), indexId.toLongLong());
        if (it == checkedIndexIds.end()) {
            return;
        }
        std::swap(*it, checkedIndexIds.back());
        checkedIndexIds.pop_back();
    }
}

std::vector<int64_t> FaceList::getCheckedIndexIds() {
    return checkedIndexIds;
}

void FaceList::selectAll() {
    for (int i = 0; i < this->layout()->count(); i++) {
        QLayoutItem *it = this->layout()->itemAt(i);
        auto faceListItem = dynamic_cast<FaceListItem *>(it->widget());
        // 一定要保证转成功
        if (faceListItem) {
            // 如果已经被选过就不要再选了
            if (!faceListItem->getCheckedStatus()) {
                faceListItem->setCheckStatus(true);
            }
        }
    }
    qDebug() << checkedIndexIds.size();
}

void FaceList::unSelectAll() {
    for (int i = 0; i < this->layout()->count(); i++) {
        QLayoutItem *it = this->layout()->itemAt(i);
        auto faceListItem = dynamic_cast<FaceListItem *>(it->widget());
        // 一定要保证转成功
        if (faceListItem) {
            faceListItem->setCheckStatus(false);
        }
    }
    qDebug() << checkedIndexIds.size();
}

void FaceList::reverseSelectAll() {
    for (int i = 0; i < this->layout()->count(); i++) {
        QLayoutItem *it = this->layout()->itemAt(i);
        auto faceListItem = dynamic_cast<FaceListItem *>(it->widget());
        // 一定要保证转成功
        if (faceListItem) {
            faceListItem->setCheckStatus(!faceListItem->getCheckedStatus());
        }
    }
    qDebug() << checkedIndexIds.size();
}

