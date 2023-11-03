#include "widgets/registerwidget.h"
#include "ui_registerwidget.h"
#include "core/SeetaFace.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include "utils/Utils.h"

RegisterWidget::RegisterWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::RegisterWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    init_lw();
}

RegisterWidget::~RegisterWidget() {
    delete ui;
}


void RegisterWidget::update_frame(const QImage &img) {
    if (img.isNull()) return;
    img_ = img;
    QImage img_scaled = img.scaled(ui->pic->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->pic->setPixmap(QPixmap::fromImage(img_scaled));
    update();
}

void RegisterWidget::on_push_back_clicked() {
    if (register_face_.empty()) {
        clear_register_info();
        emit register_finished_signal();
    } else {
        QMessageBox *msg_box = new QMessageBox(QMessageBox::Warning,
                                               "警告", "人脸信息还未保存，"
                                                       "请回到注册界面点击确认按钮保存人脸信息.",
                                               QMessageBox::Yes | QMessageBox::Cancel, this);
        msg_box->setDefaultButton(QMessageBox::Yes);
        msg_box->button(QMessageBox::Yes)->setText("确认");
        msg_box->button(QMessageBox::Cancel)->setText("取消");
        auto ret = msg_box->exec();
        qWarning() << "faces is not saved! please click the ensure button and back to main window";
        if (QMessageBox::Yes != ret) {
            clear_register_info();
            emit register_finished_signal();
        }
    }
}


void RegisterWidget::on_button_ensure_clicked() {
    while (!register_face_.isEmpty()) {
        auto face_info = register_face_.takeFirst();
        auto item = ui->face_list->takeItem(0);
        ui->face_list->removeItemWidget(item);
        bool ret = SeetaFace::getInstance()->add_face(face_info.img,
                                                      face_info.uid,
                                                      face_info.name);
        if (!ret) {
            qWarning() << "currnt face is invailid...";
            QMessageBox::warning(this, "警告",
                                 "当前存在图片未检测到人脸，或其他原因导致人脸录入失败，"
                                 "请确保人脸在画面正中间，切勿大幅晃动！！");
            continue;
        }
    }
    ui->edit_name->setText("");
    ui->edit_staff_id->setText("");
    emit register_finished_signal();
}

void RegisterWidget::init_lw() {
    ui->face_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->face_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->face_list->setIconSize(QSize(140, 150));
    ui->face_list->setResizeMode(QListView::Adjust);
    ui->face_list->setViewMode(QListView::IconMode);
    ui->face_list->setMovement(QListView::Static);
    ui->face_list->setSpacing(20);
    connect(ui->face_list, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        int index = item->listWidget()->currentRow();
        register_face_.removeAt(index);
    });
}

void RegisterWidget::clear_register_info() {
    ui->edit_name->setText("");
    ui->edit_staff_id->setText("");
    ui->face_list->clear();
}


void RegisterWidget::on_pb_add_clicked() {
    if (ui->edit_name->text().isEmpty() || ui->edit_staff_id->text().isEmpty()) {
        auto ret = QMessageBox::warning(this, "warning", "staff id and name must be not empty", QMessageBox::Yes);
        return;
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(ui->edit_name->text());
    item->setIcon(QIcon(QPixmap::fromImage(img_)));
    ui->face_list->addItem(item);
    register_face_.push_back(RegisterFace{Utils::QImage2CvMat(img_),
                                          ui->edit_staff_id->text(),
                                          ui->edit_name->text()});
    if (ui->face_list->count() > Config::getInstance()->getMax_face_num()) {
        auto item = ui->face_list->takeItem(0);
        delete item;
        register_face_.pop_front();
    }
}

