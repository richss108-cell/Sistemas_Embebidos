TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        compression.c \
        filesystem.c \
        main.c \
        tree.c

HEADERS += \
    compression.h \
    filesystem.h \
    tree.h
