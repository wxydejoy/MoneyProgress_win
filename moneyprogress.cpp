#include "moneyprogress.h"
#include "./ui_moneyprogress.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>


MoneyProgress::MoneyProgress(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MoneyProgress)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//隐藏标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    this->setWindowIcon(QIcon(":/img/ico/48x48.ico"));

    //判断系统是否支持托盘图标显示
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qDebug("not icon");
        return;
    }
    if(!QSystemTrayIcon::supportsMessages()){
        qDebug("not message");
    }



    createMenu();

    //实例化托盘图标控件
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/ico/32x32.ico"));     //设置托盘图标显示
    trayIcon->setToolTip("钱条"); //显示提示信息
//    trayIcon->setVisible(true);
//    trayIcon->showMessage("托盘","托盘管理",QSystemTrayIcon::Information,10000);
    trayIcon->setContextMenu(trayMenu); //绑定托盘菜单
    trayIcon->show();   //在任务栏显示图标
//    trayIcon->showMessage("Test","contestt",QIcon(":/img/ico/32x32.ico"));
//    message iconmessage = message();

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));

}

void MoneyProgress::onTrayActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            //单击托盘图标
            qDebug("click");
            iconmessage.show();
            break;
        case QSystemTrayIcon::DoubleClick:
            //双击托盘图标
            //双击后显示主程序窗口
            this->show();
            break;
        default:
            break;
        }
}

void MoneyProgress::update(){
    //
    qDebug("update");

}

void MoneyProgress::createMenu()
{
    restoreWinAction = new QAction("恢复(&R)",this);
    quitAction = new QAction("退出(&Q)",this);

    //恢复窗口
    connect(restoreWinAction,SIGNAL(triggered(bool)),this,SLOT(showNormal()));

    //退出程序
    connect(quitAction,SIGNAL(triggered(bool)),qApp,SLOT(quit()));

    //添加菜单
    trayMenu = new QMenu((QWidget*)QApplication::desktop());
    trayMenu->addAction(restoreWinAction);  //将控件绑定到菜单

    //添加分隔符
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
}


void MoneyProgress::showNormal()
{
    this->show();
}


//void QWidget::changeEvent(QEvent *e)
//{
//    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
//    {
//        this->hide();
//    }
//}




//按键
void MoneyProgress::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_A:  //弹出关于界面
        qDebug("a");

        break;
    case Qt::Key_Escape:
        this->hide();
    default:
        break;
    }
}









MoneyProgress::~MoneyProgress()
{
    delete ui;
}

void MoneyProgress::mousePressEvent(QMouseEvent *event)//窗口移动事件
{
    //当鼠标左键点击时.
      if (event->button() == Qt::LeftButton)
      {
        // log
//        qDebug("mouseleft");

//          m_move = true;
          //记录鼠标的世界坐标.
          m_startPoint = event->globalPos();
          //记录窗体的世界坐标.
          m_windowPoint = this->frameGeometry().topLeft();
      }
      //esc 退出整个程序

}
void MoneyProgress::mouseMoveEvent(QMouseEvent *event)//窗口移动事件
{

    if(event->localPos().y()<80){



        if (event->buttons() & Qt::LeftButton)
          {
              //移动中的鼠标位置相对于初始位置的相对位置.
              QPoint relativePos = event->globalPos() - m_startPoint;
//              qDebug() << "re"<< relativePos << "g" << event->globalPos() << "w" << m_windowPoint << "warr" <<event->globalPos()- this->frameGeometry().topLeft() ;
              //然后移动窗体即可.
              this->move(m_windowPoint + relativePos );
          }

    }



}

void MoneyProgress::on_Startcalculate_clicked()
{
    //启动定时器
    //计时器 用来更新
    QTimer *timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&MoneyProgress::update);
    timer->start(1000); //每分钟更新一次 后面看看要不要改成可修改的
}

