//
// Created by aichao on 2022/5/11.
//

#pragma once

#include <QWidget>
#include "utils/utils.h"
#include "core/facerecthread.h"
#include "core/facedetthread.h"
#include "core/recordthread.h"
#include "core/audioplaythread.h"
#include "widgets/faceregister/faceinfowidget.h"
#include "widgets/attendhistory/historywidget.h"
#include <QMutex>
#include <QProcess>
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
        CloseButton,
        None,
    };

public:

    explicit MyWidget(QWidget *parent = nullptr);

    ~MyWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

    void sendImageSignal(QImage qimg, QRect rect);

    void runDetectThreadSignal();

    void stopDetectThreadSignal();

    void closeDetectorSignal();

    void openDetectorSignal();

    void faceRecognitionSuccessAudioSignal();


private slots:

    void on_pb_history_clicked();

    void on_pb_register_clicked();

    void on_pb_welcome_clicked();

    void on_tb_close_clicked();

    void on_updateFrame(QImage qimg, QRect rect);

    void on_faceRec(const FaceInfoWrap &rec_info);

    void on_updateTime();

    void on_receivePasswordAuthorized();

    void on_changeAudioPlayStatus();

    void on_detectNetworkConnectStatus();

    void on_faceFinished();

    void on_historyFinished();

    void mousePressEvent(QMouseEvent *event) override;

private:

    void initialWidget();

    void hideAllWidgets();


private:
    QImage img_;
    QDateTime last_rec_time_ = QDateTime::currentDateTime();
    QThread face_det_thread_;
    QThread face_rec_thread_;
    QThread attend_record_thread_;
    QThread audio_play_thread_;
    ButtonsEnum current_clicked_button_ = ButtonsEnum::None;
    HistoryWidget *history_widget_ = nullptr;
    FaceInfoWidget *face_info_widget_ = nullptr;
    QProcess *ping_cmd_ = nullptr;
    bool is_audio_finished_ = true;
    Ui::MyWidget *ui;

};

