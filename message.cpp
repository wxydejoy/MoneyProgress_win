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

void message::update(int progress,float money){
    //更新进度条
    ui->labelMoney->setText("预计今日一共挣"+QString::number(money,'f',1)+"元");
    ui->progressBar->setValue(progress);

}
void message::myhide(){
    this->hide();
}

message::~message()
{
    delete ui;
}
