//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ReceiveFolderServer.cpp                                               //
//                                                                            //
//            *** TRECEIVEFOLDERSERVER СЕРВЕР ПРИЕМА КАТАЛОГА, ***            //
//                   *** ОБРАБОТКИ И ОТПРАВКИ КВИТАНЦИИ ***                   //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "ReceiveFolderServer.h"


TReceiveFolderServer::TReceiveFolderServer()
{
  //Чтобы мой тип TReceiveFolderServer::TAbonentExt использовать в сигналах\слотах, я его регистрирую
  qRegisterMetaType<TReceiveFolderServer::TAbonentExt>("TReceiveFolderServer::TAbonentExt");

  Abonents.clear();
  Files.clear();
  ClearZIPs();
  ClearKvits();
  TimeOut = 5.0;
  CurTimer = new QBasicTimer();
}


TReceiveFolderServer::~TReceiveFolderServer()
{
  Abonents.clear();
  Files.clear();
  ClearZIPs();
  ClearKvits();
  CurTimer->stop();
  TimeOut = 0;
  delete CurTimer;
}


bool TReceiveFolderServer::Start(int _PortTCP)
{
bool res;
  TFile::ClearDir(GetTempCatalog());
  res = TBasicServerTCP::Start(_PortTCP);
  if (res == true)
  {
    CurTimer->start(1000, this);
  }
  return res;
}


void TReceiveFolderServer::Stop()
{
  TBasicServerTCP::Stop();
  CurTimer->stop();
  Abonents.clear();
  Files.clear();
  ClearZIPs();
  ClearKvits();
  TimeOut = 5.0;
}


void TReceiveFolderServer::OnError(int Number, int Duration, QString Str)
{
  Q_UNUSED(Duration);
  ReadHeader(Number, false);
  DeleteClient(Number);
  emit ErrorServer(Str);
}


void TReceiveFolderServer::OnSuccess(int Number, int Duration)
{
  Q_UNUSED(Duration);
  ReadHeader(Number, true);
  DeleteClient(Number);
}


void TReceiveFolderServer::OnZIPError(int Number, int Duration, QString Str)
{
int PortTCP;
QString FilePathName, Address;
  Q_UNUSED(Duration);
  FilePathName = ZIPs[static_cast<unsigned int>(FindZIPs(Number))]->GetZIPFilePathName();
  Address = ZIPs[static_cast<unsigned int>(FindZIPs(Number))]->GetAddress();
  PortTCP = ZIPs[static_cast<unsigned int>(FindZIPs(Number))]->GetAnswerPortTCP();
  TFile::DelFile(FilePathName);  //Удаление файла с диска
  DeleteZIPs(Number);

  emit ErrorServer(Str);

  int index = FindAbonent(Address, PortTCP);
  if (index >= 0)
  {
    SendKvit(Abonents[static_cast<unsigned int>(index)].Base.Name, Address,
             Abonents[static_cast<unsigned int>(index)].Base.PortTCP, TFile::GetFileName(FilePathName), 0, 1);
  }
  else
  {
    emit MessageServer("Получен файл от незарегистрированного сетевого абонента.    IP-адрес: " + Address +
                       "    TCP-порт: " + QString::number(PortTCP));
  }
}


void TReceiveFolderServer::OnZIPSuccess(int Number, int Duration)
{
int PortTCP;
QString FilePathName, Address;
  Q_UNUSED(Duration);
  FilePathName = ZIPs[static_cast<unsigned int>(FindZIPs(Number))]->GetZIPFilePathName();
  Address = ZIPs[static_cast<unsigned int>(FindZIPs(Number))]->GetAddress();
  PortTCP = ZIPs[static_cast<unsigned int>(FindZIPs(Number))]->GetAnswerPortTCP();
  TFile::DelFile(FilePathName);  //Удаление файла с диска
  DeleteZIPs(Number);

  emit MessageServer("Распаковка принятого файла " + TFile::GetFileName(FilePathName) +
                     " завершена за " + QString::number(Duration) + " сек");

  int index = FindAbonent(Address, PortTCP);
  if (index >= 0)
  {
    SendKvit(Abonents[static_cast<unsigned int>(index)].Base.Name, Address,
             Abonents[static_cast<unsigned int>(index)].Base.PortTCP, TFile::GetFileName(FilePathName), 1, 0);
  }
  else
  {
    emit MessageServer("Получен файл от незарегистрированного сетевого абонента.    IP-адрес: " + Address +
                       "    TCP-порт: " + QString::number(PortTCP));
  }
}


