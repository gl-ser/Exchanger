#----------------------------------------------------------------------------#
#       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      #
#                                                                            #
# Проект для Qt 5                                                            #
#                                                                            #
# Автор ГЛУЩЕНКО Сергей                                                      #
#                                                                            #
#                                                           Москва, 2025 год #
#----------------------------------------------------------------------------#

QT += core widgets network sql

TARGET = AbonentMKSOD  #Имя исполняемого файла
TEMPLATE = app

DEFINES += QUAZIP_STATIC
DEFINES += USE_FILE32API
#DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../Application  #Размещение исполняемого файла

LIBS += -L"$$PWD/../Application" -lQuaZIP  #Статическое подключение библиотеки QuaZIP
LIBS += -L"$$PWD/../Application" -lZLib    #Статическое подключение библиотеки ZLib
unix : LIBS += -L/usr/lib/ -lparsec-mac  #Для того, чтобы использовать мандатные метки под AstraLinux 1.6
win32: LIBS += -lwsock32  #Подключить библиотеку сокетов под Windows
win32: RC_ICONS = Picture/Pic4.ico  #Пиктограмма приложения

SOURCES += \
    Main.cpp \
    About.cpp \
    Base.cpp \
    Wait.cpp \
    Error.cpp \
    ../ClassLibrary/Placement.cpp \
    CancelOK.cpp \
    WinTuner.cpp \
    WinLogBook.cpp \
    SQLiteDB.cpp \
    CtrlOUTCat.cpp \
    WinAddAbonent.cpp \
    WinEditAbonent.cpp \
    WinListAbonent.cpp \
    MainWin.cpp \
    WinFind.cpp \
    ../EngineMKSOD/BasicServerTCP.cpp \
    ../EngineMKSOD/BasicThread.cpp \
    ../EngineMKSOD/ByteBuffer.cpp \
    ../EngineMKSOD/Calendar.cpp \
    ../EngineMKSOD/CheckSum.cpp \
    ../EngineMKSOD/File.cpp \
    ../EngineMKSOD/NetReceiveThread.cpp \
    ../EngineMKSOD/NetSendThread.cpp \
    ../EngineMKSOD/ReceiveFolderServer.cpp \
    ../EngineMKSOD/ReceivePartFileThread.cpp \
    ../EngineMKSOD/SendFolderThread.cpp \
    ../EngineMKSOD/SendKvitThread.cpp \
    ../EngineMKSOD/SendPartFileThread.cpp \
    ../EngineMKSOD/ZIPThread.cpp

HEADERS += \
    About.h \
    Wait.h \
    Base.h \
    Error.h \
    ../ClassLibrary/Placement.h \
    CancelOK.h \
    WinTuner.h \
    WinLogBook.h \
    SQLiteDB.h \
    CtrlOUTCat.h \
    WinListAbonent.h \
    WinEditAbonent.h \
    WinAddAbonent.h \
    MainWin.h \
    WinFind.h \
    ../EngineMKSOD/BasicServerTCP.h \
    ../EngineMKSOD/BasicThread.h \
    ../EngineMKSOD/ByteBuffer.h \
    ../EngineMKSOD/Calendar.h \
    ../EngineMKSOD/CheckSum.h \
    ../EngineMKSOD/File.h \
    ../EngineMKSOD/NetReceiveThread.h \
    ../EngineMKSOD/NetSendThread.h \
    ../EngineMKSOD/ReceiveFolderServer.h \
    ../EngineMKSOD/ReceivePartFileThread.h \
    ../EngineMKSOD/SendFolderThread.h \
    ../EngineMKSOD/SendKvitThread.h \
    ../EngineMKSOD/SendPartFileThread.h \
    ../EngineMKSOD/ZIPThread.h

FORMS += \
    About.ui \
    Wait.ui \
    CancelOK.ui \
    Error.ui \
    WinTuner.ui \
    WinLogBook.ui \
    WinAddAbonent.ui \
    WinEditAbonent.ui \
    WinListAbonent.ui \
    MainWin.ui \
    WinFind.ui

RESOURCES += \
    IconsForMenu.qrc
