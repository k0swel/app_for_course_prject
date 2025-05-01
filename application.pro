QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = application
TEMPLATE = app

CONFIG -=debug_and_release
CONFIG += release

DESTDIR = $$PWD/build

OBJECTS_DIR = ./build/obj
UI_DIR = ./build/ui
MOC_DIR = ./build/moc
RCC_DIR = ./build/rcc

INCLUDEPATH = $$PWD/headers

SOURCES += \
    $$PWD/source/database.cpp \
    $$PWD/source/database_window.cpp \
    $$PWD/source/main.cpp \
    $$PWD/source/window_query.cpp \
    $$PWD/source/notification.cpp \
    $$PWD/source/table_form.cpp \
    $$PWD/source/json_manage.cpp

HEADERS += \
    $$PWD/headers/database.h \
    $$PWD/headers/database_window.h \
    $$PWD/headers/window_query.h \
    $$PWD/headers/notification.h \
    $$PWD/headers/table_form.h \
    $$PWD/headers/json_manage.h

FORMS += \
    $$PWD/forms/database_window.ui \
    $$PWD/forms/window_query.ui \
    $$PWD/forms/notification.ui \
    $$PWD/forms/table_form.ui
	
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
