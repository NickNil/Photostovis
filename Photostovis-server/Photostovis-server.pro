TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    server.c

include(deployment.pri)
qtcAddDeployment()

