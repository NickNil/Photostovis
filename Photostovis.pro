TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    SyncManager.c \
    client.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    SyncManager.h \
    client.h

