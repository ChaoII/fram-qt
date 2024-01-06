#include "attendinfowidget.h"
#include "ui_attendinfowidget.h"

AttendInfoWidget::AttendInfoWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AttendInfoWidget) {
    ui->setupUi(this);
}

AttendInfoWidget::~AttendInfoWidget() {
    delete ui;
}

void AttendInfoWidget::setAttendInfo(const QString &name,
                                     const QString &time,
                                     const QPixmap &pic_attend_status,
                                     const QPixmap &pic_library,
                                     const QPixmap &pic_current) {
    ui->lb_name->setText(name);
    ui->lb_attend_time->setText(time);
    ui->lb_pic->setPixmap(pic_attend_status);
    ui->pic_library->setPixmap(pic_library.isNull() ? pic_library :
                               pic_library.scaled(ui->pic_library->size(),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
    ui->pic_current->setPixmap(pic_current.isNull() ? pic_current :
                               pic_current.scaled(ui->pic_current->size(),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
}

void AttendInfoWidget::clearAttendInfo() {
    setAttendInfo("", "", QPixmap(), QPixmap(), QPixmap());
}



