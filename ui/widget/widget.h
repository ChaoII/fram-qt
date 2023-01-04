#ifndef WIDGET_H
#define WIDGET_H

#include "core/camerareader.h"
#include "core/facerecognition.h"
#include "ui/inputpage/inputpage.h"
#include "ui/notifypage/notifypage.h"
#include "ui/register/registerpage.h"
#include "ui/standbypage/standbypage.h"
#include <QString>
#include <QtWidgets>

class QHBoxLayout;

enum PageType {
    PageNONE = 0,
    PageStandby,
    PageInput,
    PageNotify,
    PageRegister
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void on_mpStandbyBtn_clicked();
    void on_pb_register_clicked();
    void update_image(cv::Mat &mat);
    void updateDateTime();
    void show_result(const FaceResult &result);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initWidget();

    // Pages
    int hide_all_windows();
    void switchToStandbyPage();
    void switchToRegisterPage();
    void switchToInputPage();
    void switchToNotifyPage();

    // Pages Manager
    cv::Mat m_img;
    QHBoxLayout *m_layout = nullptr;
    int m_current_page_flag;
    StandbyPage *m_standby_page = nullptr;
    InputPage *m_input_page = nullptr;
    NotifyPage *m_notify_page = nullptr;
    RegisterPage *m_register_page = nullptr;
    QTimer *m_pTimer = nullptr;
    bool m_draw_image = true;
    CameraReaderThread *m_camera_thread = nullptr;
    FaceRecognitinThread *m_rec_thread = nullptr;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
