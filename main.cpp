#include "moneyprogress.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MoneyProgress w;
    w.show();
    return a.exec();
}
