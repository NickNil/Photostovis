TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    SyncManager.c \
    client.c \
    sha256.c \
    FileManager.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    SyncManager.h \
    client.h \
    FileManager.h \
    sha256.h

