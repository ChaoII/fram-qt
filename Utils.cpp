//
// Created by aichao on 2022/5/11.
//

#include "Utils.h"


SeetaImageData Utils::cvMat_2_img(cv::Mat &img) {
    SeetaImageData data;
    data.width = img.cols;
    data.height = img.rows;
    data.channels = img.channels();
    data.data = img.data;
    return data;
}

cv::Mat Utils::crop_img(const cv::Mat &img, cv::Size target_size) {

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

QImage Utils::cvMat_2_qimg(const cv::Mat &img) {

    int height = img.rows;
    int width = img.cols;
    int step = img.step;
    cv::Mat dst;
    cv::cvtColor(img, dst, cv::COLOR_BGR2RGB);
    QImage q_img = QImage(dst.data, width, height, step, QImage::Format_RGB888).copy();
    return q_img;
}



