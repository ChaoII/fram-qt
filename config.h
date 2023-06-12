#ifndef CONFIG_H
#define CONFIG_H

// 向量索引topk
#define TOP_K 5
//  faiss 向量索引文件名
#define INDEX_FILE "faceindex.vec"
// 人脸特征向量长度
#define VECTOR_SIZE 1024
// 單次注册人脸时允许录入 的最大人脸数
#define MAX_FACE 5
// 人脸识别算法间隔(ms)
#define REC_INTERVAL 500
// 人脸识别阈值
#define REC_THRESHOLD 0.6
// 打卡缓存时间(s)
#define RECORD_INTERVAL 5


#endif // CONFIG_H
