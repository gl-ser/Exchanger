QT += core widgets network sql

TARGET = quazip
TEMPLATE = lib

DEFINES += QUAZIP_BUILD #QUAZIP_STATIC
DEFINES += NOMINMAX
DEFINES += NOCRYPT USE_FILE32API

DESTDIR = $$PWD/../App  #Размещение исполняемого файла

CONFIG += link_pkgconfig plugin no_plugin_name_prefix  #Убрать префиксы и постфиксы в имени библиотеки (опции компилятора)

unix : CONFIG += static

LIBS += -L"$$PWD/../App" -lzlib  #Статическое подключение библиотеки zlib

SOURCES += \
    JlCompress.cpp \
    qioapi.cpp \
    quaadler32.cpp \
    quachecksum32.cpp \
    quacrc32.cpp \
    quagzipfile.cpp \
    quaziodevice.cpp \
    quazip.cpp \
    quazipdir.cpp \
    quazipfile.cpp \
    quazipfileinfo.cpp \
    quazipnewinfo.cpp \
    unzip.c \
    zip.c

HEADERS += \
    ../HeaderDLL/ioapi.h \
    ../HeaderDLL/JlCompress.h \
    ../HeaderDLL/quazip.h \
    ../HeaderDLL/quazip_global.h \
    ../HeaderDLL/quazip_qt_compat.h \
    ../HeaderDLL/quazipfile.h \
    ../HeaderDLL/quazipfileinfo.h \
    ../HeaderDLL/quazipnewinfo.h \
    ../HeaderDLL/unzip.h \
    ../HeaderDLL/zconf.h \
    ../HeaderDLL/zip.h \
    ../HeaderDLL/zlib.h \
    ../HeaderDLL/Tuner.h \
    minizip_crypt.h \
    quaadler32.h \
    quachecksum32.h \
    quacrc32.h \
    quagzipfile.h \
    quaziodevice.h \
    quazipdir.h
