#include "registerpage.h"
#include "common/config.h"
#include "core/facehelper.h"
#include "ui_registerpage.h"

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterPage) {
    ui->setupUi(this);
    ui->lw_icon->setStyleSheet("QListWidget{background-color: transparent; }");
    ui->lb_no->setStyleSheet("QLabel{font: 30px ;color: rgb(255, 255, 255);}");
    init_lw();
}

void RegisterPage::update_frame(cv::Mat &mat) {
    m_img = mat;
    auto img = Utility::Mat2QImage(mat);
    this->ui->lb_picture->setPixmap(QPixmap::fromImage(img));
}

RegisterPage::~RegisterPage() { delete ui; }

void RegisterPage::on_pb_1_clicked() { press_num(1); }

void RegisterPage::on_pb_2_clicked() { press_num(2); }

void RegisterPage::on_pb_3_clicked() { press_num(3); }

void RegisterPage::on_pb_4_clicked() { press_num(4); }

void RegisterPage::on_pb_5_clicked() { press_num(5); }

void RegisterPage::on_pb_6_clicked() { press_num(6); }

void RegisterPage::on_pb_7_clicked() { press_num(7); }

void RegisterPage::on_pb_8_clicked() { press_num(8); }

void RegisterPage::on_pb_9_clicked() { press_num(9); }

void RegisterPage::on_pb_0_clicked() { press_num(0); }

void RegisterPage::on_pb_ensure_clicked() {

    if (this->m_str_content.count() < INPUT_MAX_COUNT) {
        qDebug() << "staff no length must be" << INPUT_MAX_COUNT;
        return;
    }
    QListWidgetItem *item = new QListWidgetItem;
    if (this->m_str_content == "请输入工号") {
        item->setText("");
    }
    item->setText(this->m_str_content);
    auto img = Utility::Mat2QImage(m_img);
    item->setIcon(QIcon(QPixmap::fromImage(img)));
    ui->lw_icon->addItem(item);
    if (ui->lw_icon->count() > MAX_FACE) {
        auto item = ui->lw_icon->takeItem(0);
        delete item;
    }

    FaceHelper::getInstance()->add_database(m_img, this->m_str_content, "");
}

void RegisterPage::on_pb_delete_clicked() {
    m_is_first_key = true;
    m_str_content.clear();
    m_str_content += QString("请输入工号");
    ui->lb_no->setText(m_str_content);
}

void RegisterPage::press_num(int num) {

    if (m_is_first_key) {
        m_is_first_key = false;
        m_str_content.clear();
        m_str_content += QString::number(num);
    } else {
        if (m_str_content.count() >= INPUT_MAX_COUNT) {
            return;
        }
        m_str_content += QString::number(num);
    }
    ui->lb_no->setText(m_str_content);
}

void RegisterPage::init_lw() {

    QSize ICON_SIZE = QSize(140, 150);
    ui->lw_icon->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lw_icon->setIconSize(ICON_SIZE);
    ui->lw_icon->setResizeMode(QListView::Adjust);
    ui->lw_icon->setViewMode(QListView::IconMode);
    ui->lw_icon->setMovement(QListView::Static);
    ui->lw_icon->setSpacing(10);
    ui->lw_icon->horizontalScrollBar()->setDisabled(true);
    connect(ui->lw_icon, &QListWidget::itemDoubleClicked,
            [=](QListWidgetItem *item) { delete item; });
}
