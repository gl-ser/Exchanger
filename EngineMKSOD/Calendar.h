//----------------------------------------------------------------------------//
//         *** ДВИЖОК КОНВЕРТИРОВАНИЯ ЧИСЛОВЫХ И СТРОКОВЫХ ДАННЫХ ***         //
//                                                                            //
// Файл Calendar.h                                                            //
//                                                                            //
//       *** TCALENDAR КЛАСС СТАТИЧЕСКИХ МЕТОДОВ КОНВЕРТАЦИИ ВРЕМЕНИ ***      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2019 год //
//----------------------------------------------------------------------------//


#ifndef CalendarH
#define CalendarH

#include <QDateTime>


class TCalendar
{
  public:
    TCalendar(void);
    virtual ~TCalendar(void);

    //Переводит дату и время из формата TDateTime в формат QDateTime
    // TDateTime содержит количество суток, прошедших от 00:00 30.12.1899 г
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    static QDateTime TDateTimeToQDateTime(double Value);

    //Переводит дату и время из формата QDateTime в формат TDateTime
    // TDateTime содержит количество суток, прошедших от 00:00 30.12.1899 г
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    static double QDateTimeToTDateTime(QDateTime Value);

    //Переводит строку типа Qstring в формат QDateTime, разбирая вышеупомянутую строку по заданной пользователем маске
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // Примеры масок:
    // - "dd.MM.yyyy  HH:mm:ss.zzz";
    // - "dd.MM.yyyy  HH:mm:ss";
    // - "dd.MM.yyyy"
    static QDateTime QStringToQDateTime(QString Value, QString Mask);

    //Переводит дату и время из формата QDateTime в строку типа QString, создавая вышеупомянутую строку
    // по заданной пользователем маске
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // Примеры масок:
    // - "dd.MM.yyyy  HH:mm:ss.zzz";
    // - "dd.MM.yyyy  HH:mm:ss";
    // - "dd.MM.yyyy"
    static QString QDateTimeToQString(QDateTime Value, QString Mask);

    //Переводит строку типа Qstring в формат TDateTime, разбирая вышеупомянутую строку по заданной пользователем маске
    // TDateTime содержит количество суток, прошедших от 00:00 30.12.1899 г
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // Примеры масок:
    // - "dd.MM.yyyy  HH:mm:ss.zzz";
    // - "dd.MM.yyyy  HH:mm:ss";
    // - "dd.MM.yyyy"
    static double QStringToTDateTime(QString Value, QString Mask);

    //Переводит дату и время из формата TDateTime в строку типа QString, создавая вышеупомянутую строку
    // по заданной пользователем маске
    // TDateTime содержит количество суток, прошедших от 00:00 30.12.1899 г
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // Примеры масок:
    // - "dd.MM.yyyy  HH:mm:ss.zzz";
    // - "dd.MM.yyyy  HH:mm:ss";
    // - "dd.MM.yyyy"
    static QString TDateTimeToQString(double Value, QString Mask);

    //Разбирает дату и время из формата QDateTime на составные части
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // На вход подается:
    // - Value дата и время в формате QDateTime.
    // На выходе можно получить:
    // - DayInWeek день недели (1 - понедельник; 2 - вторник; 3 - среда; 4 - четверг; 5 - пятница; 6 - суббота; 7 - воскресенье);
    // - DayInYear номер дня в году (начинается с 1);
    // - WeekInYear номер недели в году (начинается с 1);
    // - Year год;
    // - Month месяц;
    // - Day день;
    // - Hour часы;
    // - Min минуты;
    // - Sec секунды;
    // - MilliSec милисекунды
    static void DecodeQDateTime(QDateTime Value, int *DayInWeek, int *DayInYear, int *WeekInYear, int *Year, int *Month,
                                int *Day, int *Hour, int *Min, int *Sec, int *MilliSec);

    //Разбирает дату и время из формата QDateTime на составные части
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // На вход подается:
    // - Value дата и время в формате QDateTime.
    // На выходе можно получить:
    // - DayInWeek день недели (1 - понедельник; 2 - вторник; 3 - среда; 4 - четверг; 5 - пятница; 6 - суббота; 7 - воскресенье);
    // - DayInYear номер дня в году (начинается с 1);
    // - WeekInYear номер недели в году (начинается с 1)
    static void DecodeQDateTime(QDateTime Value, int *DayInWeek, int *DayInYear, int *WeekInYear);

    //Разбирает дату и время из формата QDateTime на составные части
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // На вход подается:
    // - Value дата и время в формате QDateTime.
    // На выходе можно получить:
    // - Year год;
    // - Month месяц;
    // - Day день;
    // - Hour часы;
    // - Min минуты;
    // - Sec секунды;
    // - MilliSec милисекунды
    static void DecodeQDateTime(QDateTime Value, int *Year, int *Month, int *Day, int *Hour, int *Min, int *Sec, int *MilliSec);

    //Собирает дату и время в формате QDateTime из составных частей
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // На вход подаются:
    // - Year год;
    // - Month месяц;
    // - Day день;
    // - Hour часы;
    // - Min минуты;
    // - Sec секунды;
    // - MilliSec милисекунды
    static QDateTime EncodeQDateTime(int Year, int Month, int Day, int Hour, int Min, int Sec, int MilliSec);

    //Решает задачу пересечения двух временных отрезков First и Second в формате TDateTime
    // На вход подаются:
    // - FirstStart начало первого временного отрезка;
    // - FirstStop окончание первого временного отрезка;
    // - SecondStart начало второго временного отрезка;
    // - SecondStop окончание второго временного отрезка.
    // На выходе можно получить:
    // - ResultStart начало интервала пересечения двух временных отрезков (если пересечение есть);
    // - ResultStop окончание интервала пересечения двух временных отрезков (если пересечение есть).
    // Возвращает True, если временные отрезки пересекаются
    // Возвращает False, если временные отрезки не пересекаются (ResultStart и ResultStop равны нулю)
    // Считается, что есть пересечение, если оно состоит более чем из одной точки
    static bool CrosDiapasons(double FirstStart, double FirstStop, double SecondStart, double SecondStop,
                              double *ResultStart, double *ResultStop);
};


#endif
