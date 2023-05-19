#include "moneyprogress.h"
#include "./ui_moneyprogress.h"
#include <QFile>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QtDebug>
#include <QScreen>
#include <QRect>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QObject>

MoneyProgress::MoneyProgress(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MoneyProgress)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip); // 隐藏标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);                                                    // 背景透明

//    this->setParent(&newbar);

    this->setWindowIcon(QIcon(":/img/ico/48x48.ico"));

    QFile qss(":/moneyprogress.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());

    // 判断系统是否支持托盘图标显示
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qDebug("not icon");
        return;
    }
    if (!QSystemTrayIcon::supportsMessages())
    {
        qDebug("not message");
    }

    createMenu();

    // 实例化托盘图标控件
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/ico/32x32.ico")); // 设置托盘图标显示
    trayIcon->setToolTip("钱条");                    // 显示提示信息
    //    trayIcon->setVisible(true);
    //    trayIcon->showMessage("托盘","托盘管理",QSystemTrayIcon::Information,10000);
    trayIcon->setContextMenu(trayMenu); // 绑定托盘菜单
    trayIcon->show();                   // 在任务栏显示图标
    //    trayIcon->showMessage("Test","contestt",QIcon(":/img/ico/32x32.ico"));
    //    message iconmessage = message();

    ui->hide_label->setVisible(false);
    ui->barcolorlabel->setVisible(false);
    ui->barcolor->setVisible(false);
    ui->bartext->setVisible(false);
    ui->bartextlabel->setVisible(false);
    ui->checkBoxicon->setVisible(false);

    // 右下角弹窗
    QDesktopWidget *pDeskdop = QApplication::desktop();

    int posX = pDeskdop->width();
    int posY = pDeskdop->height();

    iconmessage.setGeometry(posX - 320, posY - 220, 300, 180);
    // 更新一次数据(默认数据)

    // 注册表的一些操作
    QSettings settings("MuYin", "MoneyProgress_win");
    //    settings = QSettings(QSettings::NativeFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    qDebug() << settings.applicationName() << settings.organizationName();
    //    settings.set
    //    QCoreApplication::setOrganizationName(QString());
    // QCoreApplication::setApplicationName(QString("MoneyProgress_win"));
    settings.sync();
    qDebug() << settings.value("set");
    qDebug() << settings.value("workDown");
    qDebug() << settings.value("barpoint");
    if (settings.value("set").toBool() == true)
    { // 如果设置了
        qDebug() << settings.value("workDown");
        workDown = settings.value("workDown").toTime();
        workUp = settings.value("workUp").toTime();
        sleepDown = settings.value("sleepDown").toTime();
        sleepUp = settings.value("sleepUp").toTime();
        money = settings.value("money").toInt();
        days = settings.value("days").toInt();

        ui->timeWorkdown->setTime(workDown);
        ui->timeWorkup->setTime(workUp);
        ui->timeSleepdown->setTime(sleepDown);
        ui->timeSleepup->setTime(sleepUp);
        ui->moneyMonth->setText(QString::number(money));
        ui->workDay->setText(QString::number(days));
        if (settings.value("barpoint").isValid())
        {
            barpoint = settings.value("barpoint").toPoint();
            ui->barcheck->setChecked(true);
            newbar.show();
            newbar.move(barpoint.x(), barpoint.y());
        }
        else
        {
            //            this->x()+this->width()*2/5,this->y()+this->height()*3/4
            barpoint.setX(QApplication::screenAt(QCursor().pos())->geometry().width() / 2 - newbar.width() / 2);
            barpoint.setY(QApplication::screenAt(QCursor().pos())->geometry().height() / 2 + newbar.height());
        }
        //判断是否设置了任务栏颜色
        if (settings.value("barcolor").isValid())
        {
            barcolor = settings.value("barcolor").toString();
            ui->barcolor->setText(barcolor);
            newbar.setlabelcolor(barcolor);
        }
        else
        {
            barcolor = "#ffd83a";
            ui->barcolor->setText(barcolor);
            newbar.setlabelcolor(barcolor);
        }
        //判断是否设置了任务栏文本
        if (settings.value("bartext").isValid())
        {
            bartext = settings.value("bartext").toString();
            ui->bartext->setText(bartext);
            updateM();
        }
        else
        {
            bartext = "您今天已经赚了x元";
            ui->bartext->setText(bartext);
            updateM();
        }
        if (settings.value("baricon").isValid())
        {
            baricon = settings.value("baricon").toBool();
            ui->checkBoxicon->setChecked(baricon);
            updateM();
        }
        else
        {
            baricon = false;
            ui->checkBoxicon->setChecked(baricon);
            updateM();
        }
        settings.sync();

    }
    else
    { // 如果没有设置
        settings.setValue("set", true);
        workDown = ui->timeWorkdown->time();
        workUp = ui->timeWorkup->time();
        sleepDown = ui->timeSleepdown->time();
        sleepUp = ui->timeSleepup->time();
        money = 300;
        days = 24;

        barpoint.setX(QApplication::screenAt(QCursor().pos())->geometry().width() / 2 - newbar.width() / 2);
        barpoint.setY(QApplication::screenAt(QCursor().pos())->geometry().height() / 2 + newbar.height());

        settings.setValue("workDown", workDown);
        settings.setValue("workUp", workUp);
        settings.setValue("sleepDown", sleepDown);
        settings.setValue("sleepUp", sleepUp);
        settings.setValue("money", money);
        settings.setValue("days", days);
        // settings.setValue("barpoint", barpoint);
        // settings.setValue("barcolor", barcolor);
        // settings.setValue("bartext", bartext);

        //判断是否勾选任务栏文字


        settings.sync();
    }

    //    QString str;
    //    str = QString("My blog: <a href=\"https://blog.csdn.net/humanking7\">https://blog.csdn.net/humanking7 </a></h2>");
    //    QLabel *lab = new QLabel(str);
    ui->labelAbout->setText("<a href=\"https://c.undf.top\">Blog </a><br/> <a href=\"https://github.com/wxydejoy/MoneyProgress_win\">Github </a>");

    ui->labelAbout->setOpenExternalLinks(true); // 如果没有这句，就只能通过linkActivated信号，连接到自定义槽函数中打开
    ui->labelAbout->hide();

    ui->moneyMonth->setAttribute(Qt::WA_InputMethodEnabled, false); // 设置账号输入框点击时无法输入中文
    ui->workDay->setAttribute(Qt::WA_InputMethodEnabled, false);    // 设置账号输入框点击时无法输入中文

    updateM();

    //    connect(timer,&QTimer::timeout,this,&MoneyProgress::update);
    //    newbar.show();
    // 启动定时器
    // 计时器 用来更新
    //    QTimer *timer = new QTimer;
    //    void (MoneyProgress:: *pup)(int) = &MoneyProgress::update;
    //    QOverload::of(&QComboBox::currentIndexChanged),[=](int index){ /* … */ })

    connect(timer, &QTimer::timeout, this, qOverload<>(&MoneyProgress::update));
    timer->start(1000); // 每分钟更新一次 后面看看要不要改成可修改的

    connect(timer2, &QTimer::timeout, &iconmessage, &message::hide);

    //    this->hide()

    //    newbar.show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));
}

