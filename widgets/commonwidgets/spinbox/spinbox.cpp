//
// Created by AC on 2023/12/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SpinBox.h" resolved

#include "spinbox.h"
#include "ui_spinbox.h"


SpinBox::SpinBox(QWidget *parent) :
        QWidget(parent), ui(new Ui::SpinBox) {
    ui->setupUi(this);
    resize(112, 15);
}

SpinBox::~SpinBox() {
    delete ui;
}

void SpinBox::on_tb_sub_clicked() {
    if (value_ > min_value_) {
        value_ -= step_;
        ui->le_content->setText(QString::number(value_));
    }
}

void SpinBox::on_tb_add_clicked() {
    if (value_ < max_value_) {
        value_ += step_;
        ui->le_content->setText(QString::number(value_));
    }
}

void SpinBox::setSignalStep(int step) {
    step_ = step;
}

void SpinBox::setMinimumValue(int min_value) {
    min_value_ = min_value;
}

void SpinBox::setMaximumValue(int max_value) {
    max_value_ = max_value;
}

void SpinBox::setHintSize(int w, int h) {
    ui->tb_add->resize(h, h);
    ui->tb_sub->resize(h, h);
    ui->le_content->resize(w - 2 * h, h);
}

int SpinBox::value() const {
    return value_;
}

void SpinBox::setValue(int value) {
    value_ = value;
    ui->le_content->setText(QString::number(value));
}

void SpinBox::on_le_content_textChanged(const QString &text) {
    value_ = text.toInt();
}






