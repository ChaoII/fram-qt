#pragma once

#include <QFrame>
#include <QTimer>


namespace Ui {
    class InputPage;
}

class InputPage : public QFrame {
Q_OBJECT

public:
    explicit InputPage(QWidget *parent = nullptr);

    QString strContent() { return password_content_; }

    void showInputWidget();

    void hideInputWidget();

    ~InputPage() override;

signals:

    void passwordAuthorized();

    void reSetCountDown(int swTimeOut);

    void inputWidgetVisibleSignal(bool is_visible);

private:
    void initButton();

    void pressNum(int num);

private slots:

    void on_pb_1_clicked();

    void on_pb_2_clicked();

    void on_pb_3_clicked();

    void on_pb_4_clicked();

    void on_pb_5_clicked();

    void on_pb_6_clicked();

    void on_pb_7_clicked();

    void on_pb_8_clicked();

    void on_pb_9_clicked();

    void on_pb_0_clicked();

    void on_pb_clear_clicked();

    void on_pb_ensure_clicked();

    void updateCountDown();

    void onReSetCountDown(int swTimeOut);

private:
    int current_timeout_;
    QTimer *counter_timer_;
    bool is_first_key_;
    QString password_content_;
    Ui::InputPage *ui;
};

