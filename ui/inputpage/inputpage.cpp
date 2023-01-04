#include "inputpage.h"
#include "ui_inputpage.h"
#include "common/config.h"



InputPage::InputPage(QWidget *parent) :
    QFrame(parent),
    m_swCountDown(INPUT_TIMEOUT),
    m_pTimer(nullptr),
    m_bIsFirstKey(true),
    m_strContent("请输入密码"),
    ui(new Ui::InputPage)
{
    ui->setupUi(this);
    init();
}

InputPage::~InputPage()
{
    delete ui;
}

void InputPage::init()
{
    ui->pb_0->setIcon(QIcon(":/image/icon_num0.png"));
    ui->pb_1->setIcon(QIcon(":/image/icon_num1.png"));
    ui->pb_2->setIcon(QIcon(":/image/icon_num2.png"));
    ui->pb_3->setIcon(QIcon(":/image/icon_num3.png"));
    ui->pb_4->setIcon(QIcon(":/image/icon_num4.png"));
    ui->pb_5->setIcon(QIcon(":/image/icon_num5.png"));
    ui->pb_6->setIcon(QIcon(":/image/icon_num6.png"));
    ui->pb_7->setIcon(QIcon(":/image/icon_num7.png"));
    ui->pb_8->setIcon(QIcon(":/image/icon_num8.png"));
    ui->pb_9->setIcon(QIcon(":/image/icon_num9.png"));

    ui->lb_timeout->setStyleSheet("QLabel { font: 36px ; color: rgb(245, 245, 245); }");
    ui->lb_timeout->setHidden(true);

    ui->lb_inputcontent->setStyleSheet("QLabel { font: 36px ; color: rgb(245, 245, 245); }");
    ui->lb_inputcontent->setText(m_strContent);


    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &InputPage::updateCountDown);
    connect(this, &InputPage::reSetCountDown, this, &InputPage::onReSetCountDown);
    emit reSetCountDown(INPUT_TIMEOUT);
    ui->lb_timeout->setHidden(false);

}

void InputPage::pressNum(int num)
{
    if(m_bIsFirstKey){
        m_bIsFirstKey = false;
        m_strContent.clear();
        m_strContent += QString::number(num);
    }else{
        if(m_strContent.count() >= INPUT_MAX_COUNT){
            return ;
        }
        m_strContent += QString::number(num);
    }
    ui->lb_inputcontent->setText(m_strContent);
}

void InputPage::on_pb_1_clicked()
{
    pressNum(1);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_2_clicked()
{
    pressNum(2);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_3_clicked()
{
    pressNum(3);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_4_clicked()
{
    pressNum(4);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_5_clicked()
{
    pressNum(5);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_6_clicked()
{
    pressNum(6);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_7_clicked()
{
    pressNum(7);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_8_clicked()
{
    pressNum(8);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_9_clicked()
{
    pressNum(9);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_clear_clicked()
{
    m_bIsFirstKey = true;
    m_strContent.clear();
    m_strContent += QString("请输入密码");
    ui->lb_inputcontent->setText(m_strContent);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_0_clicked()
{
    pressNum(0);
    emit reSetCountDown(INPUT_TIMEOUT);
}

void InputPage::on_pb_ensure_clicked()
{

    //
    emit reSetCountDown(0);
}

void InputPage::on_pb_register_clicked()
{
    emit reSetCountDown(0);
}

void InputPage::updateCountDown()
{
    m_swCountDown -= UNIT_TIME;
    if(m_swCountDown <= 0){
        m_pTimer->stop();
        this->close();
        m_strContent.clear();
        m_bIsFirstKey = true;
    }
    ui->lb_timeout->setText(QString::number(m_swCountDown/UNIT_TIME));
}

void InputPage::onReSetCountDown(int swTimeOut)
{
    m_swCountDown = swTimeOut;
    m_pTimer->start(UNIT_TIME);
    ui->lb_timeout->setText(QString::number(m_swCountDown/UNIT_TIME));
}

