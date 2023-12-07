//
// Created by AC on 2023/12/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SpinBox.h" resolved

#include "spinboxdouble.h"
#include "ui_spinboxdouble.h"


SpinBoxDouble::SpinBoxDouble(QWidget *parent) :
        QWidget(parent), ui(new Ui::SpinBox) {
    ui->setupUi(this);
    resize(112, 15);
}

SpinBoxDouble::~SpinBoxDouble() {
    delete ui;
}

void SpinBoxDouble::on_tb_sub_clicked() {
    if (value_ > min_value_) {
        value_ -= step_;
        ui->le_content->setText(QString::number(value_));
    }
}

void SpinBoxDouble::on_tb_add_clicked() {
    if (value_ < max_value_) {
        value_ += step_;
        ui->le_content->setText(QString::number(value_));
    }
}

void SpinBoxDouble::setSignalStep(double step) {
    step_ = step;
}

void SpinBoxDouble::setMinimumValue(double min_value) {
    min_value_ = min_value;
}

void SpinBoxDouble::setMaximumValue(double max_value) {
    max_value_ = max_value;
}

void SpinBoxDouble::setHintSize(int w, int h) {
    ui->tb_add->resize(h, h);
    ui->tb_sub->resize(h, h);
    ui->le_content->resize(w - 2 * h, h);
}

double SpinBoxDouble::value() const {
    return value_;
}

void SpinBoxDouble::setValue(double value) {
    value_ = value;
    ui->le_content->setText(QString::number(value, 'f', 2));
}

void SpinBoxDouble::on_le_content_textChanged(const QString &text) {
    value_ = text.toDouble();
}






