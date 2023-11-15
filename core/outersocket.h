//
// Created by AC on 2023/11/6.
//
#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "core/SeetaFace.h"

class OuterSocket : public QObject {
Q_OBJECT
public:
    explicit OuterSocket(QObject *parent = nullptr);

    ~OuterSocket() override {
        if (tcp_server_) {
            tcp_server_->close();
        }
    }

signals:

    void outer_socket_close_detector_signal();

    void outer_socket_open_detector_signal();

public slots:

    void on_new_connect();

    void on_read_ready();

private:
    QTcpServer *tcp_server_;
};

