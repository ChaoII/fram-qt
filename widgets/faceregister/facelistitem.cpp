#include "facelistitem.h"
#include "ui_facelistitem.h"
#include <QMouseEvent>

FaceListItem::FaceListItem(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceListItem) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    ui->lb_checkBox->installEventFilter(this);
    int minSize = ui->lb_checkBox->size().width() < ui->lb_checkBox->size().height()
                  ? ui->lb_checkBox->size().width()
                  : ui->lb_checkBox->size().height();
    hintSize = int(0.6 * minSize);
    ui->lb_checkBox->setAlignment(Qt::AlignCenter);
    ui->lb_checkBox->setPixmap(QPixmap(":/img/icon_unchecked.png")
                                       .scaled(hintSize, hintSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation));

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
            isChecked = !isChecked;
            changeCheckBoxStatus(isChecked);
            emit checkBoxStatusChanged_signal(isChecked, ui->lb_indexId->text());
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void FaceListItem::changeCheckBoxStatus(bool isCheckStatus) {

    if (isCheckStatus) {

        ui->lb_checkBox->setPixmap(QPixmap(":/img/icon_checked.png")
                                           .scaled(hintSize, hintSize,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    } else {
        ui->lb_checkBox->setPixmap(QPixmap(":/img/icon_unchecked.png")
                                           .scaled(hintSize, hintSize,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    }
}

bool FaceListItem::getCheckedStatus() {
    return isChecked;
}

void FaceListItem::setCheckStatus(bool isCheckedStatus) {
    isChecked = isCheckedStatus;
    changeCheckBoxStatus(isChecked);
    emit checkBoxStatusChanged_signal(isChecked, ui->lb_indexId->text());
}


