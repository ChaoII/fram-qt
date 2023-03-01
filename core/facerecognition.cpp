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
        FaceInfo face_info =
                FaceHelper::getInstance()->get_face_info_from_id(index);
        //        emit rec_result({ResultType::Normal, face_info.uid,
        //        face_info.name, score});
        qDebug() << "uid: " << face_info.uid << "name:" << face_info.name;
    } else {
        emit rec_result({
                            ResultType::Normal,
                        });
    }
    qDebug() << "index: " << r.I[0] << "\tscore: " << r.D[0];
}
