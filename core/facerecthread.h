
#pragma once

#include <QThread>
#include "seetaface.h"

using Status = seeta::FaceAntiSpoofing::Status;

class FaceRecThread : public QObject {
Q_OBJECT
public:
    explicit FaceRecThread(QObject *parent = nullptr) : QObject(parent) {};

    ~FaceRecThread() override;

private:
    void sendRecords();

signals:

    void faceRecognitionSignal(FaceInfoWrap);

    void recordSignal(QVector<FaceInfoWrap>);


public slots:

    void faceRecognition(const QImage &img, const QRect &rect);

private:
    QDateTime last_record_time_ = QDateTime::currentDateTime();    // 上次打卡记录时间
    QVector<FaceInfoWrap> records_; // 打卡记录缓存

};

