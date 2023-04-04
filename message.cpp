#include "message.h"
#include "ui_message.h"

message::message(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//隐藏标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明
}
void message::myhide(){
    this->hide();
}

message::~message()
{
    delete ui;
}
