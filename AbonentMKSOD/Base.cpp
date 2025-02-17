//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Base.cpp                                                              //
//                                                                            //
//               *** TBASE ГЛАВНЫЙ МОДУЛЬ ДАННЫХ ПРИЛОЖЕНИЯ ***               //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "Base.h"
#include "MainWin.h"


TBase::TBase(void* Owner)
{
bool pr1;
QString FilePath, HomeCatalog, str;
QStringList ParamStr;     //Командная строка приложения
QString ComputerNetName;  //Имя компьютера
QSettings *ConfigFile;    //INI-файл настроек приложения
QFile FileDB;

  OWNER = Owner;

  NAME = "AbonentMKSOD";
  ReNAME = "ReStartMKSOD";

  IsReboot = false;

  TCP_port = 5901;

  IntervalIteration = 3;

  Result = true;
  pr1 = false;

  CheckWin = false;

  //Запоминаю имя компьютера
  ComputerNetName = QHostInfo::localHostName();

  //Запоминаю путь к имени файла приложения
  FilePath = qApp->applicationDirPath()+'/';

  //Главный INI-файл приложения. Запоминаю его имя и путь к нему
  IniFileName = FilePath.toStdString() + NAME + ".ini";

  //>>> Инициализация INI-файла >>>
  ConfigFile = new QSettings(QString::fromStdString(IniFileName), QSettings::IniFormat);

  //Каталог, в котором находится приложение (имя компьютера + путь)
  HomeCatalog = ComputerNetName+"::"+FilePath;

  if (ConfigFile->value("START/Dir", "").toString() != HomeCatalog)
  {  //Если приложение было скопировано в другой каталог или на другой компьютер
    ConfigFile->remove("");  //Очистка всего INI-файла
    ConfigFile->setValue("START/Dir", HomeCatalog);
    _GUID_ = QUuid::createUuid().toString();  //Генерация нового GUID'а
    ConfigFile->setValue("START/GUID", _GUID_);

    ConfigFile->setValue("USER/InCatalog", "");
    ConfigFile->setValue("USER/OutCatalog", "");
    ConfigFile->setValue("USER/TempCatalog", "");
    ConfigFile->setValue("USER/TCP_Port", "5901");
    ConfigFile->setValue("USER/IntervalIteration", "3");
    ConfigFile->setValue("USER/CheckWin", "0");

    //!!! ЗДЕСЬ ПИШЕМ !!!

    pr1 = true;  //Признак первого запуска приложения
  }
  else
  {  //Если это не первый запуск приложения
    _GUID_ = ConfigFile->value("START/GUID", "").toString();
    if (_GUID_ == "")
    {
      //Генерация GUID'а заново, т.к. соответствующая секция INI-файла почему-то испорчена
      _GUID_ = QUuid::createUuid().toString();
      ConfigFile->setValue("START/GUID", _GUID_);
    }

    if (ConfigFile->value("USER/InCatalog", "").toString() != "")
    {
      InCatalog = ConfigFile->value("USER/InCatalog", "").toString();
    }
    else
    {
      ConfigFile->setValue("USER/InCatalog", "");
    }

    if (ConfigFile->value("USER/OutCatalog", "").toString() != "")
    {
      OutCatalog = ConfigFile->value("USER/OutCatalog", "").toString();
    }
    else
    {
      ConfigFile->setValue("USER/OutCatalog", "");
    }

    if (ConfigFile->value("USER/TempCatalog", "").toString() != "")
    {
      TempCatalog = ConfigFile->value("USER/TempCatalog", "").toString();
    }
    else
    {
      ConfigFile->setValue("USER/TempCatalog", "");
    }

    if (ConfigFile->value("USER/TCP_Port", "").toString().toInt() > 0)
    {
      TCP_port = ConfigFile->value("USER/TCP_Port", "").toString().toInt();
    }
    else
    {
      ConfigFile->setValue("USER/TCP_Port", "5901");
    }

    if (ConfigFile->value("USER/IntervalIteration", "").toString().toInt() >= 1)
    {
      IntervalIteration = ConfigFile->value("USER/IntervalIteration", "").toString().toInt();
    }
    else
    {
      ConfigFile->setValue("USER/IntervalIteration", "3");
    }

    if (ConfigFile->value("USER/CheckWin", "").toString().toInt() > 0)
    {
      if (ConfigFile->value("USER/CheckWin", "").toString().toInt() == 0)
        CheckWin = false;
      else
        CheckWin = true;
    }
    else
    {
      ConfigFile->setValue("USER/CheckWin", "0");
    }

    //!!! ЗДЕСЬ ПИШЕМ !!!

  }
  delete ConfigFile;
  QFile::setPermissions(QString::fromStdString(IniFileName), QFile::ReadOwner | QFile::WriteOwner |
                                                             QFile::ReadGroup | QFile::WriteGroup |
                                                             QFile::ReadOther | QFile::WriteOther);
  //<<< Инициализация INI-файла <<<

  Error = new TError(static_cast<QWidget*>(OWNER));
  About = new TAbout(static_cast<QWidget*>(OWNER));
  CancelOK = new TCancelOK(static_cast<QWidget*>(OWNER));
  Wait = new TWait(static_cast<QWidget*>(OWNER));
  WinFind = new TWinFind(static_cast<QWidget*>(OWNER));
  Tuner = new TWinTuner(static_cast<QWidget*>(OWNER));
  Tuner->OWNER = this;
  Log = new TWinLogBook(static_cast<QWidget*>(OWNER));
  Log->OWNER = this;
  ListAbonent = new TWinListAbonent(static_cast<QWidget*>(OWNER));
  ListAbonent->OWNER = this;
  AddAbonent = new TWinAddAbonent(static_cast<QWidget*>(OWNER));
  AddAbonent->OWNER = this;
  EditAbonent = new TWinEditAbonent(static_cast<QWidget*>(OWNER));
  EditAbonent->OWNER = this;
  CtrlOUTCat = new TCtrlOUTCat(this);


  (static_cast<TMainWin*>(OWNER))->IniFileName(IniFileName);
  Error->IniFileName(IniFileName);
  About->IniFileName(IniFileName);
  CancelOK->IniFileName(IniFileName);
  Wait->IniFileName(IniFileName);
  WinFind->IniFileName(IniFileName);
  Tuner->IniFileName(IniFileName);
  Log->IniFileName(IniFileName);
  ListAbonent->IniFileName(IniFileName);
  AddAbonent->IniFileName(IniFileName);
  EditAbonent->IniFileName(IniFileName);

  if (Result == false)
    Error->MessageRed("Нет прав для создания временного каталога");

  if (Result == true)
  if (pr1 == true)
  {
    //Удалить старую версию файла локальной БД
    FileDB.setFileName(qApp->applicationDirPath() + "/" + QString::fromStdString(NAME) + ".db");
    FileDB.remove();

    Error->MessageGreen("Поздравляю Вас с первым запуском \"Абонента МКСОД\"");

    //!!! ЗДЕСЬ ПИШЕМ !!!
  }


  //Именно здесь надо создавать объект, т.к. ранее его должен быть создан объект Error и
  // произведена проверка на первый запуск приложения
  SQLiteDB = new TSQLiteDB(this);

  //Именно здесь надо создавать объект, т.к. ранее его должны быть созданы объекты Error и Log
  Server = new TReceiveFolderServer();

  if (Result == true)
  {
    str = QString::fromStdString(ReNAME);

    #ifndef __unix__
    str = str + ".exe";
    #endif

    if (QFile::exists(FilePath + str) == false)
    {
      Error->MessageRed("Отсутствует утилита " + ReNAME);
      Result = false;
    }
  }


#ifndef __unix__
  //Этот вызов необходим для корректной инициализации "шрифтового менеджера" под ОС Windows
  Wait->show();
  QTimer::singleShot(1000, Wait, SLOT(close()));
#endif


  //В случае первого запуска приложения отложенно (по таймеру) открывается окно настроек приложения
  if (Result == true)
  if (pr1 == true)
  {
    (static_cast<TMainWin*>(OWNER))->FirstStart();
  }

  (static_cast<TMainWin*>(OWNER))->setWindowTitle("Абонент МКСОД");
  Log->setWindowTitle("Протокол Абонента МКСОД");
  ListAbonent->setWindowTitle("Управление списком абонентов МКСОД");

  if (CheckWin == true)
  {
    Log->show();
    ListAbonent->show();
  }

  ListAbonent->RestoreCatalogOfAbonent();

  StartServer();

  CtrlOUTCat->StartCtrl();

  connect(Server, SIGNAL(ErrorServer(QString)), this, SLOT(OnErrorServer(QString)), Qt::QueuedConnection);
  connect(Server, SIGNAL(MessageServer(QString)), this, SLOT(OnMessageServer(QString)), Qt::QueuedConnection);
  connect(Server, SIGNAL(GotKvitError(TReceiveFolderServer::TAbonentExt, QString, int)), this, SLOT(OnGotKvitError(TReceiveFolderServer::TAbonentExt, QString, int)), Qt::QueuedConnection);
  connect(Server, SIGNAL(GotKvitSuccess(TReceiveFolderServer::TAbonentExt, QString)), this, SLOT(OnGotKvitSuccess(TReceiveFolderServer::TAbonentExt, QString)), Qt::QueuedConnection);
}


