//
// Created by AC on 2023/11/14.
//

#include "mysplashscreen.h"
#include <QtWidgets>

MySplashScreen::MySplashScreen(const QPixmap &pix) : QSplashScreen(pix) {
    installEventFilter(this);//设置事件发送
    progress_bar_ = new QProgressBar(this);
    progress_bar_->setRange(0, 15);
    progress_bar_->setTextVisible(false);
    progress_bar_->setGeometry(0, 290, 300, 5);
}

void MySplashScreen::mySleep(int ms) {
    QTime time = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < time) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MySplashScreen::setRange(int min, int max) {
    progress_bar_->setRange(min, max);
}

void MySplashScreen::updateProcess(const QString &msg) {
    QApplication::processEvents();
    progress_bar_->setValue(++value_);
//    this->showMessage(msg, Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    mySleep(50);
    qDebug() << value_;
}

// 使用事件过滤器，防止进度条在加载中的时候，用户点击了图片，造成窗口被隐藏 // 可以去除体验效果
bool MySplashScreen::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Type::MouseButtonPress || event->type() == QEvent::Type::MouseButtonDblClick) {
        return true;
    }
    return false;
}


