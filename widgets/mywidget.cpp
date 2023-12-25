//
// Created by aichao on 2022/5/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWidget.h" resolved

#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPainter>
#include <QTimer>
#include <QMetaType>
#include "widgets/mysplashscreen.h"
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

    // 当输入窗体或者其它窗体打开时，需要关闭人脸检测算法
    connect(ui->widget_input, &InputPage::inputWidgetVisibleSignal, this, [&](bool is_visible) {
        if (is_visible) {
            emit closeDetectorSignal();
        } else {
            if (ui->widget->isVisible()) {
                emit openDetectorSignal();
            }
        }
    });

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


    connect(ui->widget_input, &InputPage::passwordAuthorized, this, &MyWidget::on_receivePasswordAuthorized);

    MySplashScreen::getInstance().updateProcess("load outer socket component...");

    // 注册信号槽元对象
    qRegisterMetaType<FaceInfoWrap>();
    qRegisterMetaType<QVector<FaceInfoWrap>>();

    // socket
    auto out_socket = new OuterSocket(this);
    connect(out_socket, &OuterSocket::outerSocketCloseDetectorSignal, this,
            [&]() { emit closeDetectorSignal(); });
    connect(out_socket, &OuterSocket::outerSocketOpenDetectorSignal, this,
            [&]() { emit openDetectorSignal(); });

    MySplashScreen::getInstance().updateProcess("load camera device and ready to read frame...");

    // 视频解码及人脸检测线程
    auto face_det_thread = new FaceDetThread();
    face_det_thread->moveToThread(&face_det_thread_);
    connect(face_det_thread, &FaceDetThread::imgSendSignal, this, &MyWidget::on_updateFrame);
    connect(this, &MyWidget::runDetectThreadSignal, face_det_thread, &FaceDetThread::runDetect);
    connect(this, &MyWidget::stopDetectThreadSignal, face_det_thread, &FaceDetThread::stopThread);
    connect(this, &MyWidget::closeDetectorSignal, face_det_thread, &FaceDetThread::closeDetector,
            Qt::BlockingQueuedConnection);
    connect(this, &MyWidget::openDetectorSignal, face_det_thread, &FaceDetThread::openDetector,
            Qt::BlockingQueuedConnection);
    connect(&face_det_thread_, &QThread::finished, face_det_thread, &FaceDetThread::deleteLater);

    MySplashScreen::getInstance().updateProcess("load face recognition component...");
    // 人脸识别线程
    auto face_rec_thread = new FaceRecThread();
    face_rec_thread->moveToThread(&face_rec_thread_);
    connect(this, &MyWidget::sendImageSignal, face_rec_thread, &FaceRecThread::faceRecognition);
    connect(face_rec_thread, &FaceRecThread::faceRecognitionSignal, this, &MyWidget::on_faceRec);
    connect(&face_rec_thread_, &QThread::finished, face_rec_thread, &FaceRecThread::deleteLater);

    MySplashScreen::getInstance().updateProcess("load attend data record component...");

    // 声音播放线程
    auto audio_play_thread = new AudioPlayThread();
    audio_play_thread->moveToThread(&audio_play_thread_);
    connect(this, &MyWidget::faceRecognitionSuccessAudioSignal, audio_play_thread, &AudioPlayThread::playAudio);
    connect(audio_play_thread, &AudioPlayThread::playAudioFinished, this, &MyWidget::on_changeAudioPlayStatus);
    connect(&audio_play_thread_, &QThread::finished, audio_play_thread, &QThread::deleteLater);

    // 打卡记录线程
    auto record_thread = new RecordThread();
    record_thread->moveToThread(&attend_record_thread_);
    connect(face_rec_thread, &FaceRecThread::recordSignal, record_thread, &RecordThread::record);
    connect(&attend_record_thread_, &QThread::finished, record_thread, &QThread::deleteLater);

    MySplashScreen::getInstance().updateProcess("load face library component...");

    // 人脸库信息窗体
    face_info_widget_ = new FaceInfoWidget();
    connect(face_info_widget_, &FaceInfoWidget::face_back_signal, this, &MyWidget::on_otherWidgetFinished);

    MySplashScreen::getInstance().updateProcess("load attend history component...");

    // 打卡记录窗体
    history_widget_ = new HistoryWidget();
    connect(history_widget_, &HistoryWidget::historyBackSignal, this, &MyWidget::on_otherWidgetFinished);
    MySplashScreen::getInstance().updateProcess("init ui date time timer ...");

    // 设置窗体
    setting_widget_ = new SettingWidget();
    connect(setting_widget_, &SettingWidget::settingBackSignal, this, &MyWidget::on_otherWidgetFinished);
    connect(setting_widget_, &SettingWidget::restartProgramSignal, this, &MyWidget::close);


    // 界面时间
    auto timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &MyWidget::on_updateTime);
    timer1->start(1000);
    MySplashScreen::getInstance().updateProcess("init record  timer ...");

    // 记录线程
    auto timer2 = new QTimer(this);
    // 隔一段时间发送一组空的过去，避免数据堆积
    connect(timer2, &QTimer::timeout, [=]() { emit sendImageSignal(QImage(), QRect()); });
    timer2->start(Config::getInstance().get_recordInterval() * 1000);
    // 检测网络情况
    auto timer3 = new QTimer(this);
    connect(timer3, &QTimer::timeout, this, &MyWidget::on_detectNetworkConnectStatus);
    timer3->start(1000);
    ping_cmd_ = new QProcess(this);

    // 屏幕休眠
