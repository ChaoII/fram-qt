#pragma once

#include <QWidget>

namespace Ui {
    class FaceListItem;
}

class FaceListItem : public QWidget {
Q_OBJECT

public:
    explicit FaceListItem(QWidget *parent = nullptr);

    void setItem(const QString &number,
                 const QString &indexId,
                 const QString &name,
                 const QString &staffId,
                 const QString &registerTime,
                 const QString &pictureUrl);

    bool getCheckedStatus();

    void setCheckStatus(bool isCheckedStatus);


    ~FaceListItem();

signals:

    void checkBoxStatusChanged_signal(bool, QString);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void changeCheckBoxStatus(bool isCheckStatus);

private:
    int hintSize;
    bool isChecked = false;
    Ui::FaceListItem *ui;
};

