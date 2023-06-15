//
// Created by aichao on 2022/5/11.
//

#ifndef FRAM_MYWIDGET_H
#define FRAM_MYWIDGET_H

#include <QWidget>
#include "utils/Utils.h"
#include "core/facerecthread.h"
#include "core/facedetthread.h"
#include "core/recordthread.h"
#include "widgets/faceinfowidget.h"
#include "widgets/historywidget.h"
#include <QMutex>
#include <QMutexLocker>
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE


class MyWidget : public QWidget {
Q_OBJECT

public:

    explicit MyWidget(QWidget *parent = nullptr);

    void update_frame(QImage qimg, QRect rect);

    void on_face_rec(FaceInfoWrap rec_info);

    void on_update_time();

    void start_thread();

    void run();

    void paintEvent(QPaintEvent *event) override;

    ~MyWidget() override;

signals:

    void send_img_signal(QImage qimg, QRect rect);


private slots:

    void on_pb_history_clicked();

    void on_pb_register_clicked();

private:

    void init_widget();

    void hide_all_widgets();

    void on_face_finished();

    void on_history_finished();


private:
    QImage img_;
    QDateTime last_rec_time = QDateTime::currentDateTime();
    QThread worker_thread1;
    QThread worker_thread2;
    HistoryWidget* history_widget = nullptr;
    FaceInfoWidget *face_info_widget = nullptr;
    FaceDetThread *face_det_thread = nullptr;
    Ui::MyWidget *ui;
};


#endif //FRAM_MYWIDGET_H
