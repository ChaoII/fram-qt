#include "common/utility.h"

namespace Utility {

QImage Mat2QImage(cv::Mat &mat) {
    if (mat.type() == CV_8UC1 || mat.type() == CV_8U) {
        QImage image((const uchar *)mat.data, mat.cols, mat.rows, mat.step,
                     QImage::Format_Grayscale8);
        return image;
    } else if (mat.type() == CV_8UC3) {
        QImage image((const uchar *)mat.data, mat.cols, mat.rows, mat.step,
                     QImage::Format_RGB888);
        return image.rgbSwapped(); // r与b调换
    } else {
        std::runtime_error(
                    "un supported image type only support CV_8UC1 and CV_8UC3");
        return QImage();
    }
}

// Qt读入彩色图后一般为Format_RGB32格式（4通道），而OpenCV一般用3通道的，因此进行了转换。
cv::Mat QImage2Mat(QImage image) {
    cv::Mat mat;
    switch (image.format()) {
    case QImage::Format_RGB32: // 一般Qt读入彩色图后为此格式
        mat = cv::Mat(image.height(), image.width(), CV_8UC4,
                      (void *)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR); // 转3通道
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3,
                      (void *)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1,
                      (void *)image.constBits(), image.bytesPerLine());
        break;
    default:
        std::runtime_error("un supported image format only support Format_RGB32 "
                           "Format_RGB888 and Format_Indexed8");
    }
    return mat;
}

cv::Mat CropImage(cv::Mat &mat, std::vector<int> box) {

    return mat(cv::Rect(box[0], box[1], box[2] - box[0], box[3] - box[1]))
            .clone();
}

cv::Mat fit_screen(const cv::Mat &mat, QSize screen_size) {
    cv::Mat crop_image;
    if (mat.rows == 0) {
        return crop_image;
    }
    int s_h = screen_size.height();
    int s_w = screen_size.width();
    int i_h = mat.rows;
    int i_w = mat.cols;

    if (s_h >= s_w) {
        // h占主导
        float rate = static_cast<float>(i_h) / s_h;
        float scaled_h = i_h / rate;
        float scaled_w = i_w / rate;
        cv::Mat scaled_img;
        // if don't +1 maybe convert to dst size less than s_w or s_h. Sometimes, tt may be cause some error
        cv::resize(mat, scaled_img, cv::Size(scaled_w+1, scaled_h+1));
        int gap_w = (scaled_w - s_w) / 2;
        crop_image = scaled_img(cv::Rect(gap_w, 0, s_w, s_h)).clone();
    } else {
        float rate = static_cast<float>(i_w) / s_w;
        float scaled_h = i_h / rate;
        float scaled_w = i_w / rate;
        cv::Mat scaled_img;
        cv::resize(mat, scaled_img, cv::Size(scaled_w+1, scaled_h+1));
        int gap_h = (scaled_h - s_h) / 2;
        crop_image = scaled_img(cv::Rect(0, gap_h, s_w, s_h)).clone();
    }
    return crop_image;
}

QImage fit_screen(const QImage &img, QSize screen_size) {
    QImage crop_image;
    if (img.size().height() == 0) {
        return crop_image;
    }
    int s_h = screen_size.height();
    int s_w = screen_size.width();
    int i_h = img.height();
    int i_w = img.width();

    if (s_h >= s_w) {
        // h占主导
        float rate = static_cast<float>(i_h) / s_h;
        float scaled_h = i_h / rate;
        float scaled_w = i_w / rate;
        QImage scaled_img = img.scaled(scaled_w, scaled_h);
        int gap_w = (scaled_w - s_w) / 2;
        crop_image = scaled_img.copy(gap_w, 0, s_w, s_h);

    } else {
        float rate = static_cast<float>(i_w) / s_w;
        float scaled_h = i_h / rate;
        float scaled_w = i_w / rate;
        QImage scaled_img = img.scaled(scaled_w, scaled_h);
        int gap_h = (scaled_h - s_h) / 2;
        crop_image = scaled_img.copy(gap_h, 0, s_w, s_h);
    }
    return crop_image;
}

qint64 get_uuid() {
    uuid.init(1, 1);
    return uuid.nextid();
}

QStringList get_file_lists(const QString &dirPath) {
    QStringList file_list;
    QDir dir(dirPath);
    QFileInfoList fileInfoList =
            dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    for (auto &fileInfo : fileInfoList) {
        if (fileInfo.isDir()) {
            get_file_lists(fileInfo.absoluteFilePath());
        }
        if (fileInfo.isFile()) {
            file_list.emplace_back(fileInfo.absoluteFilePath());
        }
    }
    return file_list;
}

std::vector<float> L2Normalize(const std::vector<float> &values) {
    size_t num_val = values.size();
    if (num_val == 0) {
        return {};
    }
    std::vector<float> norm;
    float l2_sum_val = 0.f;
    for (size_t i = 0; i < num_val; ++i) {
        l2_sum_val += (values[i] * values[i]);
    }
    float l2_sum_sqrt = std::sqrt(l2_sum_val);
    norm.resize(num_val);
    for (size_t i = 0; i < num_val; ++i) {
        norm[i] = values[i] / l2_sum_sqrt;
    }
    return norm;
}

QByteArray mat2ByteArray(const cv::Mat &image) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << image.type();
    stream << image.rows;
    stream << image.cols;
    const size_t data_size = image.cols * image.rows * image.elemSize();
    QByteArray data =
            QByteArray::fromRawData((const char *)image.ptr(), data_size);
    stream << data;
    return byteArray;
}

cv::Mat byteArray2Mat(const QByteArray &byteArray) {
    QDataStream stream(byteArray);
    int matType, rows, cols;
    QByteArray data;
    stream >> matType;
    stream >> rows;
    stream >> cols;
    stream >> data;
    cv::Mat mat(rows, cols, matType, (void *)data.data());
    return mat.clone();
}

QByteArray vectorf2ByteArray(const QVector<float> &vec) {
    QByteArray array;
    // 计算vec的字节数
    int vec_byte_count = sizeof(float) * vec.size();
    // 分配内存
    array.resize(vec_byte_count);
    // 内存拷贝
    memcpy(array.data(), vec.data(), vec_byte_count);
    return array;
}

QVector<float> byteArray2Vectorf(const QByteArray &bytes) {
    QVector<float> vec;
    int vec_size = bytes.size() / sizeof(float);
    vec.resize(vec_size);
    // 拷贝的字节数
    memcpy(vec.data(), bytes.data(), bytes.size());
    return vec;
}

} // namespace Utility
