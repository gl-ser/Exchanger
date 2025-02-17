Движок Многопоточной Кроссплатформенной Системы Обмена Данными

Для использования полного функционала библиотеки необходимо в pro-файл добавить следующее:

QT += network

DEFINES += QUAZIP_STATIC
DEFINES += USE_FILE32API
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L"$$PWD/../Application" -lQuaZIP  #Статическое подключение библиотеки QuaZIP
LIBS += -L"$$PWD/../Application" -lZLib    #Статическое подключение библиотеки ZLib

unix : LIBS += -L/usr/lib/ -lparsec-mac  #Для того, чтобы использовать мандатные метки под AstraLinux 1.6

win32: LIBS += -lwsock32  #Подключить библиотеку сокетов под Windows

Для работы под Astra Linux на всех уровнях доступа библиотека кроссплатформенных классов требует, чтобы запускающему её пользователю были выставлены 3 следующие привилегии:

parsec_cap_priv_sock
parsec_cap_mac_sock
parsec_cap_chmac
