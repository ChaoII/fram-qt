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

    // socket
    out_socket = new OuterSocket(this);
    connect(out_socket, &OuterSocket::outer_socket_close_detector_signal,
            [this]() { qDebug() << "mywindow: send close detector"; emit close_detector_signal(); });
    connect(out_socket, &OuterSocket::outer_socket_open_detector_signal,
            [this]() { qDebug() << "mywindow: send open detector"; emit open_detector_signal(); });

    qDebug() << "main:" << QThread::currentThreadId();

    // 视频解码及人脸检测线程
    auto face_det_thread = new FaceDetThread();
    face_det_thread->moveToThread(&face_det_thread_);
    connect(face_det_thread, &FaceDetThread::img_send_signal, this, &MyWidget::update_frame);
    connect(this, &MyWidget::run_detect_thread_signal, face_det_thread, &FaceDetThread::run_detect);
    connect(this, &MyWidget::stop_detect_thread_signal, face_det_thread, &FaceDetThread::stop_thread);
    connect(this, &MyWidget::close_detector_signal, face_det_thread, &FaceDetThread::close_detector);
    connect(this, &MyWidget::open_detector_signal, face_det_thread, &FaceDetThread::open_detector);

    // 人脸识别线程
    auto face_rec_thread = new FaceRecThread();
    face_rec_thread->moveToThread(&face_rec_thread_);

    // 打卡记录线程
    auto record_thread = new RecordThread();
    record_thread->moveToThread(&attend_record_thread_);
    // connect(&worker_thread1,&QThread::finished,face_rec_thread,&QObject::deleteLater);
    connect(this, &MyWidget::send_img_signal, face_rec_thread, &FaceRecThread::face_recognition, Qt::QueuedConnection);

    // 注册信号槽元对象
    qRegisterMetaType<FaceInfoWrap>();
    qRegisterMetaType<QVector<FaceInfoWrap>>();
    connect(face_rec_thread, &FaceRecThread::face_rec_signal, this, &MyWidget::on_face_rec);
    connect(face_rec_thread, &FaceRecThread::record_signal, record_thread, &RecordThread::record);

    // 人脸库信息窗体
    face_info_widget = new FaceInfoWidget();
    connect(face_info_widget, &FaceInfoWidget::face_back_signal, this, &MyWidget::on_face_finished);

    // 打卡记录窗体
    history_widget = new HistoryWidget();
    connect(history_widget, &HistoryWidget::history_back_signal, this, &MyWidget::on_history_finished);

    // 界面时间
    auto timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &MyWidget::on_update_time);
    timer1->setInterval(1000);
    timer1->start();

    // 记录线程
    auto timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, [=]() { emit send_img_signal(QImage(), QRect()); });
    timer2->setInterval(Config::getInstance()->getRecord_interval() * 1000);
    timer2->start();


    // 初始化界面
    init_widget();
    run();
}

void MyWidget::update_frame(QImage qimg, QRect rect) {

    if (!rect.isEmpty()) {
        QDateTime cur_time = QDateTime::currentDateTime();
        // 通过计时调整识别的频率
        if (last_rec_time.msecsTo(cur_time) > Config::getInstance()->getRec_interval()) {
            emit send_img_signal(qimg, rect);
            last_rec_time = cur_time;
        }
        QPainter painter(&qimg);
        painter.setPen(QPen(QColor(Qt::green)));
        painter.drawRect(rect);
    } else {
        Utils::setBackgroundColor(ui->widget, QColor(255, 255, 255, 0));
    }
    ui->widget_info->setVisible(!rect.isEmpty());
    if (face_info_widget && face_info_widget->isVisible()) {
        face_info_widget->update_register_frame(qimg);
    };
    if (!ui->widget->isVisible()) return;
    img_ = qimg.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
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

void MyWidget::on_pb_register_clicked() {
    hide_all_widgets();
    face_info_widget->setVisible(true);
    face_info_widget->update_register_widget();
    face_rec_thread_.quit();
    face_rec_thread_.wait();
    emit close_detector_signal();
}

void MyWidget::init_widget() {
    layout()->addWidget(history_widget);
    layout()->addWidget(face_info_widget);
    hide_all_widgets();
    history_widget->setVisible(false);
    history_widget->setVisible(false);
    face_info_widget->setVisible(false);
    ui->widget->setVisible(true);
}

void MyWidget::on_face_finished() {
    emit open_detector_signal();
    hide_all_widgets();
    ui->widget->setVisible(true);
    if (!face_rec_thread_.isRunning()) {
        qDebug() << "start worker_thread1";
        face_rec_thread_.start();
        qDebug() << "worker_thread1 is running: " << face_rec_thread_.isRunning();
    }
}

void MyWidget::on_history_finished() {
    hide_all_widgets();
    ui->widget->setVisible(true);
}

void MyWidget::on_update_time() {
    ui->lb_cur_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}


void MyWidget::paintEvent(QPaintEvent *event) {
    if (!ui->widget->isVisible()) return;
    if (img_.isNull()) return;
    QPainter painter(this);
    painter.drawImage(0, 0, img_);
}


void MyWidget::run() {
    face_det_thread_.start();
    face_rec_thread_.start();
    attend_record_thread_.start();
    emit run_detect_thread_signal();
}

MyWidget::~MyWidget() {
    face_rec_thread_.quit();
    face_rec_thread_.wait();
    attend_record_thread_.quit();
    attend_record_thread_.wait();
    emit stop_detect_thread_signal();
    face_det_thread_.quit();
    face_det_thread_.wait();
    delete ui;
}

void MyWidget::on_pb_history_clicked() {
    hide_all_widgets();
    history_widget->setVisible(true);
    history_widget->update_history_widget();
}

void MyWidget::hide_all_widgets() {
    if (ui->widget->isVisible()) {
        ui->widget->setVisible(false);
    }
    if (history_widget->isVisible()) {
        history_widget->setVisible(false);
    }
    if (face_info_widget->isVisible()) {
        face_info_widget->setVisible(false);
    }
}

