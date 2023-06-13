//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWidget.h" resolved

#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPainter>
#include <QTimer>
#include <QMetaType>
#include "historywidget.h"


MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose);
    auto face_rec_thread = new FaceRecThread();
    face_rec_thread->moveToThread(&worker_thread1);
    auto record_thread = new RecordThread();
    record_thread->moveToThread(&worker_thread2);
    // connect(&worker_thread1,&QThread::finished,face_rec_thread,&QObject::deleteLater);
    connect(this,&MyWidget::send_img_signal, face_rec_thread, &FaceRecThread::face_recognition, Qt::QueuedConnection);
    qRegisterMetaType<FaceInfoWrap>();
    qRegisterMetaType<QVector<FaceInfoWrap>>();

    connect(face_rec_thread, &FaceRecThread::face_rec_signal, this, &MyWidget::on_face_rec);
    connect(face_rec_thread, &FaceRecThread::record_signal, record_thread, &RecordThread::record);

    face_info_widget = new FaceInfoWidget();
    connect(face_info_widget, &FaceInfoWidget::finished_signal, this, &MyWidget::on_register_finished);

    face_det_thread = new FaceDetThread(this);
    connect(face_det_thread, &FaceDetThread::img_send_signal, this, &MyWidget::update_frame);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyWidget::on_update_time);
    timer->setInterval(1000);
    timer->start();
}

void MyWidget::update_frame(QImage qimg, QRect rect) {
    if(!rect.isEmpty()){
        QDateTime cur_time = QDateTime::currentDateTime();
        if(last_rec_time.msecsTo(cur_time) > REC_INTERVAL){
            emit send_img_signal(qimg,rect);
            last_rec_time = cur_time;
        }
        QPainter painter(&qimg);
        painter.setPen(QPen(QColor(Qt::green)));
        painter.drawRect(rect);
    }
    else{
        Utils::setBackgroundColor(ui->widget, QColor(255, 255, 255, 0));
    }
    ui->widget_info->setVisible(!rect.isEmpty());
    if(face_info_widget && face_info_widget->isVisible()){
        face_info_widget->update_register_frame(qimg);
    };
    if (!isVisible()) return;
    _img = qimg.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void MyWidget::on_face_rec(FaceInfoWrap rec_info) {

    QString attend_time = rec_info.time.split(" ")[1].split(".")[0];
    if (rec_info.code == -1) {
        ui->lb_name->setText("攻击人脸");
        ui->lb_attend_time->setText(attend_time);
        ui->lb_pic->setPixmap(QPixmap(":img/signin_fail.png"));
        ui->pic_library->setPixmap(QPixmap());
        ui->pic_current->setPixmap(QPixmap());
        Utils::setBackgroundColor(ui->widget, QColor(255, 0, 0, 40));
        Utils::setBackgroundColor(ui->widget_info, QColor(255, 0, 0, 40));
    } else if (rec_info.code == 0) {
        ui->lb_name->setText("未知");
        ui->lb_attend_time->setText(attend_time);
        ui->lb_pic->setPixmap(QPixmap(":img/signin_fail.png"));
        ui->pic_library->setPixmap(QPixmap());
        ui->pic_current->setPixmap(QPixmap());
        Utils::setBackgroundColor(ui->widget, QColor(255, 255, 255, 0));
        Utils::setBackgroundColor(ui->widget_info, QColor(255, 0, 0, 40));
    } else {
        ui->lb_name->setText(rec_info.ret.name);
        ui->lb_attend_time->setText(attend_time);
        ui->lb_pic->setPixmap(QPixmap(":img/signin_success.png"));
        ui->pic_library->setPixmap(QPixmap(rec_info.ret.pic_url)
                                   .scaled(ui->pic_library->size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation));
        ui->pic_current->setPixmap(QPixmap::fromImage(rec_info.ret.img)
                                   .scaled(ui->pic_current->size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation));
        Utils::setBackgroundColor(ui->widget, QColor(255, 0, 0, 0));
        Utils::setBackgroundColor(ui->widget_info, QColor(0, 255, 0, 40));
    }
}

void MyWidget::on_pb_register_clicked(){
    face_info_widget->show();
    worker_thread1.quit();
    worker_thread1.wait();
    face_det_thread->close_detect();
    hide();
}

void MyWidget::on_register_finished()
{
    face_det_thread->open_detect();
    face_info_widget->close();
    if(!isVisible()) {
        show();
        worker_thread1.start();
    }
}

void MyWidget::on_update_time() {
    ui->lb_cur_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MyWidget::closeEvent(QCloseEvent *event) {

    face_det_thread->stop_thread();
    face_det_thread->wait();
    event->accept();
}

void MyWidget::paintEvent(QPaintEvent *event) {
    if (_img.isNull()) return;
    QPainter painter(this);
    painter.drawImage(0, 0, _img);
}


void MyWidget::start_thread() {
    if (face_det_thread && !face_det_thread->isRunning()) {
        face_det_thread->start();
    }
}

void MyWidget::run()
{
    worker_thread1.start();
    worker_thread2.start();
    face_det_thread->start();
}


MyWidget::~MyWidget() {
    std::cout << "mywidget" << std::endl;
    delete ui;
}

void MyWidget::on_pb_history_clicked()
{
    HistoryWidget * w = new HistoryWidget;
    w->show();
    hide();
}

