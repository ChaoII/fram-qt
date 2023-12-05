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
                 const QString &index_id,
                 const QString &name,
                 const QString &staff_id,
                 const QString &register_time,
                 const QString &picture_url);

    [[nodiscard]] bool getCheckedStatus() const;

    void setCheckStatus(bool isCheckedStatus);

    ~FaceListItem() override;

signals:

    void checkBoxStatusChanged_signal(bool, QString);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void changeCheckBoxStatus(bool isCheckStatus);

private:
    int hint_size_;
    bool is_checked_ = false;
    Ui::FaceListItem *ui;
};

