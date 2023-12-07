//
// Created by AC on 2023/12/6.
//


#pragma once

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SpinBox; }
QT_END_NAMESPACE

class SpinBox : public QWidget {
Q_OBJECT

public:

    explicit SpinBox(QWidget *parent = nullptr);

    ~SpinBox() override;

    void setSignalStep(int step);

    void setMinimumValue(int value);

    void setMaximumValue(int value);

    void setHintSize(int w, int h);

    void setValue(int value);

    [[nodiscard]] int value() const;


private slots:

    void on_tb_sub_clicked();

    void on_tb_add_clicked();

    void on_le_content_textChanged(const QString &text);

private:
    int min_value_ = 0;
    int max_value_ = 99;
    int step_ = 1;
    int value_ = 0;
    Ui::SpinBox *ui;
};

