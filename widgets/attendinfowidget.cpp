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
                                     const QPixmap &picAttendStatus,
                                     const QPixmap &picLibrary,
                                     const QPixmap &picCurrent) {
    ui->lb_name->setText(name);
    ui->lb_attend_time->setText(time);
    ui->lb_pic->setPixmap(picAttendStatus);
    ui->pic_library->setPixmap(picLibrary.scaled(ui->pic_library->size(),
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
    ui->pic_current->setPixmap(picCurrent.scaled(ui->pic_current->size(),
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
}