void TReceiveFolderServer::OnKvitError(int Number, int Duration, QString Str)
{
  Q_UNUSED(Duration);
  DeleteKvits(Number);

  emit ErrorServer(Str);
}


void TReceiveFolderServer::OnKvitSuccess(int Number, int Duration)
{
QString FilePathName, Address;
  Q_UNUSED(Duration);
  DeleteKvits(Number);
}


void TReceiveFolderServer::SendKvit(QString Name, QString Address, int PortTCP, QString FileName, int Type, int Err)
{
  Kvits.push_back(new TSendKvitThread);

  Kvits[Kvits.size()-1]->SetNumber(static_cast<int>(Kvits.size()));
  Kvits[Kvits.size()-1]->SetAnswerPortTCP(GetPortTCP());
  Kvits[Kvits.size()-1]->SetAbonent(Name, Address, PortTCP);
  Kvits[Kvits.size()-1]->SendKvit(FileName, Type, Err);

  connect(Kvits[Kvits.size()-1], SIGNAL(Success(int, int)), this, SLOT(OnKvitSuccess(int, int)), Qt::QueuedConnection);
  connect(Kvits[Kvits.size()-1], SIGNAL(Error(int, int, QString)), this, SLOT(OnKvitError(int, int, QString)), Qt::QueuedConnection);
  connect(Kvits[Kvits.size()-1], SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  Kvits[Kvits.size()-1]->start();
}


void TReceiveFolderServer::CreateThreadObject()
{
  Clients.push_back(new TReceivePartFileThread());
}


void TReceiveFolderServer::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == CurTimer->timerId())
  {
    AnalyseFiles();
  }
}


