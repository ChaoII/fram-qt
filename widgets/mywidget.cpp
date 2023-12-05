//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWidget.h" resolved

#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPainter>
#include <QTimer>
#include <QHostInfo>
#include <QMetaType>
#include "widgets/MySplashScreen.h"
#include "widgets/attendhistory/historywidget.h"


MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);
    this->ui->tb_close->setVisible(false);
    this->setAttribute(Qt::WA_QuitOnClose);
    if (Config::getInstance().get_framelessStatus()) {
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->ui->tb_close->setVisible(true);
    }
    this->setWindowFlags(windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":img/icon-64x64.png"));

    ui->mpTimeLab->setStyleSheet("QLabel{font-size:28px;color:rgb(200,200,200);}");
    ui->mpDateLab->setStyleSheet("QLabel{font-size:28px;color:rgb(200,200,200);}");
    ui->mpWeekLab->setStyleSheet("QLabel{font-size:28px;color:rgb(200,200,200);}");

    ui->pb_welcome->setStyleSheet(
            "QPushButton { border-radius:120px; }\n"
            "QPushButton:hover { background-color: #181d24ff;}\n"
            "QPushButton:pressed {background-color: #18dd24ff; }");

    ui->pb_history->setStyleSheet(
            "QPushButton { border-radius:20px; }\n"
            "QPushButton {border-image:url(:img/icon_attendHistory.png)}\n"
            "QPushButton:hover:!pressed {border-image:url(:img/icon_attendHistory.png);background-color: #181d24ff; }\n"
            "QPushButton:hover:pressed{border-image:url(:img/icon_attendHistory_pressed.png);background-color: #18dd24ff;}\n");

    ui->pb_register->setStyleSheet(
            "QPushButton { border-radius:20px; }\n"
            "QPushButton {border-image:url(:img/icon_face.png)}\n"
            "QPushButton:hover:!pressed{border-image:url(:img/icon_face.png);background-color: #181d24ff; }\n"
            "QPushButton:hover:pressed{border-image:url(:img/icon_face_pressed.png);background-color: #18dd24ff;}\n");

    ui->gridLayout->setAlignment(ui->pb_history, Qt::AlignHCenter | Qt::AlignVCenter);
    ui->gridLayout->setAlignment(ui->pb_register, Qt::AlignHCenter | Qt::AlignVCenter);
    ui->gridLayout->setAlignment(ui->pb_welcome, Qt::AlignHCenter | Qt::AlignTop);
    ui->gridLayout->setAlignment(ui->mpWeekLab, Qt::AlignHCenter | Qt::AlignVCenter);
    ui->gridLayout->setAlignment(ui->mpDateLab, Qt::AlignHCenter | Qt::AlignVCenter);
    ui->gridLayout->setAlignment(ui->mpTimeLab, Qt::AlignHCenter | Qt::AlignVCenter);


    connect(ui->widget_input, &InputPage::passwordAuthorized, this, &MyWidget::on_receive_password_authorized);

    MySplashScreen::getInstance().update_process("load outer socket component...");

    // 注册信号槽元对象
    qRegisterMetaType<FaceInfoWrap>();
    qRegisterMetaType<QVector<FaceInfoWrap>>();

    // socket
    auto out_socket = new OuterSocket(this);
    connect(out_socket, &OuterSocket::outer_socket_close_detector_signal,
            [this]() { emit close_detector_signal(); });
    connect(out_socket, &OuterSocket::outer_socket_open_detector_signal,
            [this]() { emit open_detector_signal(); });


    MySplashScreen::getInstance().update_process("load camera device and ready to read frame...");

    // 视频解码及人脸检测线程
    auto face_det_thread = new FaceDetThread();
    face_det_thread->moveToThread(&face_det_thread_);
    connect(face_det_thread, &FaceDetThread::img_send_signal, this, &MyWidget::update_frame);
    connect(this, &MyWidget::run_detect_thread_signal, face_det_thread, &FaceDetThread::run_detect);
    connect(this, &MyWidget::stop_detect_thread_signal, face_det_thread, &FaceDetThread::stop_thread);
    connect(this, &MyWidget::close_detector_signal, face_det_thread, &FaceDetThread::close_detector);
    connect(this, &MyWidget::open_detector_signal, face_det_thread, &FaceDetThread::open_detector);
    connect(&face_det_thread_, &QThread::finished, face_det_thread, &FaceDetThread::deleteLater);

    MySplashScreen::getInstance().update_process("load face recognition component...");
    // 人脸识别线程
    auto face_rec_thread = new FaceRecThread();
    face_rec_thread->moveToThread(&face_rec_thread_);
    connect(this, &MyWidget::send_img_signal, face_rec_thread, &FaceRecThread::face_recognition);
    connect(face_rec_thread, &FaceRecThread::face_rec_signal, this, &MyWidget::on_face_rec);
    connect(&face_rec_thread_, &QThread::finished, face_rec_thread, &FaceRecThread::deleteLater);

    MySplashScreen::getInstance().update_process("load attend data record component...");

    // 声音播放线程
    auto audio_play_thread = new AudioPlayThread();
    audio_play_thread->moveToThread(&audio_play_thread_);
    connect(this, &MyWidget::face_recognition_success_audio_signal, audio_play_thread, &AudioPlayThread::playAudio);
    connect(audio_play_thread, &AudioPlayThread::playAudioFinished, this, &MyWidget::on_changeAudioPlayStatus);
    connect(&audio_play_thread_, &QThread::finished, audio_play_thread, &QThread::deleteLater);

    // 打卡记录线程
    auto record_thread = new RecordThread();
    record_thread->moveToThread(&attend_record_thread_);
    connect(face_rec_thread, &FaceRecThread::record_signal, record_thread, &RecordThread::record);
    connect(&attend_record_thread_, &QThread::finished, record_thread, &QThread::deleteLater);

    MySplashScreen::getInstance().update_process("load face library component...");

    // 人脸库信息窗体
    face_info_widget = new FaceInfoWidget();
    connect(face_info_widget, &FaceInfoWidget::face_back_signal, this, &MyWidget::on_face_finished);

    MySplashScreen::getInstance().update_process("load attend history component...");

    // 打卡记录窗体
    history_widget = new HistoryWidget();
    connect(history_widget, &HistoryWidget::history_back_signal, this, &MyWidget::on_history_finished);
    MySplashScreen::getInstance().update_process("init ui date time timer ...");
    // 界面时间
    auto timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &MyWidget::on_update_time);
    timer1->setInterval(1000);
    timer1->start();
    MySplashScreen::getInstance().update_process("init record  timer ...");
    // 记录线程
    auto timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, [=]() { emit send_img_signal(QImage(), QRect()); });
    timer2->setInterval(Config::getInstance().get_record_interval() * 1000);
    timer2->start();
    // 检测网络情况
    auto timer3 = new QTimer(this);
    connect(timer3, &QTimer::timeout, this, &MyWidget::on_detectNetworkConnectStatus);
    timer3->start(1000);
    pingCmd = new QProcess(this);

    // 屏幕休眠
