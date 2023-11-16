#include "facelistheader.h"
#include "ui_facelistheader.h"

FaceListHeader::FaceListHeader(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceListHeader) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

FaceListHeader::~FaceListHeader() {
    delete ui;
}
