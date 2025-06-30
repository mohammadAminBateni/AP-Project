QT       += core gui
QT+=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editprofiledialog.cpp \
    forgotpassword.cpp \
    gamepage.cpp \
    gameresult.cpp \
    login.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    signupdialog.cpp \
    swapdialog.cpp \
    user.cpp

HEADERS += \
    editprofiledialog.h \
    forgotpassword.h \
    gamepage.h \
    gameresult.h \
    login.h \
    logindialog.h \
    mainwindow.h \
    menu.h \
    signupdialog.h \
    swapdialog.h \
    user.h

FORMS += \
    editprofiledialog.ui \
    forgotpassword.ui \
    gamepage.ui \
    gameresult.ui \
    login.ui \
    logindialog.ui \
    mainwindow.ui \
    menu.ui \
    signupdialog.ui \
    swapdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cardPictures.qrc \
    cards.qrc \
    loginPic.qrc \
    menuPic.qrc \
    result.qrc

DISTFILES += \
    menu.jpg
