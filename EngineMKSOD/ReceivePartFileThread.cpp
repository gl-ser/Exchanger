//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ReceivePartFileThread.cpp                                             //
//                                                                            //
//    *** TRECEIVEPARTFILETHREAD ПОТОК ПРИЕМА КВИТАНЦИИ ИЛИ ЧАСТИ ФАЙЛА ***   //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "ReceivePartFileThread.h"


TReceivePartFileThread::TReceivePartFileThread()
{
  FileHeader.Empty();
  KvitHeader.Empty();
  Buffer.SetSize(110);
  Counter = 0;
}


TReceivePartFileThread::~TReceivePartFileThread()
{
  FileHeader.Empty();
  KvitHeader.Empty();
  Counter = 0;
}


void TReceivePartFileThread::run()
{
  //Если вызываем цикл обработки сообщений, то метод run лучше ничем другим не заполнять, т.к. под Astra Linux'ом будут глюки
  exec();
}


TReceivePartFileThread::THeaderFile TReceivePartFileThread::GetFileHeader()
{
  return FileHeader;
}


TReceivePartFileThread::THeaderKvit TReceivePartFileThread::GetKvitHeader()
{
  return KvitHeader;
}


void TReceivePartFileThread::OnRead()
{
int q;
bool ToSave, pr;
QByteArray Buf, Array = ReadData();
  pr = false;
  if ((FileHeader.IsEmpty == true) && (KvitHeader.IsEmpty == true))
  {  //Если заголовки файла и квитанции еще не считаны
    q = Buffer.PushBack(Array);
    if (Buffer.IsFull() == true)
    {
      Buf = Buffer.Data();


      if (QString(Buf.mid(0, 12)) == KvitHeader.KeyWord)
      {  //Разбор заголовка квитанции
        QByteArray Data;

        KvitHeader.GUID = QString(Buf.mid(12, 38));

        Data = Buf.mid(50, 4);
        KvitHeader.Type = ((static_cast<int>(Data[3]) & 0xFF) << 24) + ((static_cast<int>(Data[2]) & 0xFF) << 16) +
                          ((static_cast<int>(Data[1]) & 0xFF) << 8) + (static_cast<int>(Data[0]) & 0xFF);

        Data = Buf.mid(54, 4);
        KvitHeader.Err = ((static_cast<int>(Data[3]) & 0xFF) << 24) + ((static_cast<int>(Data[2]) & 0xFF) << 16) +
                         ((static_cast<int>(Data[1]) & 0xFF) << 8) + (static_cast<int>(Data[0]) & 0xFF);

        Data = Buf.mid(58, 4);
        KvitHeader.AnswerPortTCP = ((static_cast<int>(Data[3]) & 0xFF) << 24) + ((static_cast<int>(Data[2]) & 0xFF) << 16) +
                                   ((static_cast<int>(Data[1]) & 0xFF) << 8) + (static_cast<int>(Data[0]) & 0xFF);

        KvitHeader.IsEmpty = false;
        Data.clear();
      }


      if (QString(Buf.mid(0, 12)) == FileHeader.KeyWord)
      {  //Разбор заголовка файла
        QByteArray Data;

        FileHeader.GUID = QString(Buf.mid(12, 38));

        FileHeader.MD5 =  QString(Buf.mid(50, 32));

        Data = Buf.mid(82, 8);
        FileHeader.FileSize = ((static_cast<long long>(Data[7]) & 0xFF) << 56) + ((static_cast<long long>(Data[6]) & 0xFF) << 48) +
                              ((static_cast<long long>(Data[5]) & 0xFF) << 40) + ((static_cast<long long>(Data[4]) & 0xFF) << 32) +
                              ((static_cast<long long>(Data[3]) & 0xFF) << 24) + ((static_cast<long long>(Data[2]) & 0xFF) << 16) +
                              ((static_cast<long long>(Data[1]) & 0xFF) << 8) + (static_cast<long long>(Data[0]) & 0xFF);

        Data = Buf.mid(90, 8);
        FileHeader.StartIndex = ((static_cast<long long>(Data[7]) & 0xFF) << 56) + ((static_cast<long long>(Data[6]) & 0xFF) << 48) +
                                ((static_cast<long long>(Data[5]) & 0xFF) << 40) + ((static_cast<long long>(Data[4]) & 0xFF) << 32) +
                                ((static_cast<long long>(Data[3]) & 0xFF) << 24) + ((static_cast<long long>(Data[2]) & 0xFF) << 16) +
                                ((static_cast<long long>(Data[1]) & 0xFF) << 8) + (static_cast<long long>(Data[0]) & 0xFF);

        Data = Buf.mid(98, 4);
        FileHeader.IndexPart = ((static_cast<int>(Data[3]) & 0xFF) << 24) + ((static_cast<int>(Data[2]) & 0xFF) << 16) +
                               ((static_cast<int>(Data[1]) & 0xFF) << 8) + (static_cast<int>(Data[0]) & 0xFF);

        Data = Buf.mid(102, 4);
        FileHeader.CountParts = ((static_cast<int>(Data[3]) & 0xFF) << 24) + ((static_cast<int>(Data[2]) & 0xFF) << 16) +
                                ((static_cast<int>(Data[1]) & 0xFF) << 8) + (static_cast<int>(Data[0]) & 0xFF);

        Data = Buf.mid(106, 4);
        FileHeader.AnswerPortTCP = ((static_cast<int>(Data[3]) & 0xFF) << 24) + ((static_cast<int>(Data[2]) & 0xFF) << 16) +
                                   ((static_cast<int>(Data[1]) & 0xFF) << 8) + (static_cast<int>(Data[0]) & 0xFF);

        FileHeader.FilePathName = TempCatalog + "/" + FileHeader.GUID;

        FileHeader.IsEmpty = false;
        Data.clear();

        if (TFile::FileExist(FileHeader.FilePathName) == false)
        {
          QMutex mutex;
          mutex.lock();
          ToSave = TFile::CreateEmptyFile(FileHeader.FilePathName, FileHeader.FileSize);
          mutex.unlock();

          if (ToSave == false)
          {
            TryError(GetNumber(), "Ошибка создания временного файла");
          }
          else
          {
            if (q >= Array.size())
              Data.clear();
            else
              Data = Array.mid(q);

            ToSave = TFile::SavePartFile(FileHeader.FilePathName, Data, FileHeader.StartIndex);
            FileHeader.StartIndex = FileHeader.StartIndex + Data.count();
            Data.clear();

            if (ToSave == false)
              TryError(GetNumber(), "Ошибка № 1 записи в файл");
          }
        }
        else
        {
          if (q >= Array.size())
            Data.clear();
          else
            Data = Array.mid(q);

          ToSave = TFile::SavePartFile(FileHeader.FilePathName, Data, FileHeader.StartIndex);
          FileHeader.StartIndex = FileHeader.StartIndex + Data.count();
          Data.clear();

          if (ToSave == false)
            TryError(GetNumber(), "Ошибка № 2 записи в файл");
        }
      }


    }
    pr = true;
  }


  if ((FileHeader.IsEmpty == false) && (pr == false))
  {  //Если заголовок файла уже считан и заполнен
    ToSave = TFile::SavePartFile(FileHeader.FilePathName, Array, FileHeader.StartIndex);
    FileHeader.StartIndex = FileHeader.StartIndex + Array.count();

    if (ToSave == false)
      TryError(GetNumber(), "Ошибка № 3 записи в файл");
  }


  Array.clear();
  Buf.clear();
}


void TReceivePartFileThread::OnDisconnect()
{
  TNetReceiveThread::OnDisconnect();
}


void TReceivePartFileThread::HeaderFile::Empty()
{
  IsEmpty = true;
  KeyWord = "<MKSOD_FILE>";
  GUID = "";
  MD5 = "";
  FileSize = 0LL;
  StartIndex = 0LL;
  IndexPart = 0;
  CountParts = 0;
  AnswerPortTCP = 0;
  FilePathName = "";
}


void TReceivePartFileThread::HeaderKvit::Empty()
{
  IsEmpty = true;
  KeyWord = "<MKSOD_KVIT>";
  GUID = "";
  Type = 0;
  Err = 0;
  AnswerPortTCP = 0;
}
