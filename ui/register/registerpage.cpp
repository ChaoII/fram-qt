#include "registerpage.h"
#include "common/config.h"
#include "core/facehelper.h"
#include "ui_registerpage.h"
#include "core/staff.h"

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterPage) {
    ui->setupUi(this);
//    ui->le_staffno->setStyleSheet("QLineEdit{font: 30px ;color: rgb(255, 255, 255);}");
//    ui->le_name->setStyleSheet("QLineEdit{font: 30px ;color: rgb(255, 255, 255);}");

    this->setStyleSheet(this->styleSheet() + "#RegisterPage{background-color:rgb(0,8,22)}");
    init_lw();
}

void RegisterPage::update_frame(cv::Mat &mat) {
    m_img = mat;
    auto img = Utility::Mat2QImage(mat);
    this->ui->lb_picture->setPixmap(QPixmap::fromImage(img));
}

RegisterPage::~RegisterPage() { delete ui; }

void RegisterPage::on_pb_ensure_clicked() {

    if (ui->le_staffno->text().count() < INPUT_MAX_COUNT) {
        qDebug() << "staff no length must be" << INPUT_MAX_COUNT;
        return;
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(ui->le_staffno->text());
    auto img = Utility::Mat2QImage(m_img);
    QPainter painter(&img);
    QPixmap pixmap(":/image/icon_113x183.png");
    painter.drawPixmap(0,0,img.width(),img.height(),pixmap);
    item->setIcon(QIcon(QPixmap::fromImage(img)));
    ui->lw_icon->addItem(item);
    if (ui->lw_icon->count() > MAX_FACE) {
        auto item = ui->lw_icon->takeItem(0);
        delete item;
    }

    FaceHelper::getInstance()->add_database(m_img, ui->le_staffno->text(),ui->le_name->text());
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
