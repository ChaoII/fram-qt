//
// Created by AC on 2023/11/14.
//

#pragma once

#include <QSplashScreen>
#include <QProgressBar>

class MySplashScreen : public QSplashScreen {
public:

    static MySplashScreen &getInstance() {
        static QPixmap pix(":img/splash.png");
        static MySplashScreen mySplashScreen(pix.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        return mySplashScreen;
    }

    void setRange(int min, int max);//设置进度条大小

    void updateProcess(const QString &msg);

private:

    explicit MySplashScreen(const QPixmap &pix);

    ~MySplashScreen() override = default;

    void mySleep(int ms);//进度条每次更新间隔

protected:
    bool eventFilter(QObject *watched, QEvent *event);


private:

    QProgressBar *progress_bar_;

    int value_ = 0;

};


