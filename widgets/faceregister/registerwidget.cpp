#include "registerwidget.h"
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
    ui->frame->setStyleSheet("#frame{border:3px solid rgb(200,200,200);}"
                             "QFrame{font-size:18px;}"
                             "QFrame .QLineEdit {font-size:18px;\n"
                             "border:1px solid rgba(150,150,150,100);\n"
                             "border-radius:3px;\n"
                             "background-color:rgba(200,200,200,40)}\n"
                             "QListWidget{font-size:12px}");
    initialRegisterFaceListWidget();
}

RegisterWidget::~RegisterWidget() {
    delete ui;
}


void RegisterWidget::update_frame(const QImage &img) {
    if (img.isNull()) return;
    img_ = img;
    QImage img_scaled = img.scaled(ui->pic->size(),
                                   Qt::AspectRatioMode::IgnoreAspectRatio,
                                   Qt::FastTransformation);
    ui->pic->setPixmap(QPixmap::fromImage(img_scaled));
    update();
}

void RegisterWidget::on_tb_back_clicked() {
    if (register_face_.empty()) {
        clear_register_info();
        emit register_finished_signal();
    } else {
        auto msg_box = new QMessageBox(QMessageBox::Warning,
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


void RegisterWidget::on_tb_ensure_clicked() {
    while (!register_face_.isEmpty()) {
        auto face_info = register_face_.takeFirst();
        auto item = ui->lw_faceList->takeItem(0);
        ui->lw_faceList->removeItemWidget(item);
        bool ret = SeetaFace::getInstance().add_face(face_info.img,
                                                     face_info.uid,
                                                     face_info.name);
        if (!ret) {
            qWarning() << "current face is invalid...";
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

void RegisterWidget::initialRegisterFaceListWidget() {
    ui->lw_faceList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lw_faceList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lw_faceList->setIconSize(QSize(140, 150));
    ui->lw_faceList->setResizeMode(QListView::Fixed);
    ui->lw_faceList->setViewMode(QListView::IconMode);
    ui->lw_faceList->setMovement(QListView::Static);
    ui->lw_faceList->setSpacing(14);
    ui->lw_faceList->setItemAlignment(Qt::AlignCenter);
    connect(ui->lw_faceList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        int index = item->listWidget()->currentRow();
        register_face_.removeAt(index);
    });
}

void RegisterWidget::clear_register_info() {
    ui->edit_name->setText("");
    ui->edit_staff_id->setText("");
    ui->lw_faceList->clear();
}


void RegisterWidget::on_tb_add_clicked() {
    if (ui->edit_name->text().isEmpty() || ui->edit_staff_id->text().isEmpty()) {
        QMessageBox::warning(this, "warning", "工号和姓名不能为空", QMessageBox::Yes);
        return;
    }
    if (img_.isNull()) {
        QMessageBox::warning(this, "warning", "请检查摄像头是否正确显示画面", QMessageBox::Yes);
        return;
    }
    auto item = new QListWidgetItem;
    item->setText(ui->edit_name->text());
    item->setIcon(QIcon(QPixmap::fromImage(img_)));
    ui->lw_faceList->addItem(item);
    register_face_.push_back(RegisterFace{Utils::QImage2CvMat(img_),
                                          ui->edit_staff_id->text(),
                                          ui->edit_name->text()});
    if (ui->lw_faceList->count() > Config::getInstance().get_max_face_num()) {
        auto ele = ui->lw_faceList->takeItem(0);
        delete ele;
        register_face_.pop_front();
    }
}

