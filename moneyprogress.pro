QT += widgets

CONFIG += c++17

TARGET = MoneyProgress_win
TEMPLATE = app

HEADERS += \
    moneyprogress.h \
    message.h \
    bar.h

SOURCES += \
    main.cpp \
    moneyprogress.cpp \
    message.cpp \
    bar.cpp

FORMS += \
    moneyprogress.ui \
    message.ui \
    bar.ui

RESOURCES += \
    img.qrc \
    logo.rc


