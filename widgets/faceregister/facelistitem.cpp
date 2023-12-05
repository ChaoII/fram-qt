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
    hint_size_ = int(0.6 * minSize);
    ui->lb_checkBox->setAlignment(Qt::AlignCenter);
    ui->lb_checkBox->setPixmap(QPixmap(":/img/icon_unchecked.png")
                                       .scaled(hint_size_, hint_size_,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation));

}

FaceListItem::~FaceListItem() {
    delete ui;
}

void FaceListItem::setItem(const QString &number,
                           const QString &index_id,
                           const QString &name,
                           const QString &staff_id,
                           const QString &register_time,
                           const QString &picture_url) {
    ui->lb_number->setText(number);
    ui->lb_indexId->setText(index_id);
    ui->lb_name->setText(name);
    ui->lb_staffId->setText(staff_id);
    ui->lb_registerTime->setText(register_time);
    ui->lb_picture->setPixmap(QPixmap(picture_url)
                                      .scaled(ui->lb_picture->size(),
                                              Qt::KeepAspectRatio,
                                              Qt::FastTransformation));
}

bool FaceListItem::eventFilter(QObject *watched, QEvent *event) {
    //拦截发往label子控件的鼠标按下事件
    if (watched == ui->lb_checkBox) {
        if (event->type() == QEvent::MouseButtonPress) {
            is_checked_ = !is_checked_;
            changeCheckBoxStatus(is_checked_);
            emit checkBoxStatusChanged_signal(is_checked_, ui->lb_indexId->text());
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void FaceListItem::changeCheckBoxStatus(bool isCheckStatus) {

    if (isCheckStatus) {

        ui->lb_checkBox->setPixmap(QPixmap(":/img/icon_checked.png")
                                           .scaled(hint_size_, hint_size_,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    } else {
        ui->lb_checkBox->setPixmap(QPixmap(":/img/icon_unchecked.png")
                                           .scaled(hint_size_, hint_size_,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    }
}

bool FaceListItem::getCheckedStatus() const {
    return is_checked_;
}

void FaceListItem::setCheckStatus(bool isCheckedStatus) {
    is_checked_ = isCheckedStatus;
    changeCheckBoxStatus(is_checked_);
    emit checkBoxStatusChanged_signal(is_checked_, ui->lb_indexId->text());
}


