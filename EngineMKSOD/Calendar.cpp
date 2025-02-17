//----------------------------------------------------------------------------//
//         *** ДВИЖОК КОНВЕРТИРОВАНИЯ ЧИСЛОВЫХ И СТРОКОВЫХ ДАННЫХ ***         //
//                                                                            //
// Файл Calendar.cpp                                                          //
//                                                                            //
//       *** TCALENDAR КЛАСС СТАТИЧЕСКИХ МЕТОДОВ КОНВЕРТАЦИИ ВРЕМЕНИ ***      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2019 год //
//----------------------------------------------------------------------------//


#include "Calendar.h"
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <math.h>


TCalendar::TCalendar()
{
  //Пустой конструктор
}


TCalendar::~TCalendar()
{
  //Пустой деструктор
}


QDateTime TCalendar::TDateTimeToQDateTime(double Value)
{
QDateTime dt;
  if (Value >= 25569.0)
  {
    dt = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>((Value-25569.0)*86400.0*1000.0), Qt::OffsetFromUTC, 0);
  }
  else
  {
    dt = (QDateTime::fromString("01.01.1970 00:00:00", "dd.MM.yyyy HH:mm:ss"));
    dt.setTimeSpec(Qt::UTC);
  }
  return dt;
}


double TCalendar::QDateTimeToTDateTime(QDateTime Value)
{
QDateTime dt;
double res;
  dt = (QDateTime::fromString("01.01.1970 00:00:00", "dd.MM.yyyy HH:mm:ss"));
  dt.setTimeSpec(Qt::UTC);
  Value.setTimeSpec(Qt::UTC);

  if (Value.toMSecsSinceEpoch() >= dt.toMSecsSinceEpoch())
  {
    res = (static_cast<double>(Value.toMSecsSinceEpoch()))/1000.0/86400.0+25569.0;
  }
  else
  {
    res = (static_cast<double>(dt.toMSecsSinceEpoch()))/1000.0/86400.0+25569.0;
  }
  return res;
}


QDateTime TCalendar::QStringToQDateTime(QString Value, QString Mask)
{
QDateTime dt;
  dt = QDateTime::fromString(Value, Mask);
  dt.setTimeSpec(Qt::UTC);
  return dt;
}


QString TCalendar::QDateTimeToQString(QDateTime Value, QString Mask)
{
QString res;
  res = Value.toString(Mask);
  return res;
}


double TCalendar::QStringToTDateTime(QString Value, QString Mask)
{
QDateTime dt;
double res;
  dt = TCalendar::QStringToQDateTime(Value, Mask);
  res = TCalendar::QDateTimeToTDateTime(dt);
  return res;
}


QString TCalendar::TDateTimeToQString(double Value, QString Mask)
{
QDateTime dt;
QString res;
  dt = TCalendar::TDateTimeToQDateTime(Value);
  res = TCalendar::QDateTimeToQString(dt, Mask);
  return res;
}


void TCalendar::DecodeQDateTime(QDateTime Value, int *DayInWeek, int *DayInYear, int *WeekInYear, int *Year, int *Month,
                                int *Day, int *Hour, int *Min, int *Sec, int *MilliSec)
{
  Value.setTimeSpec(Qt::UTC);
  *DayInWeek = Value.date().dayOfWeek();
  *DayInYear = Value.date().dayOfYear();
  *WeekInYear = Value.date().weekNumber();
  *Year = Value.date().year();
  *Month = Value.date().month();
  *Day = Value.date().day();
  *Hour = Value.time().hour();
  *Min = Value.time().minute();
  *Sec = Value.time().second();
  *MilliSec = Value.time().msec();
}


void TCalendar::DecodeQDateTime(QDateTime Value, int *DayInWeek, int *DayInYear, int *WeekInYear)
{
int Year, Month, Day, Hour, Min, Sec, MilliSec;
  TCalendar::DecodeQDateTime(Value, DayInWeek, DayInYear, WeekInYear, &Year, &Month, &Day, &Hour, &Min, &Sec, &MilliSec);
}


void TCalendar::DecodeQDateTime(QDateTime Value, int *Year, int *Month, int *Day, int *Hour, int *Min, int *Sec, int *MilliSec)
{
int DayInWeek, DayInYear, WeekInYear;
  TCalendar::DecodeQDateTime(Value, &DayInWeek, &DayInYear, &WeekInYear, Year, Month, Day, Hour, Min, Sec, MilliSec);
}


