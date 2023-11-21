//
// Created by AC on 2023/11/6.
//
#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "core/SeetaFace.h"
#include "core/Dao.h"

enum class MessageFlag {
    AddFace,
    DeleteFace,
    DeleteAttend,
    None,
};


//0x0001 添加人脸成功
//0x0002 删除人脸成功
//0x0003 删除打卡历史成功
//0x0101 添加人脸失败
//0x0102 添加人脸失败
//0x0103 删除打卡历史失败
//0x0201 字符串格式不正确，必须以@作为分隔符并且flag必须为1或-1
//0x0202 字符串格式不正确，添加人脸必须包含flag,image_path,uid,name
//0x0203 字符串格式不正确，删除人脸必须包含flag,index_id

enum class ResultFlag {
    AddFaceSuccess = 0x0001,
    DeleteFaceSuccess = 0x0002,
    DeleteAttendSuccess = 0x0003,
    AddFaceFail = 0x0101,
    DeleteFaceFail = 0x0102,
    DeleteAttendFail = 0x0103,
    FormatError = 0x0201,
};


class OuterSocket : public QObject {


Q_OBJECT
public:
    explicit OuterSocket(QObject *parent = nullptr);

    ~OuterSocket() override {
        if (tcp_server_) {
            tcp_server_->close();
        }
    }


private:
    void execAddFace(const QStringList &rets, QTcpSocket *tcpSocketPtr);

    void execDeleteFace(const QStringList &rets, QTcpSocket *tcpSocketPtr);

    void execDeleteAttend(const QStringList &rets, QTcpSocket *tcpSocketPtr);

signals:

    void outer_socket_close_detector_signal();

    void outer_socket_open_detector_signal();

public slots:

    void on_new_connect();

    void on_read_ready();

private:
    QTcpServer *tcp_server_;
};


