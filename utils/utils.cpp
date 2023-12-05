//
// Created by aichao on 2022/5/11.
//

#include "utils.h"


SeetaImageData utils::cvMatToSImg(cv::Mat &img) {
    SeetaImageData data{};
    data.width = img.cols;
    data.height = img.rows;
    data.channels = img.channels();
    data.data = img.data;
    return data;
}

cv::Mat utils::cropImg(const cv::Mat &img, cv::Size target_size) {

    int h = img.rows;
    int w = img.cols;
    float rate_w = w * 1.0f / target_size.width;
    float rate_h = h * 1.0f / target_size.height;
    float w_d_h = target_size.width * 1.0f / target_size.height;
    cv::Mat img_crop;
    if (rate_h < rate_w) {
        int h_t = h;
        int w_t = h_t * w_d_h;
        int gap_w = (w - w_t) / 2;
        cv::Rect rect(gap_w, 0, w_t, h_t);
        img(rect).copyTo(img_crop);
    } else {
        int w_t = w;
        int h_t = w_t / w_d_h;
        int gap_h = (h - h_t) / 2;
        cv::Rect rect(0, gap_h, w_t, h_t);
        img(rect).copyTo(img_crop);
    }
    return img_crop;
}

QImage utils::cvMat2QImage(const cv::Mat &img) {
    if (img.channels() != 3) {
        qDebug() << "unsupported cv image chich channel is:" << img.channels();
        return {};
    }
    int height = img.rows;
    int width = img.cols;
    int step = img.step;
    cv::Mat dst;
    cv::cvtColor(img, dst, cv::COLOR_BGR2RGB);
    QImage q_img = QImage(dst.data, width, height, step, QImage::Format_RGB888).copy();
    return q_img;
}

cv::Mat utils::qImageToCvMat(const QImage &img) {
    if (img.format() != QImage::Format_RGB888) {
        qDebug() << "QImage only support Format_RGB888,but the QImage format is:" << img.format();
        return {};
    }
    auto swap = img.rgbSwapped();
    return cv::Mat(swap.height(), swap.width(), CV_8UC3, (unsigned char *) swap.bits(),
                   (size_t) swap.bytesPerLine()).clone();
}

SeetaRect utils::qRectToSRect(const QRect &rect) {
    SeetaRect s_rect{};
    s_rect.x = rect.x();
    s_rect.y = rect.y();
    s_rect.width = rect.width();
    s_rect.height = rect.height();
    return s_rect;
}

QRect utils::sRectToQRect(const SeetaRect &s_rect) {
    QRect q_rect;
    q_rect.setX(s_rect.x);
    q_rect.setY(s_rect.y);
    q_rect.setWidth(s_rect.width);
    q_rect.setHeight(s_rect.height);
    return q_rect;
}

qint64 utils::getUuid() {
    uuid_.init(1, 1);
    return uuid_.nextid();
}

QByteArray utils::floatArray2QByteArray(float *buffer, int size) {
    QByteArray array;
    size_t byte_num = sizeof(float) * size;
    array.resize(byte_num);
    memcpy(array.data(), buffer, byte_num);
    return array;
}

void utils::setBackgroundColor(QWidget *w, QColor color) {
    w->setAutoFillBackground(true);
    QPalette palette = w->palette();
    palette.setColor(QPalette::Window, color);
    w->setAutoFillBackground(true);
    w->setPalette(palette);
}



