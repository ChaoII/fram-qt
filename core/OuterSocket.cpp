//
// Created by AC on 2023/11/6.
//

#include "outersocket.h"

MessageFlag strToMessageFlag(const QString &str) {
    if (str == "1") {
        return MessageFlag::AddFace;
    } else if (str == "2") {
        return MessageFlag::DeleteFace;
    } else if (str == "3") {
        return MessageFlag::DeleteAttend;
    } else {
        return MessageFlag::None;
    }
}

QString messageFlagToStr(const MessageFlag &flag) {
    if (flag == MessageFlag::AddFace) {
        return "1";
    } else if (flag == MessageFlag::DeleteFace) {
        return "2";
    } else if (flag == MessageFlag::DeleteAttend) {
        return "3";
    } else {
        return "-1";
    }
}

OuterSocket::OuterSocket(QObject *parent) : QObject(parent) {
    tcp_server_ = new QTcpServer(this);
    tcp_server_->listen(QHostAddress::Any, Config::getInstance().get_socketPort());
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
    MessageFlag messageFlag = strToMessageFlag(rets[0]);
    if (messageFlag == MessageFlag::AddFace) {
        execAddFace(rets, tcp_socket_ptr);
    } else if (messageFlag == MessageFlag::DeleteFace) {
        execDeleteFace(rets, tcp_socket_ptr);
    } else if (messageFlag == MessageFlag::DeleteAttend) {
        execDeleteAttend(rets, tcp_socket_ptr);
    } else {
        qDebug() << "> buffer must start with 1 or -1 and split by '@'";
        tcp_socket_ptr->write(QString("0x0201").toUtf8());
    }
}

void OuterSocket::execAddFace(const QStringList &rets, QTcpSocket *tcpSocketPtr) {
    emit outer_socket_close_detector_signal();
    QThread::msleep(200);
    qDebug() << "> adding face ....";
    if (rets.size() < 4) {
        qDebug() << "> data must contain 'flag','image_directory','uid','name'";
        tcpSocketPtr->write(QString("0x0202").toUtf8());
        return;
    }
    const QString &img_directory = rets[1];
    const QString &uid = rets[2];
    const QString &name = rets[3];
    qDebug() << "> receive the data:['file_path':" << img_directory << "'uid':" << uid << "'name':" << name << "]";
    QFileInfo file_info(img_directory);
    if (file_info.exists() && file_info.suffix() == "jpg") {
        QString file_path = file_info.absoluteFilePath();
        auto img = cv::imread(file_path.toStdString());
        bool r = SeetaFace::getInstance().addFace(img, uid, name);
        if (r) {
            tcpSocketPtr->write(QString("0x0001").toUtf8());
        } else {
            tcpSocketPtr->write(QString("0x0101").toUtf8());
        }
    } else {
        tcpSocketPtr->write(QString("0x0101").toUtf8());
    }
    QThread::msleep(200);
    emit outer_socket_open_detector_signal();
}

void OuterSocket::execDeleteFace(const QStringList &rets, QTcpSocket *tcpSocketPtr) {
    if (rets.size() < 2) {
        qDebug() << "> data must contain 'flag','index_id'";
        tcpSocketPtr->write(QString("0x0201").toUtf8());
        return;
    }
    std::vector<int64_t> index_ids;
    qint64 index_id = QString(rets[1]).toLongLong();
    index_ids.push_back(index_id);
    bool r = SeetaFace::getInstance().deleteFaceByIds(index_ids);
    if (r)
        tcpSocketPtr->write(QString("0x0002").toUtf8());
    else
        tcpSocketPtr->write(QString("0x0102").toUtf8());
}

void OuterSocket::execDeleteAttend(const QStringList &rets, QTcpSocket *tcpSocketPtr) {

    if (rets.size() < 2) {
        qDebug() << "> data must contain [MessageFlag],[EndTime]";
        tcpSocketPtr->write(QString("0x0201").toUtf8());
        return;
    }
    const QString &endTime = rets[1];
    qDebug() << "> delete attend history ....";
    bool r = Dao::deleteAttendHistoryByUpBound(endTime);
    if (r)
        tcpSocketPtr->write(QString("0x0003").toUtf8());
    else
        tcpSocketPtr->write(QString("0x0103").toUtf8());

}
