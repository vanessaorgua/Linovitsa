# -------------------------------------------------
# Project created by QtCreator 2010-02-25T14:01:57
# -------------------------------------------------
QT += network \
    sql svg

TARGET = test
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    form.cpp

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build

OTHER_FILES += list.txt \
    ../librcada.a

RESOURCES += test.qrc
QMAKE_LIBDIR += ../../lib
LIBS += -lrcada
INCLUDEPATH += ../../lib/include
HEADERS += form.h
FORMS += form.ui