TBase::~TBase(void)
{
QString FilePath, LogFileName;

  CtrlOUTCat->StopCtrl();
  StopServer();

  //Запоминаю путь к имени файла приложения
  FilePath = qApp->applicationDirPath() + "/";

  //Главный LOG-файл приложения
  LogFileName = FilePath + QString::fromStdString(NAME) + ".txt";

  Log->TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Приложение остановлено");

  //Сохранить журнал в главном LOG-файле приложения
  Log->SaveToFile(LogFileName, false);

  TCP_port = 0;
  IntervalIteration = 0;

  delete About;
  delete Error;
  delete CancelOK;
  delete Wait;
  delete WinFind;
  delete Tuner;
  delete Log;
  delete ListAbonent;
  delete AddAbonent;
  delete EditAbonent;
  delete Server;
  delete CtrlOUTCat;

  IniFileName = "";
  Result = false;

  NAME = "";
  ReNAME = "";

  OWNER = NULL;
  IsReboot = false;

  delete SQLiteDB;
}


void TBase::HowCalc(qint64 Index, qint64 Quantity)
{
int _Index;
bool pr;
  _Index = static_cast<int>((static_cast<double>(Index) / static_cast<double>(Quantity)) * 100.0);
  pr = false;

  switch (_Index)
  {
    case 0:
      pr = true;
    break;

    case 10:
      pr = true;
    break;

    case 20:
      pr = true;
    break;

    case 30:
      pr = true;
    break;

    case 40:
      pr = true;
    break;

    case 50:
      pr = true;
    break;

    case 60:
      pr = true;
    break;

    case 70:
      pr = true;
    break;

    case 80:
      pr = true;
    break;

    case 90:
      pr = true;
    break;

    case 100:
      pr = true;
    break;
  }

  if (pr == true)
  {
    qApp->processEvents();
    //(static_cast<TMainWin*>(OWNER))->progressBar->setMinimum(0);
    //(static_cast<TMainWin*>(OWNER))->progressBar->setMaximum(100);
    //(static_cast<TMainWin*>(OWNER))->progressBar->setValue(_Index);
    qApp->processEvents();
  }
}


