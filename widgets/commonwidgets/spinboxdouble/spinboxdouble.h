//
// Created by AC on 2023/12/6.
//


#pragma once

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SpinBox; }
QT_END_NAMESPACE

class SpinBoxDouble : public QWidget {
Q_OBJECT

public:

    explicit SpinBoxDouble(QWidget *parent = nullptr);

    ~SpinBoxDouble() override;

    void setSignalStep(double step);

    void setMinimumValue(double value);

    void setMaximumValue(double value);

    void setHintSize(int w, int h);

    void setValue(double value);

    [[nodiscard]] double value() const;


private slots:

    void on_tb_sub_clicked();

    void on_tb_add_clicked();

    void on_le_content_textChanged(const QString &text);

private:
    double min_value_ = 0.00;
    double max_value_ = 99.99;
    double step_ = 0.10;
    double value_ = 0.00;
    Ui::SpinBox *ui;
};

