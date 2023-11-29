//
// Created by AC on 2023/11/14.
//

#include "MySplashScreen.h"
#include <QtWidgets>

MySplashScreen::MySplashScreen(const QPixmap &pix) : QSplashScreen(pix) {
    installEventFilter(this);//设置事件发送
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 15);
    progressBar->setTextVisible(false);
    progressBar->setGeometry(0, 290, 300, 5);
}

void MySplashScreen::mySleep(int ms) {
    QTime time = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < time) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MySplashScreen::setRange(int min, int max) {
    progressBar->setRange(min, max);
}

void MySplashScreen::update_process(const QString &msg) {
    QApplication::processEvents();
    progressBar->setValue(++value);
//    this->showMessage(msg, Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    mySleep(50);
    qDebug() << value;
    qDebug() << msg;
}


bool MySplashScreen::eventFilter(QObject *watched, QEvent *event) // 使用事件过滤器，防止进度条在加载中的时候，用户点击了图片，造成窗口被隐藏 // 可以去除体验效果
{
    if (event->type() == QEvent::Type::MouseButtonPress || event->type() == QEvent::Type::MouseButtonDblClick) {
        return true;
    }
    return false;
}


