#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <common/utility.h>


namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    void  update_frame(cv::Mat& mat);
    ~RegisterPage();

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
    void on_pb_ensure_clicked();
    void on_pb_delete_clicked();

private:
    void press_num(int num);

    void  init_lw();

    cv::Mat m_img;

    bool m_is_first_key;

    QString m_str_content;

    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
