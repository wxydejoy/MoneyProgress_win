#include "message.h"
#include "ui_message.h"
#include <QFile>
message::message(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint |Qt::ToolTip);//隐藏标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明

    QFile qss(":/message.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    ui->progressBar->setRange(0,1000);

}

void message::update(int progress,float money){
    //更新进度条
    ui->labelMoney->setText("您当前已经挣了"+QString::number(money*progress/1000,'f',1)+"元");
    ui->progressBar->setValue(progress);

}
void message::myhide(){
    this->hide();
}

message::~message()
{
    delete ui;
}
