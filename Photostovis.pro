TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    SyncManager.c \
    client.c \
<<<<<<< Updated upstream \
    FileManager.c \
    sha256.c
    sha256.c \
    FileManager.c
=======
    FileManager.c \
    sha256.c
>>>>>>> Stashed changes

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    SyncManager.h \
    client.h \
    FileManager.h \
    sha256.h
<<<<<<< Updated upstream
=======

DISTFILES +=
>>>>>>> Stashed changes

