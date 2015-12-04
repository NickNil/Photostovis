TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    SyncManager.c \
    client.c \
    sha256.c \
    FileManager.c \
    boyer_moore.c \
    error_mgmt.c \
    scrambler.c \
    encrypt_main.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    SyncManager.h \
    client.h \
    FileManager.h \
    sha256.h \
    boyer_moore.h \
    error_mgmt.h \
    scrambler.h \
    encrypt_main.h

