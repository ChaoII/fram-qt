#include "facelistitem.h"
#include "ui_facelistitem.h"
#include <QMouseEvent>

FaceListItem::FaceListItem(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceListItem) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    ui->lb_checkBox->installEventFilter(this);
}

FaceListItem::~FaceListItem() {
    delete ui;
}

void FaceListItem::setItem(const QString &number,
                           const QString &indexId,
                           const QString &name,
                           const QString &staffId,
                           const QString &registerTime,
                           const QString &pictureUrl) {
    ui->lb_number->setText(number);
    ui->lb_indexId->setText(indexId);
    ui->lb_name->setText(name);
    ui->lb_staffId->setText(staffId);
    ui->lb_registerTime->setText(registerTime);
    ui->lb_picture->setPixmap(QPixmap(pictureUrl)
                                      .scaled(ui->lb_picture->size(),
                                              Qt::KeepAspectRatio,
                                              Qt::FastTransformation));
}

bool FaceListItem::eventFilter(QObject *watched, QEvent *event) {
    //拦截发往label子控件的鼠标按下事件
    if (watched == ui->lb_checkBox) {
        if (event->type() == QEvent::MouseButtonPress) {
            changeCheckBoxStatus();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void FaceListItem::changeCheckBoxStatus() {
    isChecked = !isChecked;
    if (isChecked) {
        ui->lb_checkBox->setPixmap(QPixmap(":/img/signin_fail.png"));
    } else {
        ui->lb_checkBox->setPixmap(QPixmap(":/img/signin_success.png"));
    }
}

