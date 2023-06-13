#ifndef FACERECOGNITIONTHREAD_H
#define FACERECOGNITIONTHREAD_H

#include <QThread>
#include "SeetaFace.h"

using Status = seeta::FaceAntiSpoofing::Status;

class FaceRecThread : public QObject
{
    Q_OBJECT
public:
    explicit FaceRecThread(QObject *parent = nullptr):QObject(parent){};


private:
    void send_records();

signals:
    void face_rec_signal(FaceInfoWrap);

    void record_signal(QVector<FaceInfoWrap>);


public slots:
    void face_recognition(const QImage &img, const QRect &rect);

private:
    QDateTime last_record_time_ = QDateTime::currentDateTime();    // 上次打卡记录时间
    QVector<FaceInfoWrap> records_; // 打卡记录缓存

};

#endif // FACERECOGNITIONTHREAD_H
