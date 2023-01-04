#include "notifypage.h"
#include "common/config.h"
#include "ui_notifypage.h"
#include <QFont>
#include <QPainter>

NotifyPage::NotifyPage(QWidget *parent)
    : QFrame(parent), m_swCountDown(NOTIFY_TIMEOUT), m_pTimer(nullptr),
    ui(new Ui::NotifyPage) {
    ui->setupUi(this);
    m_pTimer = new QTimer(this);
    m_pTimer->start(UNIT_TIME/2);
    connect(m_pTimer, &QTimer::timeout, this, &NotifyPage::updateCountDown);
}

NotifyPage::~NotifyPage() {
    delete m_pTimer;
    delete ui;
}

void NotifyPage::setTipsBarBg(int index) {
    switch (index) {
    case 1:
        ui->mpBgFrame->setStyleSheet("QFrame#mpBgFrame { border-image: "
                                     "url(:/image/img_tipsbar_bg_normal.png); }");
        break;
    case 2:
        ui->mpBgFrame->setStyleSheet("QFrame#mpBgFrame { border-image: "
                                     "url(:/image/img_tipsbar_bg_warning.png); }");
        break;
    case 3:
        ui->mpBgFrame->setStyleSheet("QFrame#mpBgFrame { border-image: "
                                     "url(:/image/img_tipsbar_bg_error.png); }");
        break;
    default:
        ui->mpBgFrame->setStyleSheet(
            "QFrame#mpBgFrame { border-image: url(:/image/img_tipsbar_bg.png); }");
        break;
    }
}

void NotifyPage::setTipsColor(QString strColor) { m_Color = strColor; }

void NotifyPage::setTipsContent(QString strTips) { m_TipsContent = strTips; }

void NotifyPage::setUserName(QString strName) { m_UserName = strName; }

void NotifyPage::setUserId(QString strId) { m_UserId = strId; }

void NotifyPage::setUserHeadImg(QString strImgPath) { m_HeadImg = strImgPath; }

void NotifyPage::reSetTime()
{
    m_swCountDown = NOTIFY_TIMEOUT;
}

void NotifyPage::updateCountDown() {
    m_swCountDown -= UNIT_TIME/2;
    if (m_swCountDown <= 0) {
        emit finish_show_notify();
        m_swCountDown = NOTIFY_TIMEOUT;
    }
}

void NotifyPage::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QFont font;
    font.setPointSize(15);
    font.setWeight(QFont::Weight::Normal);
    painter.setFont(font);

    painter.setPen(QColor(255, 255, 0));

    QString color = "#" + m_Color;
    QPalette pe;
    pe.setColor(QPalette::WindowText, QColor(color.toStdString().c_str()));
    ui->mpNotifyLab->setPalette(pe);
    ui->mpNotifyLab->setText(m_TipsContent);
    ui->mpNameLab->setPalette(pe);
    ui->mpNameLab->setText(m_UserName);
    ui->mpIdLab->setPalette(pe);
    ui->mpIdLab->setText(m_UserId);

    //    // 加载头像路径
    //    QImage img;
    //    img.load(m_HeadImg);
    //    // 设置图片大小
    //    QImage imgscaled;
    //    imgscaled = img.scaled(192,192, Qt::IgnoreAspectRatio,
    //    Qt::SmoothTransformation); QPixmap picPixmap =
    //    QPixmap::fromImage(imgscaled); ui->mpHeadImg->setPixmap(picPixmap);
}
