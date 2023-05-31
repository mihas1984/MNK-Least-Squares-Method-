QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filebrowser.cpp \
    helpdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mnk.cpp \
    mnkwindow.cpp \
    readfile.cpp \
    skvwindow.cpp

HEADERS += \
    exceptions.hpp \
    filebrowser.h \
    helpdialog.h \
    mainwindow.h \
    mnk.hpp \
    mnkwindow.h \
    readfile.hpp \
    skvwindow.h \
    student_coeffs.hpp

FORMS += \
    filebrowser.ui \
    helpdialog.ui \
    mainwindow.ui \
    mnkwindow.ui \
    skvwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
