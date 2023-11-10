//
// Created by AC on 2023/11/6.
//

#include "outersocket.h"

OuterSocket::OuterSocket(QObject *parent) : QObject(parent) {
    tcp_server_ = new QTcpServer(this);
    tcp_server_->listen(QHostAddress::LocalHost, 9088);
    connect(tcp_server_, &QTcpServer::newConnection, this, &OuterSocket::on_new_connect);
}

void OuterSocket::on_new_connect() {
    auto tcp_socket = tcp_server_->nextPendingConnection();
    qDebug() << "> new connection ip: " << tcp_socket->peerAddress().toString() << " port: " << tcp_socket->peerPort();
    connect(tcp_socket, &QTcpSocket::readyRead, this, &OuterSocket::on_read_ready);
    connect(tcp_socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "> connect shutdown ip: " << tcp_socket->peerAddress().toString() << " port: "
                 << tcp_socket->peerPort();
    });
}

//0x0001 添加人脸成功
//0x0002 删除人脸成功
//0x0101 添加人脸失败
//0x0102 添加人脸失败
//0x0201 字符串格式不正确，必须以@作为分隔符并且flag必须为1或-1
//0x0202 字符串格式不正确，添加人脸必须包含flag,image_path,uid,name
//0x0203 字符串格式不正确，删除人脸必须包含flag,index_id

void OuterSocket::on_read_ready() {
    auto tcp_socket_ptr = dynamic_cast<QTcpSocket *> (sender());
    QString buffer;
    buffer = tcp_socket_ptr->readAll();
    qDebug() << "> buffer: " << buffer;
    auto rets = buffer.split("@");
    if (rets.empty()) {
        qDebug() << "> buffer format error, string must split by '@'";
        tcp_socket_ptr->write(QString("0x0201").toUtf8());
    }
    if (rets[0] == "1") {
        emit outer_socket_close_detector_signal();
        QThread::msleep(200);
        qDebug() << "> adding face ....";
        if (rets.size() < 4) {
            qDebug() << "> data must contain 'flag','image_directory','uid','name'";
            tcp_socket_ptr->write(QString("0x0202").toUtf8());
            return;
        }
        QString img_directory = rets[1];
        QString uid = rets[2];
        QString name = rets[3];
        qDebug() << "> receive the data:['file_path':" << img_directory << "'uid':" << uid << "'name':" << name << "]";
        QFileInfo file_info(img_directory);
        if (file_info.exists() && file_info.suffix() == "jpg") {
            QString file_path = file_info.absoluteFilePath();
            auto img = cv::imread(file_path.toStdString());
            SeetaFace::getInstance()->add_face(img, uid, name);
            tcp_socket_ptr->write(QString("0x0001").toUtf8());
        } else {
            tcp_socket_ptr->write(QString("0x0101").toUtf8());
        }
        QThread::msleep(200);
        emit outer_socket_open_detector_signal();
    } else if (rets[0] == "-1") {
        if (rets.size() < 2) {
            qDebug() << "> data must contain 'flag','index_id'";
            tcp_socket_ptr->write(QString("0x0201").toUtf8());
            return;
        }
        qDebug() << "> delete face ....";
        std::vector<int64_t> index_ids;
        qint64 index_id = QString(rets[1]).toLongLong();
        index_ids.push_back(index_id);
        bool r = SeetaFace::getInstance()->delete_face_by_ids(index_ids);
        if (r)
            tcp_socket_ptr->write(QString("0x0002").toUtf8());
        else
            tcp_socket_ptr->write(QString("0x0102").toUtf8());
    } else {
        qDebug() << "> buffer must start with 1 or -1 and split by '@'";
        tcp_socket_ptr->write(QString("0x0201").toUtf8());
    }
}
