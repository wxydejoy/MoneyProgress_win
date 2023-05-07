#ifndef MONEYPROGRESS_H
#define MONEYPROGRESS_H

#include <QMainWindow>
#include <QTimer>
#include <message.h>
#include <QSystemTrayIcon>    //任务栏类
#include <QMenu>    //菜单类
#include <QTime>
#include <QSettings>
#include <QCoreApplication>

#include <bar.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MoneyProgress; }
QT_END_NAMESPACE

class MoneyProgress : public QMainWindow
{
    Q_OBJECT

public:
    MoneyProgress(QWidget *parent = nullptr);
    ~MoneyProgress();

private:
    Ui::MoneyProgress *ui;
    //任务栏进度条窗口
    message iconmessage = message();
    bar newbar = bar();

    //设置


    //上班时间
    QTime workUp = QTime();
    QTime workDown = QTime();
    //午休
    QTime sleepUp = QTime();
    QTime sleepDown = QTime();
    //工资
    float money=0;
    //天数
    int days=0;


    //任务栏文字位置
    QPoint barpoint;




    //窗口任务栏属性
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *quitAction;    //退出程序
    QAction *restoreWinAction;  //恢复窗口
    void createMenu();

    //拖拽
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    QPoint m_startPoint;
    QPoint m_windowPoint;

    //按键
    void keyPressEvent(QKeyEvent *event);

    //update
    void update();
    void updateM();


    //定时器
    QTimer *timer2 = new QTimer;

private slots:
    void showNormal();

    void on_Startcalculate_clicked();

    //托盘单击事件
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);

    void on_timeWorkup_userTimeChanged(const QTime &time);
    void on_timeWorkdown_userTimeChanged(const QTime &time);
    void on_timeSleepup_userTimeChanged(const QTime &time);
    void on_timeSleepdown_userTimeChanged(const QTime &time);
    void on_moneyMonth_editingFinished();
    void on_workDay_editingFinished();
    void on_barcheck_stateChanged(int arg1);
};
#endif // MONEYPROGRESS_H
