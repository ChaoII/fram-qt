//
// Created by AC on 2023/11/14.
//

#pragma once

#include <QSplashScreen>
#include <QProgressBar>

class MySplashScreen : public QSplashScreen {
public:

    static MySplashScreen &getInstance() {
        static QPixmap pix(":img/face.png");
        static MySplashScreen mySplashScreen(pix.scaled(300, 300));
        return mySplashScreen;
    }

    void setRange(int min, int max);//设置进度条大小

    void update_process(const QString &msg);

private:

    explicit MySplashScreen(const QPixmap &pix);

    ~MySplashScreen() override = default;

    void mySleep(int ms);//进度条每次更新间隔

protected:
    bool eventFilter(QObject *watched, QEvent *event);


private:

    QProgressBar *progressBar;

    int value = 0;

};


