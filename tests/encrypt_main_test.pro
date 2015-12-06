TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../boyer_moore.c \
    ../error_mgmt.c \
    ../scrambler.c \
    ../encrypt_main.c \
    encrypt_main_test.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../boyer_moore.h \
    ../error_mgmt.h \
    ../scrambler.h \
    ../encrypt_main.h

