#include "inputpage.h"
#include "ui_inputpage.h"

#define INPUT_MAX_COUNT     6
#define MAX_FACE            4
#define UNIT_TIME           (1000)               // ms
#define INPUT_TIMEOUT       (10 * UNIT_TIME) // ms
#define NOTIFY_TIMEOUT      (1 * UNIT_TIME)
#define PASSWORD            "123456"

InputPage::InputPage(QWidget *parent) :
        QFrame(parent),
        currentTimeout(INPUT_TIMEOUT),
        counterTimer(nullptr),
        isFirstKey(true),
        ui(new Ui::InputPage) {
    ui->setupUi(this);
    this->setStyleSheet(
            "QPushButton{color: rgb(255, 255, 255);border-image:url(:/img/icon_num_btn.png);} "
            "QPushButton:hover, QPushButton:pressed{color: rgb(255, 255, 255);border-image: url(:/img/icon_num_btn_hover.png);}");

    ui->le_inputContent->setPlaceholderText("请输入密码");
    ui->message->setStyleSheet("QLabel{color:rgb(255,0,0)}");
    init();
}

InputPage::~InputPage() {
    delete ui;
}

void InputPage::init() {
    ui->pb_0->setIcon(QIcon(":/img/icon_num0.png"));
    ui->pb_1->setIcon(QIcon(":/img/icon_num1.png"));
    ui->pb_2->setIcon(QIcon(":/img/icon_num2.png"));
    ui->pb_3->setIcon(QIcon(":/img/icon_num3.png"));
    ui->pb_4->setIcon(QIcon(":/img/icon_num4.png"));
    ui->pb_5->setIcon(QIcon(":/img/icon_num5.png"));
    ui->pb_6->setIcon(QIcon(":/img/icon_num6.png"));
    ui->pb_7->setIcon(QIcon(":/img/icon_num7.png"));
    ui->pb_8->setIcon(QIcon(":/img/icon_num8.png"));
    ui->pb_9->setIcon(QIcon(":/img/icon_num9.png"));
    ui->lb_timeout->setStyleSheet("QLabel { font: 36px ; color: rgb(245, 245, 245); }");
    ui->lb_timeout->setHidden(true);
    ui->le_inputContent->setStyleSheet(
            "QLineEdit { background-color:transparent; font: 36px ; color: rgb(245, 245, 245);border:0px; }");
    ui->le_inputContent->setText(passwordContent);
    counterTimer = new QTimer(this);
    connect(counterTimer, &QTimer::timeout, this, &InputPage::updateCountDown);
    connect(this, &InputPage::reSetCountDown, this, &InputPage::onReSetCountDown);
    emit reSetCountDown(INPUT_TIMEOUT);
    this->counterTimer->stop();
    ui->lb_timeout->setHidden(false);
}

void InputPage::pressNum(int num) {
    if (isFirstKey) {
        isFirstKey = false;
        passwordContent.clear();
        passwordContent += QString::number(num);
    } else {
        if (passwordContent.size() >= INPUT_MAX_COUNT) {
            ui->message->setText(QString("管理员密码最大长度为") + QString::number(INPUT_MAX_COUNT));
            return;
        }
        passwordContent += QString::number(num);
    }
    ui->message->setText("");
    ui->le_inputContent->setText(passwordContent);
}

void InputPage::on_pb_1_clicked() {
    pressNum(1);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_2_clicked() {
    pressNum(2);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_3_clicked() {
    pressNum(3);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_4_clicked() {
    pressNum(4);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_5_clicked() {
    pressNum(5);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_6_clicked() {
    pressNum(6);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_7_clicked() {
    pressNum(7);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_8_clicked() {
    pressNum(8);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_9_clicked() {
    pressNum(9);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_clear_clicked() {
    isFirstKey = true;
    passwordContent.clear();
    ui->message->clear();
    ui->le_inputContent->setText(passwordContent);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_0_clicked() {
    pressNum(0);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_ensure_clicked() {
    if (passwordContent.trimmed() == PASSWORD) {
        emit passwordAuthorized();
        emit reSetCountDown(0);
    } else {
        ui->message->setText("密码错误！");
    }
    emit reSetCountDown(INPUT_TIMEOUT);
}


void InputPage::updateCountDown() {
    currentTimeout -= UNIT_TIME;
    if (currentTimeout <= 0) {
        counterTimer->stop();
        this->close();
        passwordContent.clear();
        isFirstKey = true;
    }
    ui->lb_timeout->setText(QString::number(currentTimeout / UNIT_TIME));
}

void InputPage::onReSetCountDown(int swTimeOut) {
    currentTimeout = swTimeOut;
    counterTimer->start(UNIT_TIME);
    ui->lb_timeout->setText(QString::number(currentTimeout / UNIT_TIME));
}

void InputPage::showInputWidget() {
    this->setVisible(true);
    onReSetCountDown(INPUT_TIMEOUT);
}

void InputPage::hideInputWidget() {
    counterTimer->stop();
    this->setVisible(false);
}


