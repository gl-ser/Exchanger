//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SendFolderThread.cpp                                                  //
//                                                                            //
//     *** TSENDFOLDERTHREAD ПОТОК ОТПРАВКИ КАТАЛОГА ПО ЛОКАЛЬНОЙ СЕТИ ***    //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//

#include <QCoreApplication>
#include "SendFolderThread.h"


TSendFolderThread::TSendFolderThread()
{
  TempCatalog = "";
  CatPathName = "";
  _GUID = "";
  MD5 = "";
  FileSize = 0LL;

  QProc = static_cast<signed int>(std::thread::hardware_concurrency());
  if (QProc <= 0) QProc = 4;
  //Запросил кол-во процессоров
//QProc=1;
  ClearParts();
}


TSendFolderThread::~TSendFolderThread()
{
  TempCatalog = "";
  CatPathName = "";
  _GUID = "";
  MD5 = "";
  FileSize = 0LL;
  QProc = 0;

  ClearParts();
}


void TSendFolderThread::run()
{
int q, dlit;
unsigned int i;
bool pr, pr2;
TFile::TFileIndexes Indexes;

  CheckStart();
  ClearParts();

  if (GetNumber() < 0)
  {
    TryError(GetNumber(), "Не задан номер потока");
  }
  else
  {
    if (IsNotEmptyAbonent() == true)
    {
      if (TempCatalog != "")
      {
        if (CatPathName != "")
        {
          //TryMessage(GetNumber(), "Начало передачи каталога");
          TryMessage(GetNumber(), "Начата упаковка каталога " + CatPathName);

          _GUID = QUuid::createUuid().toString();  //Генерация нового GUID'а
          FilePathName = TempCatalog + "/" + _GUID;

          ZIP.SetNumber(GetNumber()*100+1);
          ZIP.SetPaths(CatPathName, FilePathName);
          ZIP.CompressDir();
          ZIP.start();

          ZIP.quit();
          ZIP.wait();

          if (ZIP.GetStatus() == TStatus::FINISH_SUCCESS)
          {
            TryMessage(GetNumber(), "Упаковка каталога " + CatPathName + " в файл " + _GUID +
                       " завершена за " + QString::number(ZIP.GetDuration()) + " сек");

            MD5 = TCheckSum::FileMD5(FilePathName);  //Вычисление контрольной суммы файла по алгоритму MD5

            FileSize = TFile::GetFileSize(FilePathName);

            //TryMessage(GetNumber(), "Создан файл " + FilePathName);
            //TryMessage(GetNumber(), "Размер файла " + QString::number(FileSize) + " байт");
            //TryMessage(GetNumber(), "Контрольная сумма " + MD5);

            if (FileSize <= static_cast<long long>(QProc))
              q = 1;
            else
              q = QProc;

            Indexes = TFile::SplitFile(FileSize, q);
            if (Indexes.size() >= 1)
            {
              //TryMessage(GetNumber(), "Файл успешно разделен на " + QString::number(Indexes.size()) + " частей");

              Parts.resize(Indexes.size());
              for(i=0;i<Parts.size();i++)
              {
                Parts[i] = new TSendPartFileThread();
                Parts[i]->SetNumber(GetNumber()*201+static_cast<int>(i));
                Parts[i]->SetAnswerPortTCP(GetAnswerPortTCP());
                Parts[i]->SetAbonent(GetAbonent().Name, GetAbonent().Address, GetAbonent().PortTCP);
                Parts[i]->SendPartFile(FilePathName, _GUID, MD5, FileSize, Indexes[i].start, Indexes[i].size,
                                       static_cast<int>(i), static_cast<int>(Parts.size()));
                Parts[i]->start();
              }

              TryMessage(GetNumber(), "Начата сетевая передача файла " + _GUID + " абоненту    Имя: " + GetAbonent().Name +
                                 "    IP-адрес: " + GetAbonent().Address +
                                 "    TCP-порт: " + QString::number(GetAbonent().PortTCP) +
                                 "    в " + QString::number(Parts.size()) + " потоков");
              //TryMessage(GetNumber(), "Начата сетевая передача в " + QString::number(Parts.size()) + " потоков");

              for(i=0;i<Parts.size();i++)
              {
                Parts[i]->quit();
                Parts[i]->wait();
              }

              pr = true;
              for(i=0;i<Parts.size();i++)
              if (Parts[i]->GetStatus() == TStatus::FINISH_ERROR)
              {
                pr = false;
                TryError(GetNumber(), Parts[i]->GetErrorNote());
                break;
              }

              if (pr == true)
              {
                dlit = 0;
                for(i=0;i<Parts.size();i++)
                if (Parts[i]->GetDuration() >= dlit)
                  dlit = Parts[i]->GetDuration();

                TryMessage(GetNumber(), "Сетевая передача файла " + _GUID + " абоненту    Имя: " + GetAbonent().Name +
                                   "    IP-адрес: " + GetAbonent().Address +
                                   "    TCP-порт: " + QString::number(GetAbonent().PortTCP) +
                                   "    завершена за " + QString::number(dlit) + " сек");
                //TryMessage(GetNumber(), "Сетевая передача завершена за " + QString::number(dlit) + " сек");
              }

              pr2 = TFile::DelFile(FilePathName);  //Удаление файла с диска
              if (pr2 == false)
              {
                TryError(GetNumber(), "Ошибка удаления временного файла");
              }

              if ((pr == true) && (pr2 == true))
                TrySuccess(GetNumber());

              //TryMessage(GetNumber(), "Конец передачи каталога");
            }
            else
            {
              TryError(GetNumber(), "Ошибка разделения файла на части");
            }
          }
          else
          {
            TryError(GetNumber(), ZIP.GetErrorNote());
          }
        }
        else
        {
          TryError(GetNumber(), "Не задан каталог для отправки");
        }
      }
      else
      {
        TryError(GetNumber(), "Не задан временный каталог");
      }
    }
    else
    {
      TryError(GetNumber(), "Не заданы атрибуты сетевого абонента");
    }
  }
  Indexes.clear();
}


void TSendFolderThread::SetTempCatalog(QString _TempCatalog)
{
  if (GetStatus() != TStatus::WORK)
    TempCatalog = _TempCatalog;
}


void TSendFolderThread::SendFolder(QString _CatPathName)
{
  if (GetStatus() != TStatus::WORK)
    CatPathName = _CatPathName;
}


void TSendFolderThread::ClearParts()
{
unsigned int i;
  for(i=0; i<Parts.size(); i++)
  if (Parts[i] != NULL)
  {
    delete Parts[i];
    Parts[i] = NULL;
  }
  Parts.clear();
}