#ifdef __linux__
    auto timer4 = new QTimer(this);
    connect(timer4, &QTimer::timeout, this, [&]() {
        QDateTime cur_time = QDateTime::currentDateTime();
        if (last_rec_time_.secsTo(cur_time) > Config::getInstance().get_displayOffInterval()) {
            if (!is_display_off_) {
                QProcess::execute("xset", QStringList() << "dpms" << "force" << "off");
                is_display_off_ = true;
            }
        } else {
            if (is_display_off_) {
                QProcess::execute("xset", QStringList() << "dpms" << "force" << "on");
                is_display_off_ = false;
            }
        }
    });
    timer4->start(1000);
#endif
    // 初始化界面
    initialWidget();
    // 启动后台线程
    face_det_thread_.start();
    face_rec_thread_.start();
    attend_record_thread_.start();
    audio_play_thread_.start();
    emit runDetectThreadSignal();
    MySplashScreen::getInstance().updateProcess("init finished...");
}

void MyWidget::on_updateFrame(QImage qimg, QRect rect) {
    if (!rect.isEmpty()) {
        QDateTime cur_time = QDateTime::currentDateTime();
        // 通过计时调整识别的频率0.5秒识别一次
        if (last_rec_time_.msecsTo(cur_time) > static_cast<int>(Config::getInstance().get_recInterval() * 1000)) {
            emit sendImageSignal(qimg, rect);
            last_rec_time_ = cur_time;
        }
        QPainter painter(&qimg);
        painter.setPen(QPen(QColor(Qt::green)));
        painter.drawRect(rect);
    } else {
        utils::setBackgroundColor(ui->widget, QColor(255, 255, 255, 0));
    }
    ui->widget_info->setVisible(!rect.isEmpty());
    if (face_info_widget_ && face_info_widget_->isVisible()) {
        face_info_widget_->updateRegisterFrame(qimg);
    }
    if (!ui->widget->isVisible()) return;
    img_ = qimg.scaled(size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    update();
}

void MyWidget::on_faceRec(const FaceInfoWrap &rec_info) {

    QString attend_time = rec_info.time.split("T")[1].split(".")[0];
    if (rec_info.status == RecognitionStatus::SPOOF) {
        ui->widget_info->setAttendInfo("攻击人脸",
                                       attend_time,
                                       QPixmap(":img/icon_fail.png"),
                                       QPixmap(),
                                       QPixmap());

        utils::setBackgroundColor(ui->widget, QColor(255, 0, 0, 40));
        utils::setBackgroundColor(ui->widget_info, QColor(255, 0, 0, 40));
    } else if (rec_info.status == RecognitionStatus::Unknown) {
        ui->widget_info->setAttendInfo("未知", attend_time,
                                       QPixmap(":img/icon_fail.png"),
                                       QPixmap(),
                                       QPixmap());
        utils::setBackgroundColor(ui->widget, QColor(255, 255, 255, 0));
        utils::setBackgroundColor(ui->widget_info, QColor(255, 0, 0, 40));
    } else if (rec_info.status == RecognitionStatus::Success) {
        ui->widget_info->setAttendInfo(rec_info.ret.name, attend_time,
                                       QPixmap(":img/icon_success.png"),
                                       QPixmap(rec_info.ret.pic_url),
                                       QPixmap::fromImage(rec_info.ret.img));

        if (is_audio_finished_) {
            is_audio_finished_ = false;
            emit faceRecognitionSuccessAudioSignal();
        }
        utils::setBackgroundColor(ui->widget, QColor(255, 0, 0, 0));
        utils::setBackgroundColor(ui->widget_info, QColor(0, 255, 0, 40));
    } else {
        qDebug() << "识别结果为【None】";
    }
}

void MyWidget::on_pb_welcome_clicked() {
    current_clicked_button_ = ButtonsEnum::WelcomeButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_pb_register_clicked() {
    current_clicked_button_ = ButtonsEnum::RegisterButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_pb_history_clicked() {
    current_clicked_button_ = ButtonsEnum::HistoryButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_tb_close_clicked() {
    current_clicked_button_ = ButtonsEnum::CloseButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::on_tb_setting_clicked() {
    current_clicked_button_ = ButtonsEnum::SettingButton;
    if (ui->widget_input->isVisible()) {
        ui->widget_input->hideInputWidget();
        return;
    }
    ui->widget_input->showInputWidget();
}

void MyWidget::initialWidget() {
    layout()->addWidget(history_widget_);
    layout()->addWidget(face_info_widget_);
    layout()->addWidget(setting_widget_);
    history_widget_->setVisible(false);
    face_info_widget_->setVisible(false);
    setting_widget_->setVisible(false);
    ui->widget_input->setVisible(false);
    ui->widget_info->setVisible(false);
    ui->widget->setVisible(true);
}

void MyWidget::on_otherWidgetFinished() {
    emit openDetectorSignal();
    hideAllWidgets();
    ui->widget->setVisible(true);
}

void MyWidget::on_updateTime() {
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
    emit stopDetectThreadSignal();
    face_det_thread_.quit();
    face_det_thread_.wait();
    audio_play_thread_.quit();
    audio_play_thread_.wait();
    delete ui;
}


void MyWidget::hideAllWidgets() {
    if (ui->widget->isVisible()) {
        ui->widget->setVisible(false);
    }
    if (history_widget_->isVisible()) {
        history_widget_->setVisible(false);
    }
    if (face_info_widget_->isVisible()) {
        face_info_widget_->setVisible(false);
    }
    if (setting_widget_->isVisible()) {
        setting_widget_->setVisible(false);
    }
}

void MyWidget::on_receivePasswordAuthorized() {
    if (current_clicked_button_ == ButtonsEnum::HistoryButton) {
        hideAllWidgets();
        history_widget_->setVisible(true);
        history_widget_->updateHistoryWidget();
        emit closeDetectorSignal();
    } else if (current_clicked_button_ == ButtonsEnum::RegisterButton) {
        hideAllWidgets();
        face_info_widget_->setVisible(true);
        face_info_widget_->updateRegisterWidget();
        emit closeDetectorSignal();
    } else if (current_clicked_button_ == ButtonsEnum::WelcomeButton) {
        //todo other task such as open the door
    } else if (current_clicked_button_ == ButtonsEnum::CloseButton) {
        close();
    } else if (current_clicked_button_ == ButtonsEnum::SettingButton) {
        hideAllWidgets();
        setting_widget_->setVisible(true);
        emit closeDetectorSignal();
    } else {
        // todo
    }
}


void MyWidget::on_changeAudioPlayStatus() {
    is_audio_finished_ = true;
}

void MyWidget::on_detectNetworkConnectStatus() {
    QStringList ping_parameters;
    ping_parameters << Config::getInstance().get_gateway();
#if defined(__linux__) || defined(__APPLE__)
    ping_parameters << "-c";
#else
    ping_parameters << "-n";
#endif
    ping_parameters << "1";
    ping_cmd_->start("ping", ping_parameters);
    bool connect_status = false;
#if defined(__linux__) || defined(__APPLE__)
    QString key_str = "ttl";
#else
    QString key_str = "TTL";
#endif
    if (ping_cmd_->waitForFinished(200)) {
        QString res = QString::fromLocal8Bit(ping_cmd_->readAll());
        if (res.indexOf(key_str) != -1) {
            connect_status = true;
        }
    }
    auto pic_url = connect_status ? ":img/icon_wifi_connect.png" : ":img/icon_wifi_disconnect.png";
    ui->lb_net->setPixmap(QPixmap(pic_url));
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        last_rec_time_ = QDateTime::currentDateTime();
    }
    QWidget::mousePressEvent(event);
}


