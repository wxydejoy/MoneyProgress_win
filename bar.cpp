#include "bar.h"
#include "ui_bar.h"
#include <QMouseEvent>
bar::bar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bar)
{
    ui->setupUi(this);
    this->setWindowFlags( Qt::Dialog|Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |Qt::ToolTip); // 隐藏标题栏
//    this->setWindowFlags(Qt::Tool);		//当前窗口的构造函数下调用
//    this->setWindowFlags(); 	//同时隐藏任务栏图标和标题栏图标
    this->setAttribute(Qt::WA_TranslucentBackground);
        // 背景透明

}

bar::~bar()
{
    delete ui;
}


void bar::updatetext(QString text){
    ui->label_text->setText(text);
}




//qss修改label颜色
void bar::setlabelcolor(QString color){
    ui->label_text->setStyleSheet("color:"+color);
}


void bar::setIcon(bool state){
    ui->label_icon->setVisible(state);
}








void bar::mousePressEvent(QMouseEvent *event) // 窗口移动事件
{
    // 当鼠标左键点击时.
    if (event->button() == Qt::LeftButton)
    {
        // log
        //        qDebug("mouseleft");

        //          m_move = true;
        // 记录鼠标的世界坐标.
        m_startPoint = event->globalPos();
        // 记录窗体的世界坐标.
        m_windowPoint = this->frameGeometry().topLeft();
    }
    // esc 退出整个程序
}
void bar::mouseMoveEvent(QMouseEvent *event) // 窗口移动事件
{

    if (event->localPos().y() < 80)
    {

        if (event->buttons() & Qt::LeftButton)
        {
            // 移动中的鼠标位置相对于初始位置的相对位置.
            QPoint relativePos = event->globalPos() - m_startPoint;
            //              qDebug() << "re"<< relativePos << "g" << event->globalPos() << "w" << m_windowPoint << "warr" <<event->globalPos()- this->frameGeometry().topLeft() ;
            // 然后移动窗体即可.
            this->move(m_windowPoint + relativePos);
        }
    }
}
