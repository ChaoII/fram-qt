#include "facerecthread.h"
#include "utils/struct.h"

void FaceRecThread::faceRecognition(const QImage &img, const QRect &rect) {
    if (rect.isEmpty()) {
        sendRecords();
        return;
    }
    auto img_ = utils::qImageToCvMat(img);
    auto rect_ = utils::qRectToSRect(rect);
    auto points = SeetaFace::getInstance().faceMarker(img_, rect_);
    //人脸活体检测
    auto status = Status::REAL;
    FaceInfoWrap info{};
#ifndef __linux__
    status = SeetaFace::getInstance().faceAntiSpoofing(img_, rect_, points);
#endif
    if (status == Status::SPOOF) {
        info = FaceInfoWrap{RecognitionStatus::SPOOF,
                            QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzzzz"),
                            {}};
    } else {
        //人脸识别
        auto ret = SeetaFace::getInstance().faceRecognition(img_, points);
        if (ret.second < Config::getInstance().get_recThreshold() || ret.second > 1) {
            info = FaceInfoWrap{RecognitionStatus::Unknown,
                                QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzzzz"), {}};
            qWarning() << "未知人脸: " << "score: " << qPrintable(QString::asprintf("%.2f", ret.second));
        } else {
            auto r = SeetaFace::getInstance().getFaceInfoFromIndexId(ret.first);
            info = FaceInfoWrap{RecognitionStatus::Success,
                                QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzzzz"),
                                {r.uid, r.name, r.pic_url, img, ret.second}};
            qInfo() << "打卡成功 name: " << qPrintable(r.name) << "score: "
                    << qPrintable(QString::asprintf("%.2f", ret.second));
            records_.append(info);
        }
    }
    emit faceRecognitionSignal(info);
}

void FaceRecThread::sendRecords() {
    QDateTime cur_time = QDateTime::currentDateTime();
    if (last_record_time_.secsTo(cur_time) > Config::getInstance().get_recordInterval()) {
        if (!records_.empty()) {
            emit recordSignal(records_);
            // 打卡机记录存储后清空缓存
            records_.clear();
        }
        last_record_time_ = cur_time;
    }
}

FaceRecThread::~FaceRecThread() {
    qDebug() << "+ destroy face recognition thread,thread id is:" << QThread::currentThreadId();
}

