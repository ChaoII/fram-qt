#ifndef CONFIG_H
#define CONFIG_H

#include <QtWidgets>

#define INPUT_MAX_COUNT     6
#define MAX_FACE            4
#define UNIT_TIME           (1000)               // ms
#define INPUT_TIMEOUT       (10 * UNIT_TIME) // ms
#define NOTIFY_TIMEOUT      (1 * UNIT_TIME)
#define FACE_IMAGE_DIR      "faceimages/"
#define FACE_LIBS           "facelibs.dat"
#define INDEX_FILE          "index.vec"
#define VECTOR_DIMENSION    512
#define TOP_K               5
#define MAX_QUERY           6
#define REC_THRESHOLD       0.8
#define DB_FILE             "faceinfo.db"
#define FACE_INFO_TABLE     "faceinfo"
#define ATTEND_RECORD_TABLE "attend"

struct FaceInfo {
    int uid;
    QString name;
    QVector<float> feature;
    friend QDataStream &operator<<(QDataStream &output,
                                   const FaceInfo &face_info) {
        output << face_info.uid << face_info.name << face_info.feature;
        return output;
    }

    //友元函数 实现QDataStream operation>>
    friend QDataStream &operator>>(QDataStream &input, FaceInfo &face_info) {
        input >> face_info.uid >> face_info.name >> face_info.feature;
        return input;
    }
};

enum ResultType{
    Normal=0,
    Warning,
    Error
};
struct FaceResult {
    ResultType type;
    int uid;
    QString name;
    float score;
};


#endif // CONFIG_H
