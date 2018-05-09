#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T10:25:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CanDriveTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        TheMainForm.cpp \
    ControlCanDll.cpp \
    Message/messagedialog.cpp \
    Message/UiClass.cpp \
    Message/SCToastr.cpp \
    LoopADataForm.cpp \
    LoopBDataForm.cpp \
    ConfigDataForm.cpp \
    Setting/setting.cpp \
    SystemStatusForm.cpp \
    Message/MarqueeWidget.cpp \
    ConfigDataWriteForm.cpp

HEADERS += \
        TheMainForm.h \
    ControlCanDll.h \
    lib/ControlCAN.h \
    Message/messagedialog.h \
    Message/UiClass.h \
    Message/SCToastr.h \
    LoopADataForm.h \
    LoopBDataForm.h \
    ConfigDataForm.h \
    Setting/setting.h \
    SystemStatusForm.h \
    Message/MarqueeWidget.h \
    ConfigDataWriteForm.h

FORMS += \
        TheMainForm.ui \
    Message/messagedialog.ui \
    LoopADataForm.ui \
    LoopBDataForm.ui \
    ConfigDataForm.ui \
    SystemStatusForm.ui \
    ConfigDataWriteForm.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lControlCAN
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lControlCAN
else:unix: LIBS += -L$$PWD/lib/ -lControlCAN

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
