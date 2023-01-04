#ifndef NOTIFYPAGE_H
#define NOTIFYPAGE_H

#include <QFrame>
#include <QString>
#include <QtWidgets>

struct NotifyData_t {
    QString textColor;
    QString imagePath;
    QString tips;
    QString name;
    QString id;
};

namespace Ui {
class NotifyPage;
}

class NotifyPage : public QFrame
{
    Q_OBJECT

public:
    explicit NotifyPage(QWidget *parent = nullptr);
    ~NotifyPage();

    void setTipsBarBg(int index);
    void setTipsColor(QString strColor);
    void setTipsContent(QString strTips);
    void setUserName(QString strName);
    void setUserId(QString strId);
    void setUserHeadImg(QString strImgPath);
    void reSetTime();

signals:
    void finish_show_notify();

private slots:
    void updateCountDown();

private:
    int m_swCountDown;
    QTimer *m_pTimer;

    void paintEvent(QPaintEvent *);

    Ui::NotifyPage *ui;
    QString m_Color;
    QString m_TipsContent;
    QString m_UserName;
    QString m_UserId;
    QString m_HeadImg;
};

#endif // NOTIFYPAGE_H
