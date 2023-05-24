#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QObject>
#include "models.h"
#include <opencv2/opencv.hpp>
#include "SeetaFace.h"

class DataBaseHelper : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseHelper(QObject *parent = nullptr);

    void add_face(cv::Mat& img, QString uid, QString name);

    void delete_face(QString uid);

signals:

};

#endif // DATABASEHELPER_H
