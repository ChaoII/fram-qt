#include "pagingwidget.h"
#include "ui_pagingwidget.h"
#include <QDebug>

PagingWidget::PagingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PagingWidget)
{
    ui->setupUi(this);

}

void PagingWidget::set_params(int cur_page, int num_pre_page, int total_num)
{
    cur_page_ = cur_page;
    num_pre_page_ = num_pre_page;
    total_num_ = total_num;
    total_page_ = total_num/num_pre_page+1;
    init_paging();
}

void PagingWidget::init_paging()
{
    ui->label->setText("共"+QString::number(total_num_)+"条");
    ui->lbl_total_page->setText(QString::number(total_page_));
    for(int i=0; i< max_btn_num_; i++){
        QToolButton *btn = new QToolButton();
        btn->setCheckable(true);
        QString text = QString::number(i+1);
        btn->setText(text);
        btns_.append(btn);
        connect(btn,&QToolButton::clicked,this,&PagingWidget::del_button_clicked);
        ui->page_horize_layout->addWidget(btn);
    }
    btns_[0]->setChecked(true);
}




PagingWidget::~PagingWidget()
{
    delete ui;
}

void PagingWidget::del_button_clicked()
{
    QToolButton* btn = static_cast<QToolButton*>(sender());
    int cur_page_index = btn->text().toInt();
    set_btns_dischecked();
    set_btn_checked(cur_page_-1);

}

void PagingWidget::set_btns_dischecked()
{
    for(auto btn:btns_){
        btn->setChecked(false);
    }
}

void PagingWidget::set_btn_checked(int index)
{
    for(auto &btn: btns_){
        if(btn->text() == QString::number(index+1)){
            btn->setChecked(true);
        }
    }
}

void PagingWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Enter){
        cur_page_ = ui->lineEdit->text().toInt()-1;
        if(ui->lineEdit->hasFocus() and cur_page_>=0 && cur_page_ <= total_page_){
            set_btns_dischecked();
            set_btn_checked(cur_page_);
        }
    }
}

void PagingWidget::on_btn_pre_clicked()
{
    if(btns_[0]->text().toInt()!=1){
        for(int i=0; i < btns_.size(); i++){
            btns_[i]->setText(QString::number(btns_[i]->text().toInt()-1));
        }
    }

    if(cur_page_<=0){
       return;
    }
    cur_page_--;
    set_btns_dischecked();
    set_btn_checked(cur_page_);
}


void PagingWidget::on_btn_next_clicked()
{


    if(btns_[max_btn_num_-1]->text().toInt()!=total_page_){
        for(int i=0; i < btns_.size(); i++){
            btns_[i]->setText(QString::number(btns_[i]->text().toInt()+1));
        }
    }

    if(cur_page_+1>=total_page_){
       return;
    }
     cur_page_++;
    qDebug()<<cur_page_;
    set_btns_dischecked();
    set_btn_checked(cur_page_);
}

void PagingWidget::on_cbx_num_currentIndexChanged(const QString &arg1)
{
    num_pre_page_ = arg1.toInt();
    total_page_ = total_num_/num_pre_page_+1;
}

