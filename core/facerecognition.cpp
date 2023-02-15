#include "core/facerecognition.h"
#include "core/facehelper.h"
#include "core/vectorsearch.h"

void FaceRecognitinThread::set_frame(cv::Mat mat) { cur_frame_ = mat; }

void FaceRecognitinThread::run() {
    if (cur_frame_.empty())
        return;
    cv::Mat roi = FaceHelper::getInstance()->get_face_roi(cur_frame_);
    if (roi.empty()) {
        return;
    }
    std::vector<float> feature = FaceHelper::getInstance()->get_face_feature(roi);
    SearchResult r = VectorSearch::getInstance()->search(feature.data(), 3);
    qint64 index = r.I[0];
    float score = r.D[0];
    if (score >= REC_THRESHOLD) {
        QString uid = FaceHelper::getInstance()->get_facelibs()[index].uid;
        QString name = FaceHelper::getInstance()->get_facelibs()[index].name;
        emit rec_result({ResultType::Normal, uid, name, score});
        } else {
        emit rec_result({
                         ResultType::Normal,
                         });
    }
    qDebug() << "index: " << r.I[0] << "\tscore: " << r.D[0];
}
