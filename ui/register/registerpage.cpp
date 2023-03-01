#include "registerpage.h"
#include "common/config.h"
#include "core/facehelper.h"
#include "ui_registerpage.h"
#include <QtConcurrent/QtConcurrent>

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterPage) {
    ui->setupUi(this);
    //    ui->le_staffno->setStyleSheet("QLineEdit{font: 30px ;color: rgb(255,
    //    255, 255);}"); ui->le_name->setStyleSheet("QLineEdit{font: 30px ;color:
    //    rgb(255, 255, 255);}");

    this->setStyleSheet(this->styleSheet() +
                        "#RegisterPage{background-color:rgb(0,8,22)}");
    connect(this, &RegisterPage::add_db_finished_signal, this, [&](int index) {
        qDebug() << index;
        if (index == m_register_face.size() - 1) {
            qDebug() << "finished";
            emit send_closed_signal();
            this->close();
        }
    });
    init_lw();
}

void RegisterPage::update_frame(cv::Mat &mat) {
    m_img = mat;
    auto img = Utility::Mat2QImage(mat);
    this->ui->lb_picture->setPixmap(QPixmap::fromImage(img));
}

void RegisterPage::clear_info() {
    ui->lw_icon->clear();
    ui->lb_picture->clear();
    ui->le_staffno->clear();
    ui->le_name->clear();
    m_register_face.clear();
    m_register_face.shrink_to_fit();
}

RegisterPage::~RegisterPage() { delete ui; }

void RegisterPage::on_pb_add_clicked() {
    if (ui->le_staffno->text().size() < INPUT_MAX_COUNT) {
        qDebug() << "staff no length must be" << INPUT_MAX_COUNT;
        return;
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(ui->le_staffno->text());
    QImage img = Utility::Mat2QImage(m_img);
    QPainter painter(&img);
    QPixmap pixmap(":/image/icon.png");
    painter.drawPixmap(0, 0, img.width(), img.height(), pixmap);
    item->setIcon(QIcon(QPixmap::fromImage(img)));
    ui->lw_icon->addItem(item);
    RegisterFaceInfo info{m_img, ui->le_staffno->text(), ui->le_name->text()};
    m_register_face.push_back(info);
    if (ui->lw_icon->count() > MAX_FACE) {
        auto item = ui->lw_icon->takeItem(0);
        delete item;
        m_register_face.pop_front();
    }
}

void RegisterPage::on_pb_ensure_clicked() {

    if (m_register_face.empty()) {
        return;
    }

    QtConcurrent::run([=]() {
        for (size_t i = 0; i < m_register_face.size(); i++) {
            bool ret = FaceHelper::getInstance()->add_database(
                        m_register_face[i].img, m_register_face[i].staff_no,
                        m_register_face[i].name);
            emit add_db_finished_signal(i);
            if (!ret) {
                continue;
            }
        }
    });
}

void RegisterPage::on_pb_cancel_clicked() {
    clear_info();
    emit send_closed_signal();
    this->close();
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
    ui->lw_icon->verticalScrollBar()->setDisabled(true);
    connect(ui->lw_icon, &QListWidget::itemDoubleClicked,
            [=](QListWidgetItem *item) {
        int index = item->listWidget()->currentRow();
        m_register_face.removeAt(index);
        delete item;
    });
}
