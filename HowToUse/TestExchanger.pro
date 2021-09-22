QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestExchanger
TEMPLATE = app

DEFINES += QUAZIP_STATIC
DEFINES += USE_FILE32API
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../App  #Размещение исполняемого файла

LIBS += -L"$$PWD/../App" -lquazip  #Статическое подключение библиотеки quazip
LIBS += -L"$$PWD/../App" -lzlib    #Статическое подключение библиотеки zlib
unix : LIBS += -L/usr/lib/ -lparsec-mac  #Для того, чтобы использовать мандатные метки под AstraLinux 1.6
win32: LIBS += -lwsock32  #Подключить библиотеку сокетов под Windows

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ../../Calendar/Calendar.cpp \
    ../BasicThread.cpp \
    ../ZIPThread.cpp \
    ../File.cpp \
    ../CheckSum.cpp \
    ../NetSendThread.cpp \
    ../SendPartFileThread.cpp \
    ../SendFolderThread.cpp \
    ../NetReceiveThread.cpp \
    ../ReceivePartFileThread.cpp \
    ../BasicServerTCP.cpp \
    ../ReceiveFolderServer.cpp \
    ../ByteBuffer.cpp \
    ../SendKvitThread.cpp

HEADERS += \
    ../HeaderDLL/JlCompress.h \
    ../HeaderDLL/quazip.h \
    ../HeaderDLL/quazip_qt_compat.h \
    ../HeaderDLL/quazipfile.h \
    ../HeaderDLL/quazipfileinfo.h \
    ../HeaderDLL/quazip_global.h \
    ../HeaderDLL/unzip.h \
    ../HeaderDLL/zip.h \
    ../HeaderDLL/ioapi.h \
    ../HeaderDLL/quazipnewinfo.h \
    ../../Calendar/Calendar.h \
    ../BasicThread.h \
    ../ZIPThread.h \
    MainWindow.h \
    ../HeaderDLL/Tuner.h \
    ../File.h \
    ../CheckSum.h \
    ../NetSendThread.h \
    ../SendPartFileThread.h \
    ../SendFolderThread.h \
    ../NetReceiveThread.h \
    ../ReceivePartFileThread.h \
    ../BasicServerTCP.h \
    ../ReceiveFolderServer.h \
    ../ByteBuffer.h \
    ../SendKvitThread.h

FORMS += \
        MainWindow.ui