std::string TBase::RandomString(int PLen)
{
std::string str1, Result;
std::mt19937 gen(static_cast<unsigned>(time(NULL)));  //Создаю генератор с произвольным начальным состоянием
std::uniform_int_distribution<> Distr(0, 25);         //Создаю равномерное целочисленное распределение

  str1 = "abcdefghijklmnopqrstuvwxyz";  //Строка "алфавита", из символов которого будет сгенерирована результирующая строка
  Result = "";
  do
    Result = Result + str1[static_cast<unsigned>(Distr(gen))];
  while (Result.size() < static_cast<unsigned>(PLen));

  return Result;
}


void TBase::OnErrorServer(QString Str)
{
  AddToLogBook("ОШИБКА: <<< " + Str + " >>>");
}


void TBase::OnMessageServer(QString Str)
{
  AddToLogBook(Str);
}


void TBase::OnGotKvitError(TReceiveFolderServer::TAbonentExt Abonent, QString FileName, int Err)
{
  Q_UNUSED(Abonent);
  Q_UNUSED(FileName);
  Q_UNUSED(Err);
  //AddToLogBook("!!! Сработала заглушка OnGotKvitError !!!");
}


void TBase::OnGotKvitSuccess(TReceiveFolderServer::TAbonentExt Abonent, QString FileName)
{
  Q_UNUSED(Abonent);
  Q_UNUSED(FileName);
  //AddToLogBook("!!! Сработала заглушка OnGotKvitSuccess !!!");
}


void TBase::AddToLogBook(QString Value)
{
QString FilePath, LogFileName;
  if (Log->TextProtocol->document()->lineCount() > 500)
  {
    //Запоминаю путь к имени файла приложения
    FilePath = qApp->applicationDirPath() + "/";

    //Главный LOG-файл приложения
    LogFileName = FilePath + QString::fromStdString(NAME) + ".txt";

    Log->TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Протокол переполнен");

    //Сохранить переполненный журнал в главном LOG-файле приложения
    Log->SaveToFile(LogFileName, false);

    Log->TextProtocol->document()->clear();

    Log->TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") +
                              " Содержимое протокола автоматически сохранено в главном LOG-файле приложения");

    Log->TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Протокол очищен");
  }
  Log->TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " " + Value);

  //Прокрутка текста вниз
  QTextCursor cursor = Log->TextProtocol->textCursor();
  cursor.movePosition(QTextCursor::End);
  Log->TextProtocol->setTextCursor(cursor);
}


void TBase::StartServer()
{
  if ((TempCatalog != "") && (InCatalog != "") && (OutCatalog != ""))
  {
    Server->SetTempCatalog(TempCatalog);
    Server->SetTimeOut(1.0);
    TWinListAbonent::TAbonents Abon = ListAbonent->GetAbonents();
    for(unsigned int i=0; i<Abon.size(); i++)
      Server->AddAbonent(Abon[i].name, Abon[i].address, Abon[i].port, InCatalog + "/" + Abon[i].name);
    Abon.clear();
    bool res = Server->Start(TCP_port);
    if (res == true)
    {
      (static_cast<TMainWin*>(OWNER))->Set_Port(" Порт: " + QString::number(TCP_port) + " ");
      AddToLogBook("TCP-сервер успешно запущен");
      AddToLogBook("Открыт TCP-порт № " + QString::number(Server->GetPortTCP()));
    }
    else
    {
      (static_cast<TMainWin*>(OWNER))->Set_Port(" Порт: нет подключения ");
      AddToLogBook("Ошибка запуска TCP-сервера");
    }
  }
}


void TBase::StopServer()
{
  Server->Stop();
  (static_cast<TMainWin*>(OWNER))->Set_Port(" Порт: нет подключения ");
  AddToLogBook("TCP-сервер остановлен");
}
