//
// Created by aichao on 2022/5/11.
//

#pragma once

#include <QWidget>
#include "utils/Utils.h"
#include "core/facerecthread.h"
#include "core/facedetthread.h"
#include "core/recordthread.h"
#include "widgets/faceregister/faceinfowidget.h"
#include "widgets/attendhistory/historywidget.h"
#include <QMutex>
#include <QMutexLocker>
#include <QCloseEvent>
#include "core/outersocket.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE


class MyWidget : public QWidget {
Q_OBJECT

    enum class ButtonsEnum {
        RegisterButton,
        HistoryButton,
        WelcomeButton,
        None,
    };

public:

    explicit MyWidget(QWidget *parent = nullptr);

    void update_frame(QImage qimg, QRect rect);

    void on_face_rec(const FaceInfoWrap &rec_info);

    void on_update_time();

    void run();

    void paintEvent(QPaintEvent *event) override;

    ~MyWidget() override;

signals:

    void send_img_signal(QImage qimg, QRect rect);

    void run_detect_thread_signal();

    void stop_detect_thread_signal();

    void close_detector_signal();

    void open_detector_signal();


private slots:

    void on_pb_history_clicked();

    void on_pb_register_clicked();

    void on_pb_welcome_clicked();

    void on_receive_password_authorized();

private:

    void init_widget();

    void hide_all_widgets();

    void on_face_finished();

    void on_history_finished();


private:
    QImage img_;
    QDateTime last_rec_time = QDateTime::currentDateTime();
    QThread face_det_thread_;
    QThread face_rec_thread_;
    QThread attend_record_thread_;

    ButtonsEnum currentClickedButton_ = ButtonsEnum::None;
    HistoryWidget *history_widget = nullptr;
    FaceInfoWidget *face_info_widget = nullptr;
    Ui::MyWidget *ui;
};

