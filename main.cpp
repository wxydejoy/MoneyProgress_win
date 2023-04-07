#include "moneyprogress.h"

#include <QApplication>

int main(int argc, char *argv[])

{
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication a(argc, argv);
    MoneyProgress w;
    w.show();
    return a.exec();
}
