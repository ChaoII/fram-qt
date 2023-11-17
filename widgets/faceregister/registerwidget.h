#pragma once

#include <QWidget>
#include "utils/config.h"
#include <opencv2/opencv.hpp>

namespace Ui {
    class RegisterWidget;
}

struct RegisterFace {
    cv::Mat img;
    QString uid;
    QString name;
};

class RegisterWidget : public QWidget {
Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);

    ~RegisterWidget();

    void update_frame(const QImage &img);

signals:

    void register_finished_signal();

private slots:

    void on_tb_back_clicked();

    void on_tb_ensure_clicked();

    void on_tb_add_clicked();

private:

    void initialRegisterFaceListWidget();

    void clear_register_info();

private:
    Ui::RegisterWidget *ui;
    QImage img_;
    QVector<RegisterFace> register_face_;
};
