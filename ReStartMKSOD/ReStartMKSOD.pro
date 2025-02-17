#----------------------------------------------------------------------------#
#       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      #
#                                                                            #
# Проект для Qt 5                                                            #
#                                                                            #
# Автор ГЛУЩЕНКО Сергей                                                      #
#                                                                            #
#                                                           Москва, 2021 год #
#----------------------------------------------------------------------------#

QT += core widgets

TARGET = ReStartMKSOD  #Имя исполняемого файла
TEMPLATE = app

#DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../Application  #Размещение исполняемого файла

win32 : RC_ICONS = Picture/Pic1.ico  #Пиктограмма приложения

SOURCES += \
    Main.cpp \
    ../ClassLibrary/Placement.cpp \
    MainWin.cpp

HEADERS += \
    ../ClassLibrary/Placement.h \
    MainWin.h

FORMS += \
    MainWin.ui

RESOURCES += \
    IconsForMenu.qrc
