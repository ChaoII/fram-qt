//
// Created by aichao on 2022/5/11.
//
#include <QMutex>
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QtWidgets>
#include "widgets/MySplashScreen.h"
#include "widgets/mywidget.h"
#include "utils/config.h"

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch (type) {
        case QtDebugMsg:
            text = QString("Debug");
            break;
        case QtWarningMsg:
            text = QString("Warning");
            break;
        case QtCriticalMsg:
            text = QString("Critical");
            break;
        case QtFatalMsg:
            text = QString("Fatal");
            break;
        case QtInfoMsg:
            text = QString("Info");
            break;
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
#if DEBUG
    QString context_info = QString("File:(%1) Line(%2)").arg(QString(context.file)).arg(context.line);
    QString message = QString("[%1] |【%2】|%3|%4").arg(current_date_time, text, context_info, msg);
#else
    QString message = QString("[%1]|【%2】| %3").arg(current_date_time, text, msg);
#endif
    QString logFile = Config::getInstance().get_log_file();
    if (logFile.isEmpty()) {
        logFile = "log.txt";
    }
    QFile file(logFile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[]) {
#ifdef __LINUX__
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
#else
    qputenv("QT_IM_MODULE", QByteArray("Qt5Input"));
#endif
    QApplication a(argc, argv);
    if (Config::getInstance().get_is_write_log()) {
        qInstallMessageHandler(outputMessage);
    }
    MySplashScreen::getInstance().show();
    MyWidget w;
    MySplashScreen::getInstance().finish(&w);
#ifdef __LINUX__
    w.setMaximumSize(1000,1000);
    w.showFullScreen();
#else
    w.show();
#endif
    return a.exec();
}
