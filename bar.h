#ifndef BAR_H
#define BAR_H

#include <QMainWindow>

namespace Ui {
class bar;
}

class bar : public QMainWindow
{
    Q_OBJECT

public:
    explicit bar(QWidget *parent = nullptr);
    ~bar();
    void updatetext(QString text);

private:
    Ui::bar *ui;

    //拖拽
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    QPoint m_startPoint;
    QPoint m_windowPoint;
};

#endif // BAR_H
