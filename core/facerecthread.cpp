#include "facerecthread.h"

void FaceRecThread::face_recognition(const QImage &img, const QRect &rect) {
    if (rect.isEmpty()) {
        send_records();
        return;
    }
    auto img_ = Utils::QImage2CvMat(img);
    auto rect_ = Utils::QRect2SRect(rect);
    auto points = SeetaFace::getInstance()->face_marker(img_, rect_);
    //人脸活体检测
    auto status = Status::REAL;
    FaceInfoWrap info{-1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzzz"), {}};
    status = SeetaFace::getInstance()->face_anti_spoofing(img_, rect_, points);

    if (status == Status::SPOOF) {
        qCritical() << "攻击人脸";
    } else {
        //人脸识别
        auto ret = SeetaFace::getInstance()->face_recognition(img_, points);
        if (ret.second < Config::getInstance()->getRec_threshold() || ret.second > 1) {
            info = FaceInfoWrap{0, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"), {}};
            qWarning() << "未知人脸: " << "score: " << qPrintable(QString::asprintf("%.2f", ret.second));
        } else {
            auto r = SeetaFace::getInstance()->get_faceinfo_from_index_id(ret.first);
            info = FaceInfoWrap{1, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzzzzz"),
                                {r.uid, r.name, r.pic_url, img, ret.second}};
            qInfo() << "打卡成功 name: " << qPrintable(r.name) << "score: "
                    << qPrintable(QString::asprintf("%.2f", ret.second));
            records_.append(info);
        }
    }
    emit face_rec_signal(info);
}


void FaceRecThread::send_records() {
    QDateTime cur_time = QDateTime::currentDateTime();
    if (last_record_time_.secsTo(cur_time) > Config::getInstance()->getRecord_interval()) {
        if (!records_.empty()) {
            emit record_signal(records_);
            // 打卡机记录存储后清空缓存
            records_.clear();
        }
        last_record_time_ = cur_time;
    }
}

FaceRecThread::~FaceRecThread() {
    qDebug() << "+ destroy face recognition thread,thread id is:" << QThread::currentThreadId();
}