MoneyProgress::~MoneyProgress()
{
    barpoint.setX(newbar.x());
    barpoint.setY(newbar.y());
    QSettings settings("MuYin", "MoneyProgress_win");
    // 保存设置
    settings.setValue("money", money);
    settings.setValue("days", days);
    settings.setValue("workUp", workUp);
    settings.setValue("workDown", workDown);
    settings.setValue("sleepUp", sleepUp);
    settings.setValue("sleepDown", sleepDown);
//    settings.setValue("barpoint", barpoint);
    // settings.setValue("geometry", this->saveGeometry());
    // settings.setValue("windowState", this->saveState());
    if (ui->barcheck->isChecked())
    {
        settings.setValue("barpoint", barpoint);
        settings.setValue("barcolor", barcolor);
        settings.setValue("bartext", bartext);
        settings.setValue("baricon", baricon);

    }
    else
    {
        settings.remove("barpoint");
        settings.remove("barcolor");
        settings.remove("bartext");
        settings.remove("baricon");
    }
    // 保存设置
    settings.sync();

    delete ui;
}

void MoneyProgress::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        // 单击托盘图标
        qDebug("click");
        qDebug() << QCursor().pos().x() << QCursor().pos().y();

        // 鼠标点物理屏幕的尺寸
        qDebug() << QApplication::screenAt(QCursor().pos())->geometry().width() << QApplication::screenAt(QCursor().pos())->geometry().height();
        // 判断鼠标位置与屏幕的关系
        iconmessage.setGeometry(((QCursor().pos().x() + 320) > QApplication::screenAt(QCursor().pos())->geometry().width() ? QApplication::screenAt(QCursor().pos())->geometry().width() - 320 : QCursor().pos().x()),
                                ((QCursor().pos().y() + 210) > QApplication::screenAt(QCursor().pos())->geometry().height() ? QApplication::screenAt(QCursor().pos())->geometry().height() - 210 : QCursor().pos().y() - 210),
                                300, 180);

        //        updateM();
        iconmessage.show();
        update();
        timer2->start(2000); //
        break;
    case QSystemTrayIcon::DoubleClick:
        // 双击托盘图标
        // 双击后显示主程序窗口
        this->show();
        break;
    default:
        update();
        break;
    }
}