QDateTime TCalendar::EncodeQDateTime(int Year, int Month, int Day, int Hour, int Min, int Sec, int MilliSec)
{
QDateTime dt;
QDate d;
QTime t;
  d.setDate(Year, Month, Day);
  t.setHMS(Hour, Min, Sec, MilliSec);
  dt.setDate(d);
  dt.setTime(t);
  dt.setTimeSpec(Qt::UTC);
  return dt;
}


bool TCalendar::CrosDiapasons(double FirstStart, double FirstStop, double SecondStart, double SecondStop,
                              double *ResultStart, double *ResultStop)
{
bool result;
double DELTA = 1.0/86400.0/1000.0;  //Одна тысячная секунды
  //Всего существует 13 вариантов взаимного расположения двух временных отрезков.
  // Значками ==== обозначаю отрезок First
  // Значками ---- обозначаю отрезок Second
  // ВАРИАНТЫ:
  //  1. ====  ----
  //
  //  2. ----  ====
  //
  //  3. ====
  //       ----
  //
  //  4. ----
  //       ====
  //
  //  5.   ----
  //     ========
  //
  //  6.   ====
  //     --------
  //
  //  7. ====
  //     ----
  //
  //  8.    ---
  //     ======
  //
  //  9.    ===
  //     ------
  //
  // 10. ---
  //     ======
  //
  // 11. ===
  //     ------
  //
  // 12.     ----
  //     ====
  //
  // 13.     ====
  //     ----

  *ResultStart = 0.0;
  *ResultStop = 0.0;

  result = true;

  if ((SecondStart >= FirstStop) || (FirstStart >= SecondStop))
  {  //Варианты 1, 2, 12, 13
    result = false;
    goto endd;
  }

  if ((SecondStart > FirstStart) && (SecondStart < FirstStop) && (SecondStop > FirstStop))
  {  //Вариант 3
    *ResultStart = SecondStart;
    *ResultStop = FirstStop;
    goto endd;
  }

  if ((FirstStart > SecondStart) && (FirstStart < SecondStop) && (FirstStop > SecondStop))
  {  //Вариант 4
    *ResultStart = FirstStart;
    *ResultStop = SecondStop;
    goto endd;
  }

  if ((SecondStart > FirstStart) && (SecondStart < FirstStop) && (SecondStop > FirstStart) && (SecondStop < FirstStop))
  {  //Вариант 5
    *ResultStart = SecondStart;
    *ResultStop = SecondStop;
    goto endd;
  }

  if ((FirstStart > SecondStart) && (FirstStart < SecondStop) && (FirstStop > SecondStart) && (FirstStop < SecondStop))
  {  //Вариант 6
    *ResultStart = FirstStart;
    *ResultStop = FirstStop;
    goto endd;
  }

  if ((fabs(FirstStart - SecondStart) <= DELTA)  && (fabs(FirstStop - SecondStop) <= DELTA))
  {  //Вариант 7
    *ResultStart = FirstStart;
    *ResultStop = FirstStop;
    goto endd;
  }

  if ((SecondStart > FirstStart) && (SecondStart < FirstStop) && (fabs(SecondStop - FirstStop) <= DELTA))
  {  //Вариант 8
    *ResultStart = SecondStart;
    *ResultStop = SecondStop;
    goto endd;
  }

  if ((FirstStart > SecondStart) && (FirstStart < SecondStop) && (fabs(FirstStop - SecondStop) <= DELTA))
  {  //Вариант 9
    *ResultStart = FirstStart;
    *ResultStop = FirstStop;
    goto endd;
  }

  if ((fabs(SecondStart - FirstStart) <= DELTA) && (SecondStop > FirstStart) && (SecondStop < FirstStop))
  {  //Вариант 10
    *ResultStart = SecondStart;
    *ResultStop = SecondStop;
    goto endd;
  }

  if ((fabs(FirstStart - SecondStart) <= DELTA) && (FirstStop > SecondStart) && (FirstStop < SecondStop))
  {  //Вариант 11
    *ResultStart = FirstStart;
    *ResultStop = FirstStop;
    goto endd;
  }

endd:
  return result;
}
