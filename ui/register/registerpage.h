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

    void on_pb_ensure_clicked();


private:

    void  init_lw();

    cv::Mat m_img;

    bool m_is_first_key;

    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
