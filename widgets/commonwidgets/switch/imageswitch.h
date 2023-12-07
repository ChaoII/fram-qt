
#pragma once

#include <QWidget>


class ImageSwitch : public QWidget {
Q_OBJECT

public:
    enum class ButtonStyle {
        ButtonStyle0 = 0,  //开关样式1
        ButtonStyle1 = 1,  //开关样式2
        ButtonStyle2 = 2   //开关样式3
    };

    explicit ImageSwitch(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *) override;

    void paintEvent(QPaintEvent *event) override;

private:
    bool is_checked_;
    ButtonStyle button_style_;
    QString img_off_file_path_;
    QString img_on_filepath_;
    QString img_filepath_;

public:
    void setCheckState(Qt::CheckState check_state);

    [[nodiscard]] bool isChecked() const;

    [[nodiscard]] ButtonStyle getButtonStyle() const;

    [[nodiscard]] QSize sizeHint() const override;

    [[nodiscard]] QSize minimumSizeHint() const override;

public slots:

    //设置是否选中
    void setChecked(bool is_checked);

    //设置按钮样式
    void setButtonStyle(const ImageSwitch::ButtonStyle &button_style);

signals:

    void checkedChanged(bool checked);
};


