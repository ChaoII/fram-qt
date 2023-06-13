#include "widgets/registerwidget.h"
#include "ui_registerwidget.h"
#include "core/SeetaFace.h"
#include <QPainter>
#include <QDebug>
#include "utils/Utils.h"

RegisterWidget::RegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    init_lw();
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}


void RegisterWidget::update_frame(const QImage &img)
{
    if(img.isNull()) return;
    img_ = img;
    QImage img_scaled = img.scaled(ui->pic->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->pic->setPixmap(QPixmap::fromImage(img_scaled));
    update();
}

void RegisterWidget::on_push_back_clicked()
{
    if(register_face_.empty()){
        emit register_finished_signal();
    }else{
        qWarning() << "faces is not saved! please click the ensure button and back to main window";
        ui->edit_name->setText("");
        ui->edit_staff_id->setText("");
        ui->face_list->clear();
        emit register_finished_signal();
    }
}


void RegisterWidget::on_button_ensure_clicked()
{
    while(!register_face_.isEmpty()){
        auto face_info = register_face_.takeFirst();
        auto item = ui->face_list->takeItem(0);
        ui->face_list->removeItemWidget(item);
        bool ret = SeetaFace::getInstance()->add_face(face_info.img,
                                                      face_info.uid,
                                                     face_info.name);
        if(!ret) continue;
    }
    ui->edit_name->setText("");
    ui->edit_staff_id->setText("");
    emit register_finished_signal();
}

void RegisterWidget::init_lw()
{
    ui->face_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->face_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->face_list->setIconSize(QSize(140,150));
    ui->face_list->setResizeMode(QListView::Adjust);
    ui->face_list->setViewMode(QListView::IconMode);
    ui->face_list->setMovement(QListView::Static);
    ui->face_list->setSpacing(10);
    connect(ui->face_list, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        int index = item->listWidget()->currentRow();
        register_face_.removeAt(index);
    });
}


void RegisterWidget::on_pb_add_clicked()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(ui->edit_name->text());
    item->setIcon(QIcon(QPixmap::fromImage(img_)));
    ui->face_list->addItem(item);
    register_face_.push_back(RegisterFace{Utils::QImage2CvMat(img_),
                                          ui->edit_staff_id->text(),
                                          ui->edit_name->text()});
    if(ui->face_list->count()> MAX_FACE){
        auto item = ui->face_list->takeItem(0);
        delete item;
        register_face_.pop_front();
    }
}

