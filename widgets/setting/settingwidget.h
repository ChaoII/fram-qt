#pragma once

#include <QWidget>


namespace Ui {
    class SettingWidget;
}

class SettingWidget : public QWidget {
Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);


    ~SettingWidget() override;

signals:

    void settingBackSignal();

    void restartProgramSignal();

public slots:

    void on_tb_back_clicked();

    void on_tb_save_clicked();

private:

    void saveSettings();

    void updateSetting();

private:
    bool is_save = false;

    Ui::SettingWidget *ui;

};


