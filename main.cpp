#include "moneyprogress.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/moneyprogress.qss");
    qss.open(QFile::ReadOnly);

    MoneyProgress w;
    w.setStyleSheet(qss.readAll());
    w.show();
    return a.exec();
}
