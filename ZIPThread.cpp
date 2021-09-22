//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ZIPThread.cpp                                                         //
//                                                                            //
//              *** TZIPTHREAD ПОТОК РАБОТЫ С АРХИВАТОРОМ ZIP ***             //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "ZIPThread.h"


TZIPThread::TZIPThread()
{
  DirPathName = "";
  ZIPFilePathName = "";
  Task = TTask::NONE;
}


TZIPThread::~TZIPThread()
{
  DirPathName = "";
  ZIPFilePathName = "";
  Task = TTask::NONE;
}


void TZIPThread::run()
{
bool res1;
QStringList res2;

  CheckStart();

  if (GetNumber() < 0)
  {
    TryError(GetNumber(), "Не задан номер потока");
  }
  else
  {
    if ((DirPathName == "") || (ZIPFilePathName == ""))
    {
      TryError(GetNumber(), "Не заданы пути к папке и\\или к ZIP-файлу");
    }
    else
    {
      switch (Task)
      {
        case TTask::NONE:
          //Задача не поставлена
          TryError(GetNumber(), "Не поставлена задача на сжатие\\распаковку");
        break;

        case TTask::COMPRESS_DIR:
          //Сжатие каталога в ZIP-архив
          res1 = JlCompress::compressDir(ZIPFilePathName, DirPathName);

          if (res1 == true)
          {
            TrySuccess(GetNumber());
          }
          else
          {
            TryError(GetNumber(), "Ошибка сжатия каталога");
          }
        break;

        case TTask::EXTRACT_DIR:
          //Распаковка ZIP-архива в каталог
          res2 = JlCompress::extractDir(ZIPFilePathName, DirPathName);

          if (res2.count() > 0)
          {
            TrySuccess(GetNumber());
          }
          else
          {
            TryError(GetNumber(), "Ошибка распаковки ZIP-архива");
          }
        break;
      }
    }
  }
}


void TZIPThread::SetPaths(QString _DirPathName, QString _ZIPFilePathName)
{
  if (GetStatus() != TStatus::WORK)
  {
    DirPathName = _DirPathName;
    ZIPFilePathName = _ZIPFilePathName;
  }
}


void TZIPThread::CompressDir()
{
  if (GetStatus() != TStatus::WORK)
    Task = TTask::COMPRESS_DIR;
}


void TZIPThread::ExtractDir()
{
  if (GetStatus() != TStatus::WORK)
    Task = TTask::EXTRACT_DIR;
}


QString TZIPThread::GetDirPathName()
{
  return DirPathName;
}


QString TZIPThread::GetZIPFilePathName()
{
  return ZIPFilePathName;
}
