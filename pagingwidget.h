#ifndef PAGINGWIDGET_H
#define PAGINGWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QKeyEvent>

namespace Ui {
class PagingWidget;
}

class PagingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PagingWidget(QWidget *parent = nullptr);

    void set_params(int cur_page, int num_pre_page, int total_num);

    void init_paging();

    ~PagingWidget();

private slots:
    void on_btn_pre_clicked();

    void on_btn_next_clicked();

    void on_cbx_num_currentIndexChanged(const QString &arg1);

private:
    void del_button_clicked();

    void set_btns_dischecked();

    void set_btn_checked(int index);

    void goto_index(int index);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::PagingWidget *ui;
    int cur_page_ = 0;
    int total_num_;
    int total_page_;
    int num_pre_page_ = 10;
    int max_btn_num_ = 4;
    QVector<QToolButton*> btns_;
};

#endif // PAGINGWIDGET_H
