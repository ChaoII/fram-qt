#pragma execution_character_set("utf-8")

#include "imageswitch.h"
#include "qpainter.h"
#include "qdebug.h"

ImageSwitch::ImageSwitch(QWidget *parent) : QWidget(parent) {

    is_checked_ = false;
    button_style_ = ButtonStyle::ButtonStyle0;

    img_off_file_path_ = ":/imageswitch/btncheckon1.png";
    img_on_filepath_ = ":/imageswitch/btncheckon1.png";
    img_filepath_ = img_off_file_path_;
}

void ImageSwitch::mousePressEvent(QMouseEvent *) {
    img_filepath_ = is_checked_ ? img_off_file_path_ : img_on_filepath_;
    is_checked_ = !is_checked_;
    emit checkedChanged(is_checked_);
    this->update();
}

void ImageSwitch::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QImage img(img_filepath_);
    img = img.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //按照比例自动居中绘制
    int pixX = rect().center().x() - img.width() / 2;
    int pixY = rect().center().y() - img.height() / 2;
    QPoint point(pixX, pixY);
    painter.drawImage(point, img);
}

bool ImageSwitch::isChecked() const {
    return is_checked_;
}

ImageSwitch::ButtonStyle ImageSwitch::getButtonStyle() const {
    return this->button_style_;
}

QSize ImageSwitch::sizeHint() const {
    return {87, 28};
}

QSize ImageSwitch::minimumSizeHint() const {
    return {87, 28};
}

void ImageSwitch::setChecked(bool is_checked) {
    if (this->is_checked_ != is_checked) {
        this->is_checked_ = is_checked;
        img_filepath_ = is_checked ? img_on_filepath_ : img_off_file_path_;
        this->update();
    }
}

void ImageSwitch::setButtonStyle(const ImageSwitch::ButtonStyle &button_style) {
    if (this->button_style_ != button_style) {
        this->button_style_ = button_style;
        if (button_style_ == ButtonStyle::ButtonStyle0) {
            img_off_file_path_ = ":/imageswitch/btncheckoff1.png";
            img_on_filepath_ = ":/imageswitch/btncheckon1.png";
            this->resize(87, 28);
        } else if (button_style_ == ButtonStyle::ButtonStyle1) {
            img_off_file_path_ = ":/imageswitch/btncheckoff2.png";
            img_on_filepath_ = ":/imageswitch/btncheckon2.png";
            this->resize(87, 28);
        } else if (button_style_ == ButtonStyle::ButtonStyle2) {
            img_off_file_path_ = ":imageswitch/btncheckoff3.png";
            img_on_filepath_ = ":/imageswitch/btncheckon3.png";
            this->resize(96, 38);
        }
        img_filepath_ = is_checked_ ? img_on_filepath_ : img_off_file_path_;
        setChecked(is_checked_);
        this->update();
        updateGeometry();
    }
}

void ImageSwitch::setCheckState(Qt::CheckState check_state) {

    if (Qt::CheckState::Checked == check_state) {
        is_checked_ = true;
    } else if (Qt::CheckState::Unchecked == check_state) {
        is_checked_ = false;
    } else {
        qDebug() << "ImageSwitch only has two status: Checked or UnChecked, set to UnChecked";
        is_checked_ = false;
    }
    img_filepath_ = is_checked_ ? img_on_filepath_ : img_off_file_path_;
    this->update();
}
