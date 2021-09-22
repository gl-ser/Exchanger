QT -= core gui

TARGET = zlib
TEMPLATE = lib

#DEFINES += UNTITLED1_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../App  #Размещение исполняемого файла

CONFIG += link_pkgconfig plugin no_plugin_name_prefix  #Убрать префиксы и постфиксы в имени библиотеки (опции компилятора)

unix : CONFIG += static

DEF_FILE += ZLib.def  #Подключение DEF-файла, направленного против замангливания имен компилятором

HEADERS += \
    crc32.h \
    deflate.h \
    inffast.h \
    inffixed.h \
    inflate.h \
    inftrees.h \
    trees.h \
    ../HeaderDLL/zconf.h \
    zconf.in.h \
    zutil.h \
    ../HeaderDLL/zlib.h

SOURCES += \
    adler32.c \
    compress.c \
    crc32.c \
    deflate.c \
    example.c \
    gzio.c \
    infback.c \
    inffast.c \
    inflate.c \
    inftrees.c \
    trees.c \
    uncompr.c \
    zutil.c
