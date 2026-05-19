QT       += core gui
QT       += network
QT       += charts
QT += concurrent
QT += core gui axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++18





# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    excelreader.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    qlog.cpp \
    receive.cpp \
    self_test.cpp \
    work.cpp

HEADERS += \
    excelreader.h \
    login.h \
    mainwindow.h \
    qcustomplot.h \
    qlog.h \
    receive.h \
    self_test.h \
    work.h

FORMS += \
    login.ui \
    mainwindow.ui \
    self_test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Rc.qrc

DISTFILES += \
    Icon/1606455890-2961.jpg \
    Icon/1606455890-2961.png \
    Icon/Backhome.png \
    Icon/Lock.png \
    Icon/background.webp \
    Icon/connect.png \
    Icon/disconnect.png \
    Icon/icon.png \
    Icon/unLock.png

INCLUDEPATH += C:\Program Files (x86)\GE Intelligent Platforms\RFM2g\Inc


win32: LIBS += -L"C:\Program Files (x86)\GE Intelligent Platforms\RFM2g\Lib" -lrfm2gdll_stdc_32

INCLUDEPATH += "C:\Program Files (x86)\GE Intelligent Platforms\RFM2g\Lib"
DEPENDPATH += "C:\Program Files (x86)\GE Intelligent Platforms\RFM2g\Lib"

win32:!win32-g++: PRE_TARGETDEPS += "C:\Program Files (x86)\GE Intelligent Platforms\RFM2g\Lib\rfm2gdll_stdc_32"
