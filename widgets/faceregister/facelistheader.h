#pragma once

#include <QWidget>

namespace Ui {
    class FaceListHeader;
}

class FaceListHeader : public QWidget {
Q_OBJECT

public:
    explicit FaceListHeader(QWidget *parent = nullptr);

    ~FaceListHeader();

private:
    Ui::FaceListHeader *ui;
};


