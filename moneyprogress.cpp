#include "moneyprogress.h"
#include "./ui_moneyprogress.h"
#include <QFile>
#include <QMouseEvent>
#include <QKeyEvent>


#include <QDesktopWidget>


MoneyProgress::MoneyProgress(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MoneyProgress)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//隐藏标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    this->setWindowIcon(QIcon(":/img/ico/48x48.ico"));

    QFile qss(":/moneyprogress.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());

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

    //右下角弹窗
    QDesktopWidget * pDeskdop = QApplication::desktop();

    int posX = pDeskdop->width() ;
    int posY = pDeskdop->height() ;



    // 更新一次数据(默认数据)
    workDown = ui->timeWorkdown->time();
    workUp = ui->timeWorkup->time();
    sleepDown = ui->timeSleepdown->time();
    sleepUp = ui->timeSleepup->time();
    money = 300;
    days = 24;

//    QString str;
//    str = QString("My blog: <a href=\"https://blog.csdn.net/humanking7\">https://blog.csdn.net/humanking7 </a></h2>");
//    QLabel *lab = new QLabel(str);
    ui->labelAbout->setText("<a href=\"https://c.undf.top\">Blog </a><br/> <a href=\"https://github.com/wxydejoy/MoneyProgress_win\">Github </a>");

    ui->labelAbout->setOpenExternalLinks(true);//如果没有这句，就只能通过linkActivated信号，连接到自定义槽函数中打开
    ui->labelAbout->hide();
//    lab->setWindowTitle("Test Html support");




    iconmessage.setGeometry(posX-320, posY-220, 300, 180);
    updateM();


//    connect(timer,&QTimer::timeout,this,&MoneyProgress::update);

    connect(timer2,&QTimer::timeout,&iconmessage,&message::hide);

//    this->hide()



    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));

}

void MoneyProgress::onTrayActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            //单击托盘图标
            qDebug("click");
            updateM();
            iconmessage.show();
            timer2->start(2000); //每分钟更新一次 后面看看要不要改成可修改的
            break;
        case QSystemTrayIcon::DoubleClick:
            //双击托盘图标
            //双击后显示主程序窗口
            this->show();
            break;
        default:
            update();
            break;
        }
}

void MoneyProgress::update(){
    //
    qDebug("update");
    int second = (workUp.secsTo(workDown)-sleepUp.secsTo(sleepDown));
    float hours = second/3600.0;

    float moneyday = money/days;
    float moneysecond = moneyday/second;
    // 判断两个界面是否可见
    if (this->isVisible() ){
        ui->labelDay->setText("您一月工作"+QString::number(days)+"天;");
        ui->labelMoneyDay->setText("您一天能挣"+QString::number(moneyday,'f',1)+"元;");
        ui->labelHourDay->setText("您一天工作"+QString::number(hours,'f',1)+"小时;");
        ui->labelMoneySecond->setText("您一秒钟能挣"+QString::number(moneysecond,'f',6)+"元;");

    }
    if(iconmessage.isVisible()){
        int progress = 0;
        progress = workUp.secsTo(QTime::currentTime())*100/second;
        iconmessage.update(progress,moneyday);

    }

}
void MoneyProgress::updateM(){
    //
    qDebug("update");
    int second = (workUp.secsTo(workDown)-sleepUp.secsTo(sleepDown));
    float hours = second/3600.0;

    float moneyday = money/days;
    float moneysecond = moneyday/second;
    // 判断两个界面是否可见

    ui->labelDay->setText("您一月工作"+QString::number(days)+"天;");
    ui->labelMoneyDay->setText("您一天能挣"+QString::number(moneyday,'f',1)+"元;");
    ui->labelHourDay->setText("您一天工作"+QString::number(hours,'f',1)+"小时;");
    ui->labelMoneySecond->setText("您一秒钟能挣"+QString::number(moneysecond,'f',6)+"元;");
    int progress = 0;
    progress = workUp.secsTo(QTime::currentTime())*100/second;
    iconmessage.update(progress,moneyday);
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
        if(!ui->labelAbout->isVisible())
            ui->labelAbout->show();
        else
            ui->labelAbout->hide();


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
//    void (MoneyProgress:: *pup)(int) = &MoneyProgress::update;
//    QOverload::of(&QComboBox::currentIndexChanged),[=](int index){ /* … */ })


    connect(timer,&QTimer::timeout,this,qOverload<>(&MoneyProgress::update));
    timer->start(1000); //每分钟更新一次 后面看看要不要改成可修改的
}


void MoneyProgress::on_timeWorkup_userTimeChanged(const QTime &time)
{
    workUp = time;
}


void MoneyProgress::on_timeWorkdown_userTimeChanged(const QTime &time)
{
    workDown = time;
    //有效工时计算与显示
    qDebug("工作时间 %f",(workUp.secsTo(workDown)-sleepUp.secsTo(sleepDown))/3600.0);
    update();
}


void MoneyProgress::on_timeSleepup_userTimeChanged(const QTime &time)
{
    sleepUp = time;
}


void MoneyProgress::on_timeSleepdown_userTimeChanged(const QTime &time)
{
    sleepDown = time;
    update();
    qDebug("午休时间 %f",sleepUp.secsTo(sleepDown)/3600.0);
}


void MoneyProgress::on_moneyMonth_editingFinished()
{
       money = ui->moneyMonth->text().toInt();
       update();
}


void MoneyProgress::on_workDay_editingFinished()
{
    days = ui->workDay->text().toInt();
    update();

}

