#ifndef MONEYPROGRESS_H
#define MONEYPROGRESS_H

#include <QMainWindow>


#include <QSystemTrayIcon>    //任务栏类
#include <QMenu>    //菜单类


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

private slots:
    void showNormal();

    void on_Startcalculate_clicked();
};
#endif // MONEYPROGRESS_H