#ifdef __linux__
    auto timer4 = new QTimer(this);
    connect(timer4, &QTimer::timeout, this, [&]() {
        QDateTime cur_time = QDateTime::currentDateTime();
        if (last_rec_time.secsTo(cur_time) > Config::getInstance().get_displayOffInterval()) {
            QProcess::execute("xset", QStringList() << "dpms" << "force" << "off");
        } else {
            QProcess::execute("xset", QStringList() << "dpms" << "force" << "on");
        }
    });
    timer4->start(1000);
#endif


    // 初始化界面
    init_widget();
    // 启动后台线程
    face_det_thread_.start();
    face_rec_thread_.start();
    attend_record_thread_.start();
    audio_play_thread_.start();
    emit run_detect_thread_signal();
    MySplashScreen::getInstance().update_process("init finished...");
}

void MyWidget::update_frame(QImage qimg, QRect rect) {
    if (!rect.isEmpty()) {
        QDateTime cur_time = QDateTime::currentDateTime();
        // 通过计时调整识别的频率
        if (last_rec_time.msecsTo(cur_time) > Config::getInstance().get_rec_interval()) {
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
    }
    if (!ui->widget->isVisible()) return;
    img_ = qimg.scaled(size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    update();
}

void MyWidget::on_face_rec(const FaceInfoWrap &rec_info) {

    QString attend_time = rec_info.time.split("T")[1].split(".")[0];
    if (rec_info.status == RecognitionStatus::SPOOF) {
        ui->widget_info->setAttendInfo("攻击人脸",
                                       attend_time,
                                       QPixmap(":img/icon_fail.png"),
                                       QPixmap(),
                                       QPixmap());

        Utils::setBackgroundColor(ui->widget, QColor(255, 0, 0, 40));
        Utils::setBackgroundColor(ui->widget_info, QColor(255, 0, 0, 40));
    } else if (rec_info.status == RecognitionStatus::Unknown) {
        ui->widget_info->setAttendInfo("未知", attend_time,
                                       QPixmap(":img/icon_fail.png"),
                                       QPixmap(),
                                       QPixmap());
        Utils::setBackgroundColor(ui->widget, QColor(255, 255, 255, 0));
        Utils::setBackgroundColor(ui->widget_info, QColor(255, 0, 0, 40));
    } else if (rec_info.status == RecognitionStatus::Success) {
        ui->widget_info->setAttendInfo(rec_info.ret.name, attend_time,
                                       QPixmap(":img/icon_success.png"),
                                       QPixmap(rec_info.ret.pic_url),
                                       QPixmap::fromImage(rec_info.ret.img));

        if (isAudioFinished) {
            isAudioFinished = false;
            emit face_recognition_success_audio_signal();
        }
        Utils::setBackgroundColor(ui->widget, QColor(255, 0, 0, 0));
        Utils::setBackgroundColor(ui->widget_info, QColor(0, 255, 0, 40));
    } else {
        qDebug() << "识别结果为【None】";
    }
}

void MyWidget::on_pb_welcome_clicked() {
    currentClickedButton_ = ButtonsEnum::WelcomeButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_pb_register_clicked() {
    currentClickedButton_ = ButtonsEnum::RegisterButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_pb_history_clicked() {
    currentClickedButton_ = ButtonsEnum::HistoryButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_tb_close_clicked() {
    currentClickedButton_ = ButtonsEnum::CloseButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::init_widget() {
    layout()->addWidget(history_widget);
    layout()->addWidget(face_info_widget);
    history_widget->setVisible(false);
    face_info_widget->setVisible(false);
    ui->widget_input->setVisible(false);
    ui->widget_info->setVisible(false);
    ui->widget->setVisible(true);
}

void MyWidget::on_face_finished() {
    emit open_detector_signal();
    hide_all_widgets();
    ui->widget->setVisible(true);
}

void MyWidget::on_history_finished() {
    hide_all_widgets();
    ui->widget->setVisible(true);
}

void MyWidget::on_update_time() {
    auto data_time = QDateTime::currentDateTime();
    QString date = data_time.toString("yyyy-MM-dd");
    QString time = data_time.toString("HH:mm:ss");
    //指定中文显示
    QLocale locale = QLocale::Chinese;
    ui->mpDateLab->setText(date);
    ui->mpWeekLab->setText(locale.toString(data_time, QString("dddd")));
    ui->mpTimeLab->setText(time);
}


void MyWidget::paintEvent(QPaintEvent *event) {
    if (!ui->widget->isVisible()) return;
    QPainter painter(this);
    if (img_.isNull()) {
        // 绘制一个灰色的矩形
        QRect rectangle(0, 0, this->width(), this->height());
        painter.setBrush(QColor(30, 30, 30));
        painter.drawRect(rectangle);
    }
    painter.drawImage(0, 0, img_);
}


MyWidget::~MyWidget() {
    face_rec_thread_.quit();
    face_rec_thread_.wait();
    attend_record_thread_.quit();
    attend_record_thread_.wait();
    // 必须先将死循环退出后才能结束线程
    emit stop_detect_thread_signal();
    face_det_thread_.quit();
    face_det_thread_.wait();
    delete ui;
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

void MyWidget::on_receive_password_authorized() {
    if (currentClickedButton_ == ButtonsEnum::HistoryButton) {
        hide_all_widgets();
        history_widget->setVisible(true);
        history_widget->update_history_widget();
    } else if (currentClickedButton_ == ButtonsEnum::RegisterButton) {
        hide_all_widgets();
        face_info_widget->setVisible(true);
        face_info_widget->update_register_widget();
        emit close_detector_signal();
    } else if (currentClickedButton_ == ButtonsEnum::WelcomeButton) {
        //todo other task such as open the door
    } else if (currentClickedButton_ == ButtonsEnum::CloseButton) {
        close();
    } else {
        // todo
    }
}


void MyWidget::on_changeAudioPlayStatus() {
    isAudioFinished = true;
}

void MyWidget::on_detectNetworkConnectStatus() {
    QStringList pingParameters;
    pingParameters << Config::getInstance().get_gateway();
#if defined(__linux__) || defined(__APPLE__)
    pingParameters << "-c";
#else
    pingParameters << "-n";
#endif
    pingParameters << "1";
    pingCmd->start("ping", pingParameters);
    bool connectStatus = false;
#if defined(__linux__) || defined(__APPLE__)
    QString keyStr = "ttl";
#else
    QString keyStr = "TTL";
#endif
    if (pingCmd->waitForFinished(200)) {
        QString res = QString::fromLocal8Bit(pingCmd->readAll());
        if (res.indexOf(keyStr) != -1) {
            connectStatus = true;
        }
    }
    auto pic_url = connectStatus ? ":img/icon_wifi_connect.png" : ":img/icon_wifi_disconnect.png";
    ui->lb_net->setPixmap(QPixmap(pic_url));
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        last_rec_time = QDateTime::currentDateTime();
    }
    QWidget::mousePressEvent(event);
}







