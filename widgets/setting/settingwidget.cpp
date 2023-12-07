#include "settingwidget.h"
#include "ui_settingwidget.h"
#include "utils/config.h"
#include <QMessageBox>
#include <QProcess>


SettingWidget::SettingWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SettingWidget) {
    ui->setupUi(this);
    // 手动初始化子目录资源
    Q_INIT_RESOURCE(switch);
    Q_INIT_RESOURCE(spinbox);
    ui->csb_display_off_intervel->setHintSize(100, 30);
    ui->csb_display_off_intervel->setSignalStep(5);
    ui->csb_display_off_intervel->setMinimumValue(20);
    ui->csb_display_off_intervel->setMaximumValue(180);
    ui->csb_display_off_intervel->setSignalStep(5);
    ui->cb_is_wriye_log->setFixedSize(87, 30);
    ui->cb_is_wriye_log->setButtonStyle(ImageSwitch::ButtonStyle::ButtonStyle2);
    ui->cb_is_frameless->setButtonStyle(ImageSwitch::ButtonStyle::ButtonStyle2);
    ui->cb_is_frameless->setFixedSize(87, 30);
    ui->cb_vector_size->setEnabled(false);
    updateSetting();
}

SettingWidget::~SettingWidget() {
    delete ui;
}

void SettingWidget::on_tb_back_clicked() {

    auto msg_box = new QMessageBox(QMessageBox::Warning,
                                   "警告", "请确认设置信息已经保存，是否保存？",
                                   QMessageBox::Yes | QMessageBox::Cancel, this);
    msg_box->setDefaultButton(QMessageBox::Yes);
    msg_box->button(QMessageBox::Yes)->setText("是");
    msg_box->button(QMessageBox::Cancel)->setText("否");
    auto ret = msg_box->exec();
    if (QMessageBox::Yes == ret) {
        saveSettings();
    }
    emit settingBackSignal();
}

void SettingWidget::on_tb_save_clicked() {
    auto msg_box = new QMessageBox(QMessageBox::Warning,
                                   "警告", "确认保存？",
                                   QMessageBox::Yes | QMessageBox::Cancel, this);
    msg_box->setDefaultButton(QMessageBox::Yes);
    msg_box->button(QMessageBox::Yes)->setText("确认");
    msg_box->button(QMessageBox::Cancel)->setText("取消");
    auto ret = msg_box->exec();
    if (QMessageBox::Yes == ret) {
        saveSettings();
    }
}

void SettingWidget::saveSettings() {
    int top_k = ui->csb_top_k->value();
    QString index_file = ui->le_index_file->text();
    int vector_size = ui->cb_vector_size->currentText().toInt();
    int max_face_num = ui->csb_max_face_num->value();
    double rec_interval = ui->csb_rec_interval->value();
    double rec_threshold = ui->csb_rec_threshold->value();
    int record_interval = ui->csb_record_interval->value();
    QString model_dir = ui->le_model_dir->text();
    int face_recognition_thread_num = ui->csb_face_thread_num->value();
    int socket_port = ui->le_socket_port->text().toInt();
    bool is_write_log = ui->cb_is_wriye_log->isChecked();
    QString log_file = ui->le_log_file->text();
    QString gateway = ui->le_gateway->text();

    int camera_type = ui->cb_camera_type->currentIndex();
    int camera_index = ui->csb_camera_index->value();
    int frame_width = ui->le_frame_width->text().toInt();
    int frame_height = ui->le_frame_height->text().toInt();

    bool is_frameless = ui->cb_is_frameless->isChecked();
    int display_off_interval = ui->csb_display_off_intervel->value();


    Config::getInstance().updateSettings(index_file, vector_size, max_face_num, rec_interval, rec_threshold,
                                         record_interval, face_recognition_thread_num, socket_port, is_write_log,
                                         log_file, gateway, top_k, model_dir, camera_type, camera_index, frame_width,
                                         frame_height, is_frameless, display_off_interval);
    auto msg_box = new QMessageBox(QMessageBox::Warning,
                                   "警告", "重启生效，确认重启吗？",
                                   QMessageBox::Yes | QMessageBox::Cancel, this);
    msg_box->setDefaultButton(QMessageBox::Yes);
    msg_box->button(QMessageBox::Yes)->setText("确认");
    msg_box->button(QMessageBox::Cancel)->setText("取消");
    auto ret = msg_box->exec();
    if (QMessageBox::Yes == ret) {
        emit restartProgramSignal();
    }
}

void SettingWidget::updateSetting() {

    ui->csb_top_k->setValue(Config::getInstance().get_topK());
    ui->le_index_file->setText(Config::getInstance().get_indexFile());
    ui->cb_vector_size->setCurrentText(QString::number(Config::getInstance().get_vectorSize()));
    ui->csb_max_face_num->setValue(Config::getInstance().get_maxFaceNum());
    ui->csb_rec_interval->setValue(Config::getInstance().get_recInterval());
    ui->csb_rec_threshold->setValue(Config::getInstance().get_recThreshold());
    ui->csb_record_interval->setValue(Config::getInstance().get_recordInterval());
    ui->le_model_dir->setText(Config::getInstance().get_modelDir());
    ui->csb_face_thread_num->setValue(Config::getInstance().get_faceRecognitionThreadNum());
    ui->le_socket_port->setText(QString::number(Config::getInstance().get_socketPort()));
    ui->cb_is_wriye_log->setCheckState(Config::getInstance().get_isWriteLog() ? Qt::Checked : Qt::Unchecked);
    ui->le_log_file->setText(Config::getInstance().get_logFile());
    ui->le_gateway->setText(Config::getInstance().get_gateway());


    ui->cb_camera_type->setCurrentIndex(Config::getInstance().get_cameraTypeIndex());
    ui->csb_camera_index->setValue(Config::getInstance().get_cameraIndex());
    ui->le_frame_width->setText(QString::number(Config::getInstance().get_frameWidth()));
    ui->le_frame_height->setText(QString::number(Config::getInstance().get_frameHeight()));


    ui->cb_is_frameless->setCheckState(Config::getInstance().get_framelessStatus() ? Qt::Checked : Qt::Unchecked);
    ui->csb_display_off_intervel->setValue(Config::getInstance().get_displayOffInterval());
}



