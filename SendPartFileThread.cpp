//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SendPartFileThread.cpp                                                //
//                                                                            //
//  *** TSENDPARTFILETHREAD ПОТОК ОТПРАВКИ ЧАСТИ ФАЙЛА ПО ЛОКАЛЬНОЙ СЕТИ ***  //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "SendPartFileThread.h"


TSendPartFileThread::TSendPartFileThread()
{
  FilePathName = "";
  FileNameGUID = "";
  FileSize = 0LL;
  MD5 = "";
  BufferSize = 0LL;
  StartIndex = -1LL;
  IndexPart = -1;
  CountParts = -1;
}


TSendPartFileThread::~TSendPartFileThread()
{
  FilePathName = "";
  FileNameGUID = "";
  FileSize = 0LL;
  MD5 = "";
  BufferSize = 0LL;
  StartIndex = -1LL;
  IndexPart = -1;
  CountParts = -1;
}


void TSendPartFileThread::run()
{
int p;
bool pr;
long long Size;
QFile F;
QByteArray Data;

  CheckStart();

  if (GetNumber() < 0)
  {
    TryError(GetNumber(), "Не задан номер потока");
  }
  else
  {
    if (IsNotEmptyAbonent() == true)
    {
      if ((FilePathName != "") && (FileNameGUID != "") && (FileSize > 0LL) && (MD5 != "") &&
          (BufferSize > 0LL) && (StartIndex != -1) && (IndexPart != -1) && (CountParts != -1))
      {
        F.setFileName(FilePathName);
        if (F.open(QIODevice::ReadOnly) == true)
        {
          //Подключиться к удаленному компьютеру
          pr = Connect();

          if (pr == true)
          {
            pr = SendSmallData(QString("<MKSOD_FILE>").toStdString().c_str(), 12);
            //В сокет передал ключевое слово TCP-сообщения
            if (pr == true)
            {
              pr = SendSmallData(FileNameGUID.toStdString().c_str(), 38);
              //В сокет передал GUID (транспортное имя файла)
              if (pr == true)
              {
                pr = SendSmallData(MD5.toStdString().c_str(), 32);
                //В сокет передал контрольную сумму
                if (pr == true)
                {
                  pr = SendSmallData((char*)&FileSize, sizeof(long long));
                  //В сокет передал размер файла
                  if (pr == true)
                  {
                    pr = SendSmallData((char*)&StartIndex, sizeof(long long));
                    //В сокет передал позицию начала в файле передаваемой области
                    if (pr == true)
                    {
                      pr = SendSmallData((char*)&IndexPart, sizeof(int));
                      //В сокет передал порядковый номер передаваемой области (части) файла
                      if (pr == true)
                      {
                        pr = SendSmallData((char*)&CountParts, sizeof(int));
                        //В сокет передал количество областей (частей) файла
                        if (pr == true)
                        {
                          p = GetAnswerPortTCP();
                          pr = SendSmallData((char*)&p, sizeof(int));
                          //В сокет передал номер порта для ответа (для получения квитанции)
                          if (pr == true)
                          {
                            F.seek(StartIndex);
                            //Size = 1000000LL;
                            Size = 10000000LL;

                            if (BufferSize < Size)
                              Size = BufferSize;

                            while (!F.atEnd())
                            {
                              Data = F.read(Size);
                              BufferSize = BufferSize - static_cast<long long>(Data.size());
                              pr = SendBigData(Data.constData(), Data.size());
                              Data.clear();

                              if (BufferSize <= 0)
                                break;

                              if (pr == false)
                                break;
                            }
                            //В сокет передал содержимое области файла
                            //Отправка завершена
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          F.close();
          Data.clear();

          //Разорвать соединение с удаленным компьютером и закрыть сокет
          Disconnect();

          if (pr == true)
            TrySuccess(GetNumber());
        }
        else
        {
          TryError(GetNumber(), "Нет доступа к отправляемому файлу");
        }
      }
      else
      {
        TryError(GetNumber(), "Не заданы данные для отправки файла");
      }
    }
    else
    {
      TryError(GetNumber(), "Не заданы атрибуты сетевого абонента");
    }
  }
}


void TSendPartFileThread::SendPartFile(QString _FilePathName, QString _GUID, QString _MD5, long long _FileSize,
                                       long long _StartIndex, long long _BufferSize,int _IndexPart, int _CountParts)
{
  if (GetStatus() != TStatus::WORK)
  {
    FilePathName = _FilePathName;
    FileNameGUID = _GUID;
    MD5 = _MD5;
    FileSize = _FileSize;
    StartIndex = _StartIndex;
    BufferSize = _BufferSize;
    IndexPart = _IndexPart;
    CountParts = _CountParts;
  }
}