void MoneyProgress::update()
{
    //
    qDebug("update");
    int second = (workUp.secsTo(workDown) - sleepUp.secsTo(sleepDown));
    float hours = second / 3600.0;
    int progress = 0;
    float moneyday = money / days;
    float moneysecond = moneyday / second;

    if (QTime::currentTime() < sleepUp) // 判断当前使是午休之前还是之后
        progress = workUp.secsTo(QTime::currentTime()) * 1000 / second;
    else
        progress = (workUp.secsTo(QTime::currentTime()) - sleepUp.secsTo(sleepDown)) * 1000 / second;

    // 判断两个界面是否可见
    if (this->isVisible())
    {
        ui->labelMoneyNow->setText("您当前已经挣了" + QString::number(moneyday * progress / 1000, 'f', 1) + "元;");
        ui->labelDay->setText("您一月工作" + QString::number(days) + "天;");
        ui->labelMoneyDay->setText("您一天能挣" + QString::number(moneyday, 'f', 1) + "元;");
        ui->labelHourDay->setText("您一天工作" + QString::number(hours, 'f', 1) + "小时;");
        ui->labelMoneySecond->setText("您一秒钟能挣" + QString::number(moneysecond, 'f', 6) + "元;");
    }
    if (iconmessage.isVisible())
    {

        iconmessage.update(progress, moneyday);
        qDebug() << progress;
    }
    if (newbar.isVisible())
    {
        QString text = ui->bartext->text();
        if (text.contains("x"))
        {
            // 修改文本 将x替换为数字 QString::number(moneyday*progress/1000,'f',5)
            text.replace("x", QString::number(moneyday * progress / 1000, 'f', 2));
            newbar.updatetext(text);
        }
        else
        {
            QMessageBox::warning(this, "警告", "文本格式不正确");
        }
         newbar.setIcon(baricon);
    }
}
void MoneyProgress::updateM()
{
    //
    qDebug("update");
    int second = (workUp.secsTo(workDown) - sleepUp.secsTo(sleepDown));
    float hours = second / 3600.0;

    float moneyday = money / days;
    float moneysecond = moneyday / second;
    int progress = 0;
    if (QTime::currentTime() < sleepUp)
        progress = workUp.secsTo(QTime::currentTime()) * 1000 / second;
    else
        progress = (workUp.secsTo(QTime::currentTime()) - sleepUp.secsTo(sleepDown)) * 1000 / second;
    // 判断两个界面是否可见
    ui->labelMoneyNow->setText("您当前已经挣了" + QString::number(moneyday * progress / 1000, 'f', 1) + "元;");
    ui->labelDay->setText("您一月工作" + QString::number(days) + "天;");
    ui->labelMoneyDay->setText("您一天能挣" + QString::number(moneyday, 'f', 1) + "元;");
    ui->labelHourDay->setText("您一天工作" + QString::number(hours, 'f', 1) + "小时;");
    ui->labelMoneySecond->setText("您一秒钟能挣" + QString::number(moneysecond, 'f', 6) + "元;");

    qDebug() << progress;
    iconmessage.update(progress, moneyday);
    QString text = ui->bartext->text();
    if (text.contains("x"))
    {
        // 修改文本 将x替换为数字 QString::number(moneyday*progress/1000,'f',5)
        text.replace("x", QString::number(moneyday * progress / 1000, 'f', 2));
        newbar.updatetext(text);
    }
    else
    {
        QMessageBox::warning(this, "警告", "文本格式不正确");
    }
    newbar.setIcon(baricon);
}

