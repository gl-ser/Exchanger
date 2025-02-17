//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл BasicThread.cpp                                                       //
//                                                                            //
//                     *** TBASICTHREAD БАЗОВЫЙ ПОТОК ***                     //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "BasicThread.h"


TBasicThread::TBasicThread()
{
  Number = -1;
  Start = 0.0;
  Stop = 0.0;
  Status = TStatus::NONE;
  ErrorNote = "";
}


TBasicThread::~TBasicThread()
{
  Number = -1;
  Start = 0.0;
  Stop = 0.0;
  Status = TStatus::NONE;
  ErrorNote = "";
}


void TBasicThread::run()
{
  CheckStart();

  if (Number < 0)
    TryError(Number, "Не задан номер потока");
  else
    TrySuccess(Number);
}


void TBasicThread::SetNumber(int _Number)
{
  if (Status != TStatus::WORK)
    Number = _Number;
}


int TBasicThread::GetNumber()
{
  return Number;
}


int TBasicThread::GetDuration()
{
  if ((Stop > Start) && (Start > 0))
  {
    return static_cast<int>((Stop-Start)*86400.0);
  }
  else
  {
    return 0;
  }
}


TBasicThread::TStatus TBasicThread::GetStatus()
{
bool res1, res2;
TStatus res;
  res1 = isRunning();
  res2 = isFinished();
  if (res1 == false)
  {
    if (res2 == false)
    {
      res = TStatus::NONE;
    }
    else
    {
      res = Status;
    }
  }
  else
  {
    if (res2 == false)
    {
      res = TStatus::WORK;
    }
    else
    {
      res = TStatus::WORK;
    }
  }
  return res;
}


QString TBasicThread::GetErrorNote()
{
  return ErrorNote;
}


void TBasicThread::CheckStart()
{
  Status = TStatus::WORK;
  Start = TCalendar::QDateTimeToTDateTime(QDateTime::currentDateTime());
  Stop = Start;
  ErrorNote = "";
}


void TBasicThread::TryError(int Number, QString Str)
{
  CheckStop();
  Status = TStatus::FINISH_ERROR;
  ErrorNote = Str;
  emit Error(Number, GetDuration(), Str);
}


void TBasicThread::TrySuccess(int Number)
{
  CheckStop();
  Status = TStatus::FINISH_SUCCESS;
  ErrorNote = "";
  emit Success(Number, GetDuration());
}


void TBasicThread::TryMessage(int Number, QString Str)
{
  emit Message(Number, Str);
}


void TBasicThread::CheckStop()
{
  Stop = TCalendar::QDateTimeToTDateTime(QDateTime::currentDateTime());
}