void TReceiveFolderServer::ReadHeader(int Number, bool IsOk)
{
THeaderFile HF;
THeaderKvit HK;
  HF.IsOk = IsOk;
  HF.DateTime = TCalendar::QDateTimeToTDateTime(QDateTime::currentDateTime());
  HF.Address = Clients[static_cast<unsigned int>(FindClient(Number))]->GetAddress();
  HF.Header = static_cast<TReceivePartFileThread*>(Clients[static_cast<unsigned int>(FindClient(Number))])->GetFileHeader();

  HK.Address = Clients[static_cast<unsigned int>(FindClient(Number))]->GetAddress();
  HK.Header = static_cast<TReceivePartFileThread*>(Clients[static_cast<unsigned int>(FindClient(Number))])->GetKvitHeader();

  if (HF.Header.IsEmpty == false)
  {  //Из принятых TCP-пакетов удалось прочитать заголовок получаемого файла
    Files.push_back(HF);
  }

  if (HK.Header.IsEmpty == false)
  {  //Из принятых TCP-пакетов удалось прочитать заголовок квитанции
    if (HK.Header.Type == 1)
    {  //Квитанция об успешной доставке файла
      //emit MessageServer("Получена квитанция об успешной доставке файла " + HK.Header.GUID);

      int index = FindAbonent(HK.Address, HK.Header.AnswerPortTCP);
      if (index >= 0)
      {
        emit MessageServer("Получена квитанция об успешной доставке файла " + HK.Header.GUID + " абоненту    Имя: " +
                           Abonents[static_cast<unsigned int>(index)].Base.Name +
                           "    IP-адрес: " + Abonents[static_cast<unsigned int>(index)].Base.Address +
                           "    TCP-порт: " + QString::number(Abonents[static_cast<unsigned int>(index)].Base.PortTCP));

        emit GotKvitSuccess(Abonents[static_cast<unsigned int>(index)], HK.Header.GUID);
      }
      else
      {
        emit MessageServer("Получена квитанция от незарегистрированного сетевого абонента.    IP-адрес: " + HK.Address +
                           "    TCP-порт: " + QString::number(HK.Header.AnswerPortTCP));
      }
    }
    else
    {  //Квитанция о неуспешной доставке файла
      QString Str = "неизвестна";
      switch (HK.Header.Err)
      {
        case 1:
          Str = "ошибка распаковки ZIP-архива";
        break;

        case 2:
          Str = "тайм-аут приема файла";
        break;

        case 3:
          Str = "неверная контрольная сумма";
        break;

        case 4:
          Str = "одна или несколько частей файла были получены с ошибкой";
        break;
      }
      //emit MessageServer("Получена квитанция о неуспешной передаче файла " + HK.Header.GUID + " Причина: " + Str);

      int index = FindAbonent(HK.Address, HK.Header.AnswerPortTCP);
      if (index >= 0)
      {
        emit MessageServer("Получена квитанция о неуспешной доставке файла " + HK.Header.GUID + " абоненту    Имя: " +
                           Abonents[static_cast<unsigned int>(index)].Base.Name +
                           "    IP-адрес: " + Abonents[static_cast<unsigned int>(index)].Base.Address +
                           "    TCP-порт: " + QString::number(Abonents[static_cast<unsigned int>(index)].Base.PortTCP) +
                           "    Причина: " + Str);

        emit GotKvitError(Abonents[static_cast<unsigned int>(index)], HK.Header.GUID, HK.Header.Err);
      }
      else
      {
        emit MessageServer("Получена квитанция от незарегистрированного сетевого абонента.    IP-адрес: " + HK.Address +
                           "    TCP-порт: " + QString::number(HK.Header.AnswerPortTCP));
      }
    }
  }
}


