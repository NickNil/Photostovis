TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    server.c \
    sha256.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    sha256.h

