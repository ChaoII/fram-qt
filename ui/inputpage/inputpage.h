#ifndef INPUTPAGE_H
#define INPUTPAGE_H

#include <QFrame>
#include <QTimer>


namespace Ui {
class InputPage;
}

class InputPage : public QFrame
{
    Q_OBJECT

public:
    explicit InputPage(QWidget *parent = nullptr);
    QString strContent(){ return m_strContent; }
    ~InputPage();

signals:
    void reSetCountDown(int swTimeOut);

private:
    void init();
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
    void on_pb_register_clicked();

    void updateCountDown();
    void onReSetCountDown(int swTimeOut);

private:

    int m_swCountDown;
    QTimer *m_pTimer;

    bool m_bIsFirstKey;

    QString m_strContent;
    Ui::InputPage *ui;
};

#endif // INPUTPAGE_H