void MoneyProgress::createMenu()
{
    restoreWinAction = new QAction("恢复(&R)", this);
    quitAction = new QAction("退出(&Q)", this);

    // 恢复窗口
    connect(restoreWinAction, SIGNAL(triggered(bool)), this, SLOT(showNormal()));

    // 退出程序
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

    // 添加菜单
    trayMenu = new QMenu((QWidget *)QApplication::desktop());
    trayMenu->addAction(restoreWinAction); // 将控件绑定到菜单

    // 添加分隔符
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
}

void MoneyProgress::showNormal()
{
    this->show();
}

// void QWidget::changeEvent(QEvent *e)
//{
//     if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
//     {
//         this->hide();
//     }
// }

// 按键
void MoneyProgress::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A: // 弹出关于界面
        qDebug("a");
        if (!ui->labelAbout->isVisible())
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

void MoneyProgress::mousePressEvent(QMouseEvent *event) // 窗口移动事件
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
void MoneyProgress::mouseMoveEvent(QMouseEvent *event) // 窗口移动事件
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

void MoneyProgress::on_Startcalculate_clicked()
{
    //
}

void MoneyProgress::on_timeWorkup_userTimeChanged(const QTime &time)
{
    workUp = time;
}

void MoneyProgress::on_timeWorkdown_userTimeChanged(const QTime &time)
{
    workDown = time;
    // 有效工时计算与显示

    qDebug("工作时间 %f", (workUp.secsTo(workDown) - sleepUp.secsTo(sleepDown)) / 3600.0);
    // 保存设置

    update();
}

void MoneyProgress::on_timeSleepup_userTimeChanged(const QTime &time)
{
    sleepUp = time;
    update();
    qDebug("午休时间 %f", sleepUp.secsTo(sleepDown) / 3600.0);
}

void MoneyProgress::on_timeSleepdown_userTimeChanged(const QTime &time)
{
    sleepDown = time;
    update();
    qDebug("午休时间 %f", sleepUp.secsTo(sleepDown) / 3600.0);

    // 保存设置
}

void MoneyProgress::on_moneyMonth_editingFinished()
{
    money = ui->moneyMonth->text().toInt();
    update();
}

void MoneyProgress::on_workDay_editingFinished()
{
    days = ui->workDay->text().toFloat();
    update();
}

void MoneyProgress::on_barcheck_stateChanged(int arg1)
{
    if (arg1)
    {
        //        newbar.setGeometry(289,125,barpoint.x(),barpoint.y());
        newbar.show();
        newbar.move(barpoint.x(), barpoint.y());
        // 移动位置
        qDebug() << newbar.x() << newbar.y();
        //        newbar.move(this->x()+this->width()*2/5,this->y()+this->height()*3/4);
        ui->hide_label->setVisible(true);
        ui->barcolorlabel->setVisible(true);
        ui->barcolor->setVisible(true);
        ui->bartext->setVisible(true);
        ui->bartextlabel->setVisible(true);
        ui->checkBoxicon->setVisible(true);
    }
    else
    {
        newbar.hide();
        ui->hide_label->setVisible(false);
        ui->barcolorlabel->setVisible(false);
        ui->barcolor->setVisible(false);
        ui->bartext->setVisible(false);
        ui->bartextlabel->setVisible(false);
        ui->checkBoxicon->setVisible(false);
    }
}

void MoneyProgress::on_barcolor_editingFinished()
{
    // 判断是否合法并使用lineedit里面的字符修改颜色
    QString color = ui->barcolor->text();
    if (color[0] == '#')
    {
        if (color.length() == 7)
        {
            barcolor = color;
            // 修改颜色
            newbar.setlabelcolor(color);
        }
        else
        {
            QMessageBox::warning(this, "警告", "颜色格式不正确");
        }
    }
}
void MoneyProgress::on_bartext_editingFinished()
{
    bartext = ui->bartext->text();
    update();
}

void MoneyProgress::on_checkBoxicon_stateChanged(int arg1)
{
    newbar.setIcon(arg1);
    baricon = (bool) arg1;
}

