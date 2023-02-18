#include "widget.h"
#include "common/utility.h"
#include "ui_widget.h"
#include <QPalette>
#include <QWidget>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    setWindowFlag(Qt::FramelessWindowHint, true);
    ui->setupUi(this);
    m_camera_thread = new CameraReaderThread(this->size());
    m_camera_thread->start();
    m_rec_thread = new FaceRecognitinThread();
    connect(m_camera_thread, &CameraReaderThread::decodeimage, this,
            &Widget::update_image);
    connect(m_rec_thread, &FaceRecognitinThread::rec_result, this,
            &Widget::show_result);
    initWidget();
}

Widget::~Widget() { delete ui; }

void Widget::update_image(cv::Mat &mat) {
    m_img = mat;
    if (!this->m_draw_image) {
        m_register_page->update_frame(m_img);
    } else {
        m_rec_thread->set_frame(mat);
        m_rec_thread->start();
    }
}

void Widget::initWidget() {
    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0); //注意这里设置了所有参数页面的边框
    // left
    ui->mpDateLab->setStyleSheet("QLabel { font: 20px ; color: rgb(245, 245, "
                                 "245); background-color: transparent; }");
    ui->mpWeekLab->setStyleSheet("QLabel { font: 18px ; color: rgb(245, 245, "
                                 "245); background-color: transparent; }");
    // right
    ui->mpTimeLab->setStyleSheet("QLabel { font: 26px ; color: rgb(245, 245, "
                                 "245); background-color: transparent; }");
    ui->pb_register->setStyleSheet(
        "QPushButton { font: 26px ; color: rgb(245, 245, 245); background-color: "
        "transparent; }\n"
        "QPushButton:hover { font: 26px ; color: rgb(245, 245, 245); "
        "background-color: #181d24ff; }\n"
        "QPushButton:pressed { font: 26px ; color: rgb(245, 245, 245); "
        "background-color: #18dd24ff; }");

    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &Widget::updateDateTime);
    m_pTimer->start(1000);

    m_standby_page = new StandbyPage;
    m_input_page = new InputPage;
    m_notify_page = new NotifyPage;
    m_register_page = new RegisterPage;

    m_layout->addWidget(m_standby_page);
    m_layout->addWidget(m_input_page);
    m_layout->addWidget(m_notify_page);
    m_layout->addWidget(m_register_page);

    ui->frameMain->setLayout(m_layout);
    switchToStandbyPage();
    connect(m_notify_page, &NotifyPage::finish_show_notify, this, [=]() {
        if (m_current_page_flag == PageNotify) {
            switchToStandbyPage();
        }
    });
}

int Widget::hide_all_windows() {
    if (m_standby_page) {
        m_standby_page->hide();
    }
    if (m_input_page) {
        m_input_page->hide();
    }
    if (m_notify_page) {
        m_notify_page->hide();
    }
    if (m_register_page) {
        m_register_page->hide();
    }
    m_current_page_flag = PageNONE;

    return 0;
}

void Widget::switchToStandbyPage() {
    hide_all_windows();
    if (m_standby_page) {
        m_standby_page->show();
        m_current_page_flag = PageStandby;
    }
}

void Widget::switchToInputPage() {
    hide_all_windows();
    if (m_input_page) {
        m_input_page->show();
        m_current_page_flag = PageInput;
    }
}

void Widget::switchToNotifyPage() {
    hide_all_windows();
    if (m_notify_page) {
        m_notify_page->show();
        m_current_page_flag = PageNotify;
    }
}

void Widget::switchToRegisterPage() {
    hide_all_windows();
    if (m_register_page) {
        m_register_page->show();
        m_current_page_flag = PageRegister;
    }
}

void Widget::on_mpStandbyBtn_clicked() {
    if (m_current_page_flag == PageRegister) {
        this->m_draw_image = true;
        switchToStandbyPage();
    } else if (m_current_page_flag != PageInput) {
        switchToInputPage();
    } else {
        switchToStandbyPage();
    }
}

void Widget::on_pb_register_clicked() {
    if (PageRegister != m_current_page_flag) {
        this->m_draw_image = false;
        switchToRegisterPage();
    } else {
        this->m_draw_image = true;
        switchToStandbyPage();
    }
}

void Widget::updateDateTime() {
    auto data_time = QDateTime::currentDateTime();
    QString date = data_time.toString("yyyy-MM-dd");
    QString time = data_time.toString("HH:mm:ss");
    //指定中文显示
    QLocale locale = QLocale::Chinese;
    ui->mpDateLab->setText(date);
    ui->mpWeekLab->setText(locale.toString(data_time, QString("dddd")));
    ui->mpTimeLab->setText(time);
}

void Widget::show_result(const FaceResult &result) {
    if (m_current_page_flag == PageStandby) {
        switchToNotifyPage();
    }
    if (m_notify_page) {
        m_notify_page->setTipsColor("FFFFFF");
        m_notify_page->setUserHeadImg("");
        m_notify_page->setUserId(result.uid);
        m_notify_page->setUserName(result.name);
        m_notify_page->setTipsContent("通知: 识别成功");
        m_notify_page->setTipsBarBg(1);
        m_notify_page->reSetTime();
    }
}

void Widget::paintEvent(QPaintEvent *event) {

    QImage img = Utility::Mat2QImage(this->m_img);
    if (this->m_draw_image) {
        QPainter painter(this);
        painter.drawImage(0, 0, img);
        update();
    }
}
