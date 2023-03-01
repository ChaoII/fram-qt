#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <common/utility.h>


namespace Ui {
class RegisterPage;
}

struct RegisterFaceInfo{
    cv::Mat img;
    QString staff_no;
    QString name;
};

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    void  update_frame(cv::Mat& mat);
    void clear_info();
    ~RegisterPage();

signals:
    void send_closed_signal();
    void add_db_finished_signal(int);

private slots:

    void on_pb_add_clicked();
    void on_pb_ensure_clicked();
    void on_pb_cancel_clicked();



private:

    void  init_lw();

    cv::Mat m_img;

    bool m_is_first_key;

    Ui::RegisterPage *ui;

    QVector<RegisterFaceInfo> m_register_face;
};

#endif // REGISTERPAGE_H
