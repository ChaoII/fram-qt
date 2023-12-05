#include "attendinfoitemwidget.h"
#include "ui_attendinfoitemwidget.h"

AttendInfoItemWidget::AttendInfoItemWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AttendInfoItemWidget) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
}

AttendInfoItemWidget::~AttendInfoItemWidget() {
    delete ui;
}

void AttendInfoItemWidget::setItem(const QString &number,
                                   const QString &name,
                                   const QString &staffId,
                                   const QString &attendTime,
                                   const QString &pictureUrl) {
    ui->number->setText(number);
    ui->lb_name->setText(name);
    ui->lb_staffId->setText(staffId);
    ui->lb_attendTime->setText(attendTime);
    ui->lb_picture->setPixmap(QPixmap(pictureUrl)
                                      .scaled(ui->lb_picture->size(),
                                              Qt::KeepAspectRatio,
                                              Qt::FastTransformation));
}