void TReceiveFolderServer::AnalyseFiles()
{
bool pr;
unsigned int i;
double DateTime;
QString MD5, Str;
  DateTime = TCalendar::QDateTimeToTDateTime(QDateTime::currentDateTime());
beginn:

  //>>> Если клиентская прога отвалилась, а сокет не смог определить сего факта, тогда сработает эта проверка >>>
  if (Files.size() <= 0)
  for(unsigned int j=0; j<Clients.size(); j++)
  if ((dynamic_cast<TReceivePartFileThread*>(Clients[j]))->GetFileHeader().IsEmpty == true &&
      (dynamic_cast<TReceivePartFileThread*>(Clients[j]))->GetKvitHeader().IsEmpty == true)
  {
    if ((dynamic_cast<TReceivePartFileThread*>(Clients[j]))->Counter < 3)
    {
      (dynamic_cast<TReceivePartFileThread*>(Clients[j]))->Counter++;
    }

    if ((dynamic_cast<TReceivePartFileThread*>(Clients[j]))->Counter >= 3)
    {
      //emit MessageServer("Сработала защита");
      DeleteClient(Clients[j]->GetNumber());
      goto beginn;
    }
  }
  //<<< Если клиентская прога отвалилась, а сокет не смог определить сего факта, тогда сработает эта проверка <<<

  pr = false;
  for(i=0; i<Files.size(); i++)
  {
    if ((DateTime - Files[i].DateTime) > TimeOut/60.0/24.0)
    {  //Слишком долго "висит" файл на получении => это ошибка
      //Str = "Тайм-аут приема файла " + Files[i].Header.GUID;
      //emit ErrorServer(Str);
      int index = FindAbonent(Files[i].Address, Files[i].Header.AnswerPortTCP);
      if (index >= 0)
      {
        Str = "Тайм-аут приема файла " + Files[i].Header.GUID + " от сетевого абонента    Имя: " +
              Abonents[static_cast<unsigned int>(index)].Base.Name +
              "    IP-адрес: " + Abonents[static_cast<unsigned int>(index)].Base.Address +
              "    TCP-порт: " + QString::number(Abonents[static_cast<unsigned int>(index)].Base.PortTCP);
        emit ErrorServer(Str);

        SendKvit(Abonents[static_cast<unsigned int>(index)].Base.Name, Files[i].Address,
                 Abonents[static_cast<unsigned int>(index)].Base.PortTCP, Files[i].Header.GUID, 0, 2);
      }
      else
      {
        emit MessageServer("Получен файл от незарегистрированного сетевого абонента.    IP-адрес: " + Files[i].Address +
                           "    TCP-порт: " + QString::number(Files[i].Header.AnswerPortTCP));
      }

      TFile::DelFile(GetTempCatalog() + "/" + Files[i].Header.GUID);  //Удаление файла с диска
      DeleteFromFiles(Files[i].Header.GUID);
      pr = true;
      break;
    }

    if (Files[i].IsOk == true)
    if (CountParts(Files[i].Header.GUID) >= Files[i].Header.CountParts)
    {  //Все части файла успешно приняты
      MD5 = TCheckSum::FileMD5(GetTempCatalog() + "/" + Files[i].Header.GUID);
      if (MD5 == Files[i].Header.MD5)
      {  //Контрольная сумма верная, необходимо распаковать архив
        int index = FindAbonent(Files[i].Address, Files[i].Header.AnswerPortTCP);
        if (index >= 0)
        {
          emit MessageServer("От сетевого абонента    Имя: " + Abonents[static_cast<unsigned int>(index)].Base.Name +
                             "    IP-адрес: " + Abonents[static_cast<unsigned int>(index)].Base.Address +
                             "    TCP-порт: " + QString::number(Abonents[static_cast<unsigned int>(index)].Base.PortTCP) +
                             "    получен файл " + Files[i].Header.GUID + " Начата его распаковка");
          //emit MessageServer("Начата распаковка принятого файла " + Files[i].Header.GUID);

          ZIPs.push_back(new TZIPThread2);

          ZIPs[ZIPs.size()-1]->SetNumber(static_cast<int>(ZIPs.size()));
          ZIPs[ZIPs.size()-1]->SetAddress(Files[i].Address);
          ZIPs[ZIPs.size()-1]->SetAnswerPortTCP(Files[i].Header.AnswerPortTCP);
          ZIPs[ZIPs.size()-1]->SetPaths(Abonents[static_cast<unsigned int>(index)].FolderPathName, GetTempCatalog() + "/" + Files[i].Header.GUID);
          ZIPs[ZIPs.size()-1]->ExtractDir();

          connect(ZIPs[ZIPs.size()-1], SIGNAL(Success(int, int)), this, SLOT(OnZIPSuccess(int, int)), Qt::QueuedConnection);
          connect(ZIPs[ZIPs.size()-1], SIGNAL(Error(int, int, QString)), this, SLOT(OnZIPError(int, int, QString)), Qt::QueuedConnection);
          connect(ZIPs[ZIPs.size()-1], SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

          ZIPs[ZIPs.size()-1]->start();

          pr = true;
        }
        else
        {
          emit MessageServer("Получен файл от незарегистрированного сетевого абонента.    IP-адрес: " + Files[i].Address +
                             "    TCP-порт: " + QString::number(Files[i].Header.AnswerPortTCP));
          DeleteFromFiles(Files[i].Header.GUID);
          break;
        }

        if (pr == true)
        {
          DeleteFromFiles(Files[i].Header.GUID);
          break;
        }
      }
      else
      {  //Ошибка контрольной суммы
        //Str = "Неверная контрольная сумма принятого файла " + Files[i].Header.GUID;
        //emit ErrorServer(Str);
        int index = FindAbonent(Files[i].Address, Files[i].Header.AnswerPortTCP);
        if (index >= 0)
        {
          Str = "Неверная контрольная сумма принятого файла " + Files[i].Header.GUID + " от сетевого абонента    Имя: " +
              Abonents[static_cast<unsigned int>(index)].Base.Name +
              "    IP-адрес: " + Abonents[static_cast<unsigned int>(index)].Base.Address +
              "    TCP-порт: " + QString::number(Abonents[static_cast<unsigned int>(index)].Base.PortTCP);
          emit ErrorServer(Str);

          SendKvit(Abonents[static_cast<unsigned int>(index)].Base.Name, Files[i].Address,
                   Abonents[static_cast<unsigned int>(index)].Base.PortTCP, Files[i].Header.GUID, 0, 3);
        }
        else
        {
          emit MessageServer("Получен файл от незарегистрированного сетевого абонента.    IP-адрес: " + Files[i].Address +
                             "    TCP-порт: " + QString::number(Files[i].Header.AnswerPortTCP));
        }

        TFile::DelFile(GetTempCatalog() + "/" + Files[i].Header.GUID);  //Удаление файла с диска
        DeleteFromFiles(Files[i].Header.GUID);
        pr = true;
        break;
      }
    }

    if (Files[i].IsOk == false)
    if (CountPartsAll(Files[i].Header.GUID) >= Files[i].Header.CountParts)
    {  //Одна или несколько частей файла были получены с ошибкой
      //Str = "Некоторые части файла " + Files[i].Header.GUID + " были получены с ошибкой";
      //emit ErrorServer(Str);
      int index = FindAbonent(Files[i].Address, Files[i].Header.AnswerPortTCP);
      if (index >= 0)
      {
        Str = "От сетевого абонента    Имя: " + Abonents[static_cast<unsigned int>(index)].Base.Name +
              "    IP-адрес: " + Abonents[static_cast<unsigned int>(index)].Base.Address +
              "    TCP-порт: " + QString::number(Abonents[static_cast<unsigned int>(index)].Base.PortTCP) +
              "    некоторые части файла " + Files[i].Header.GUID + " были получены с ошибкой";
        emit ErrorServer(Str);

        SendKvit(Abonents[static_cast<unsigned int>(index)].Base.Name, Files[i].Address,
                 Abonents[static_cast<unsigned int>(index)].Base.PortTCP, Files[i].Header.GUID, 0, 4);
      }
      else
      {
        emit MessageServer("Получен файл от незарегистрированного сетевого абонента.    IP-адрес: " + Files[i].Address +
                           "    TCP-порт: " + QString::number(Files[i].Header.AnswerPortTCP));
      }

      TFile::DelFile(GetTempCatalog() + "/" + Files[i].Header.GUID);  //Удаление файла с диска
      DeleteFromFiles(Files[i].Header.GUID);
      pr = true;
      break;
    }
  }
  if (pr == true)
    goto beginn;
}


void TReceiveFolderServer::DeleteFromFiles(QString GUID)
{
unsigned int i;
std::vector<THeaderFile> Temp;
  Temp.clear();
  for(i=0; i<Files.size(); i++)
  if (Files[i].Header.GUID != GUID)
    Temp.push_back(Files[i]);

  Files.clear();
  for(i=0; i<Temp.size(); i++)
    Files.push_back(Temp[i]);

  Temp.clear();
}


int TReceiveFolderServer::CountParts(QString GUID)
{
int index;
unsigned int i;
  index = 0;
  for(i=0; i<Files.size(); i++)
  if ((Files[i].Header.GUID == GUID) && (Files[i].IsOk == true))
    index = index + 1;

  return index;
}


int TReceiveFolderServer::CountPartsAll(QString GUID)
{
int index;
unsigned int i;
  index = 0;
  for(i=0; i<Files.size(); i++)
  if (Files[i].Header.GUID == GUID)
    index = index + 1;

  return index;
}


void TReceiveFolderServer::AddAbonent(QString _Name, QString _Address, int _PortTCP, QString _FolderPathName)
{
TAbonentExt Abonent;
  Abonent.Base.Name = _Name;
  Abonent.Base.Address = _Address;
  Abonent.Base.PortTCP = _PortTCP;
  Abonent.FolderPathName = _FolderPathName;
  Abonents.push_back(Abonent);
}


void TReceiveFolderServer::SetTimeOut(double Value)
{
  TimeOut = Value;
}


void TReceiveFolderServer::ClearZIPs()
{
unsigned int i;
  for(i=0; i<ZIPs.size(); i++)
  if (ZIPs[i] != NULL)
  {
    ZIPs[i]->quit();
    ZIPs[i]->wait();
    delete ZIPs[i];
    ZIPs[i] = NULL;
  }
  ZIPs.clear();
}


void TReceiveFolderServer::DeleteZIPs(int Number)
{
int index1;
unsigned int i, index2;
std::vector<TZIPThread2*> Temp;

  index1 = FindZIPs(Number);
  if (index1 != -1)
  {
    index2 = static_cast<unsigned int>(index1);
    ZIPs[index2]->quit();
    ZIPs[index2]->wait();
    delete ZIPs[index2];

    Temp.clear();

    for(i=0; i<ZIPs.size(); i++)
    if (i != index2)
      Temp.push_back(ZIPs[i]);

    ZIPs.clear();

    for(i=0; i<Temp.size(); i++)
      ZIPs.push_back(Temp[i]);

    Temp.clear();
  }
}


int TReceiveFolderServer::FindZIPs(int Number)
{
int res;
unsigned int i;
  res = -1;
  for(i=0; i<ZIPs.size(); i++)
  if (ZIPs[i]->GetNumber() == Number)
  {
    res = static_cast<int>(i);
    break;
  }
  return res;
}


void TReceiveFolderServer::ClearKvits()
{
unsigned int i;
  for(i=0; i<Kvits.size(); i++)
  if (Kvits[i] != NULL)
  {
    Kvits[i]->quit();
    Kvits[i]->wait();
    delete Kvits[i];
    Kvits[i] = NULL;
  }
  Kvits.clear();
}


void TReceiveFolderServer::DeleteKvits(int Number)
{
int index1;
unsigned int i, index2;
std::vector<TSendKvitThread*> Temp;

  index1 = FindKvits(Number);
  if (index1 != -1)
  {
    index2 = static_cast<unsigned int>(index1);
    Kvits[index2]->quit();
    Kvits[index2]->wait();
    delete Kvits[index2];

    Temp.clear();

    for(i=0; i<Kvits.size(); i++)
    if (i != index2)
      Temp.push_back(Kvits[i]);

    Kvits.clear();

    for(i=0; i<Temp.size(); i++)
      Kvits.push_back(Temp[i]);

    Temp.clear();
  }
}


int TReceiveFolderServer::FindKvits(int Number)
{
int res;
unsigned int i;
  res = -1;
  for(i=0; i<Kvits.size(); i++)
  if (Kvits[i]->GetNumber() == Number)
  {
    res = static_cast<int>(i);
    break;
  }
  return res;
}


int TReceiveFolderServer::FindAbonent(QString _Address, int _AnswerPortTCP)
{
int res;
unsigned int i;
  res = -1;
  for(i=0; i<Abonents.size(); i++)
  if ((Abonents[i].Base.Address == _Address) && (Abonents[i].Base.PortTCP == _AnswerPortTCP))
  {
    res = static_cast<int>(i);
    break;
  }
  return res;
}


TZIPThread2::TZIPThread2()
{
  Address = "";
  AnswerPortTCP = 0;
}


TZIPThread2::~TZIPThread2()
{
  Address = "";
  AnswerPortTCP = 0;
}


void TZIPThread2::SetAddress(QString _Address)
{
  Address = _Address;
}


QString TZIPThread2::GetAddress()
{
  return Address;
}


void TZIPThread2::SetAnswerPortTCP(int _AnswerPortTCP)
{
  AnswerPortTCP = _AnswerPortTCP;
}


int TZIPThread2::GetAnswerPortTCP()
{
  return AnswerPortTCP;
}
