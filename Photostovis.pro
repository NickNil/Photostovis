TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    SyncManager.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    SyncManager.h

