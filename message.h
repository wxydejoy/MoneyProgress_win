#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>

namespace Ui {
class message;
}

class message : public QWidget
{
    Q_OBJECT

public:
    explicit message(QWidget *parent = nullptr);
    ~message();
//    QPoint messagePoint;
public slots:
    void myhide();

private:

    Ui::message *ui;

};

#endif // MESSAGE_H
