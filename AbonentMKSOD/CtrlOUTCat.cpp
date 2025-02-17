//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл CtrlOUTCat.cpp                                                        //
//                                                                            //
//                  *** TCTRLOUTCAT КОНТРОЛЬ КАТАЛОГА OUT ***                 //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "CtrlOUTCat.h"
#include "Base.h"


TCtrlOUTCat::TCtrlOUTCat(void* Owner)
{
  OWNER = Owner;

  TimerOUT = new QBasicTimer();
  StopCtrl();
}


TCtrlOUTCat::~TCtrlOUTCat(void)
{
  StopCtrl();

  OWNER = NULL;

  delete TimerOUT;
}


void TCtrlOUTCat::StartCtrl()
{
  Abon.clear();
  Abon = (static_cast<TBase*>(OWNER))->ListAbonent->GetAbonents();

  SendFolders.clear();
  SendFolders.resize(Abon.size());

  for(unsigned int i=0; i<Abon.size(); i++)
  {
    SendFolders[i] = new TSendFolderThread();

    connect(SendFolders[i], SIGNAL(Success(int, int)), this, SLOT(OnSendFolderSuccess(int, int)), Qt::QueuedConnection);
    connect(SendFolders[i], SIGNAL(Error(int, int, QString)), this, SLOT(OnSendFolderError(int, int, QString)), Qt::QueuedConnection);
    connect(SendFolders[i], SIGNAL(Message(int, QString)), this, SLOT(OnSendFolderMessage(int, QString)), Qt::QueuedConnection);

    SendFolders[i]->SetNumber(Abon[i].ID);
    SendFolders[i]->SetAbonent(Abon[i].name, Abon[i].address, Abon[i].port);
    SendFolders[i]->SetAnswerPortTCP((static_cast<TBase*>(OWNER))->Server->GetPortTCP());
    SendFolders[i]->SetTempCatalog((static_cast<TBase*>(OWNER))->TempCatalog);
    SendFolders[i]->SendFolder((static_cast<TBase*>(OWNER))->OutCatalog + "/" + Abon[i].name + "_TEMP_");
  }

  StartCtrlSimple();
}


void TCtrlOUTCat::StopCtrl()
{
  StopCtrlSimple();

  //Дождаться окончания работы всех потоков
  for(unsigned int i=0; i<SendFolders.size(); i++)
  {
    SendFolders[i]->quit();
    SendFolders[i]->wait();
  }

  for(unsigned int i=0; i<SendFolders.size(); i++)
  {
    //Отсоединить все обработчики всех сигналов от объекта потока
    SendFolders[i]->disconnect();
  }

  //Удалить все объекты потоков
  for(unsigned int i=0; i<SendFolders.size(); i++)
  {
    delete SendFolders[i];
  }

  SendFolders.clear();

  Abon.clear();
}


void TCtrlOUTCat::StartCtrlSimple()
{
  TimerOUT->start(1000, this);
}


void TCtrlOUTCat::StopCtrlSimple()
{
  TimerOUT->stop();
}


void TCtrlOUTCat::OnSendFolderError(int Number, int Duration, QString Str)
{
QString Name="";
  Q_UNUSED(Duration);
  for(unsigned int i=0; i<Abon.size(); i++)
  if (Abon[i].ID == Number)
    Name = Abon[i].name;

  if (Name != "")
  {
    QString Catalog = (static_cast<TBase*>(OWNER))->OutCatalog + "/" + Name + "_TEMP_";

    //Очищаю каталог пользователя
    DelSubDirs(Catalog);
    DelFilesFromDir(Catalog);

    //Удаляю сам каталог пользователя, т.к. он временный
    DelDir(Catalog);
  }

  (static_cast<TBase*>(OWNER))->AddToLogBook(Str);
}


void TCtrlOUTCat::OnSendFolderSuccess(int Number, int Duration)
{
QString Name="";

  for(unsigned int i=0; i<Abon.size(); i++)
  if (Abon[i].ID == Number)
    Name = Abon[i].name;

  if (Name != "")
  {
    QString Catalog = (static_cast<TBase*>(OWNER))->OutCatalog + "/" + Name + "_TEMP_";

    //Очищаю каталог пользователя
    DelSubDirs(Catalog);
    DelFilesFromDir(Catalog);

    //Удаляю сам каталог пользователя, т.к. он временный
    DelDir(Catalog);
  }

  (static_cast<TBase*>(OWNER))->AddToLogBook("Общее время отправки " + QString::number(Duration) + " сек");
}


void TCtrlOUTCat::DelFilesFromDir(QString PathDir)
{
  QDir *Dir = new QDir(PathDir);

  //Получаем список файлов в каталоге
  QStringList lstFiles = Dir->entryList(QDir::Files);

  //Удаляем файлы из каталога
  foreach (QString entry, lstFiles)
  {
    QString entryAbsPath = Dir->absolutePath() + "/" + entry;
    QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
    QFile::remove(entryAbsPath);
  }
  delete Dir;
}


void TCtrlOUTCat::DelSubDirs(QString PathDir)
{
  QDir *Dir = new QDir(PathDir);

  //Получаем список подкаталогов в каталоге
  QStringList lst = Dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  foreach (QString entry, lst)
  {
    QString entryAbsPath = Dir->absolutePath() + "/" + entry;
    DelFilesFromDir(entryAbsPath);
    DelSubDirs(entryAbsPath);
    Dir->rmdir(entryAbsPath);
  }

  delete Dir;
}


void TCtrlOUTCat::DelDir(QString PathDir)
{
  QDir *Dir = new QDir(PathDir);
  Dir->rmdir(PathDir);
  delete Dir;
}


void TCtrlOUTCat::CountFilesInDir(QString PathDir, unsigned int *Quantity)
{
  QDir *Dir = new QDir(PathDir);

  //Получаем список файлов в каталоге
  *Quantity = *Quantity+static_cast<unsigned int>(Dir->entryList(QDir::Files).size());

  //Получаем список подкаталогов в каталоге
  QStringList lst = Dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  foreach (QString entry, lst)
  {
    QString entryAbsPath = Dir->absolutePath() + "/" + entry;
    CountFilesInDir(entryAbsPath, Quantity);
  }

  delete Dir;
}


void TCtrlOUTCat::ListFilesInDir(QString PathDir, QStringList *FilesList)
{
  QDir *Dir = new QDir(PathDir);

  //Получаем список файлов в каталоге
  QStringList FL = Dir->entryList(QDir::Files);

  for(int i=0; i<FL.size(); i++)
    FL[i] = Dir->absolutePath() + "/" + FL[i];

  *FilesList = *FilesList + FL;

  //Получаем список подкаталогов в каталоге
  QStringList lst = Dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  foreach (QString entry, lst)
  {
    QString entryAbsPath = Dir->absolutePath() + "/" + entry;
    ListFilesInDir(entryAbsPath, FilesList);
  }

  delete Dir;
}


void TCtrlOUTCat::OnSendFolderMessage(int Number, QString Str)
{
  Q_UNUSED(Number);
  (static_cast<TBase*>(OWNER))->AddToLogBook(Str);
}


void TCtrlOUTCat::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == TimerOUT->timerId())
  {
    StopCtrlSimple();  //StopCtrl использовать здесь не надо, чтобы исключить ежесекундное перечитывание каталога абонентов

    for(unsigned int i=0; i<SendFolders.size(); i++)
    if (SendFolders[i]->GetStatus() != TBasicThread::TStatus::WORK)
    {
      QString Catalog = (static_cast<TBase*>(OWNER))->OutCatalog + "/" + SendFolders[i]->GetAbonent().Name;
      QDir Dir = QDir(Catalog);
      QFile F;
      bool res = false;
      unsigned int Q1=0, Q2=0;
      QStringList Files;

      //Первый раз запрашиваю количество файлов в каталоге абонента
      CountFilesInDir(Catalog, &Q1);

      //Получаю список файлов (с полным путем) в каталоге и во всех подкаталогах
      ListFilesInDir(Catalog, &Files);

      //>>> Цикл по всем файлам в каталоге и во всех подкаталогах >>>
      for(int j=0; j<Files.size(); j++)
      {
        //Задать имя файла
        F.setFileName(Files[j]);

        res = F.open(QIODevice::Append);
        if (res == false)
        {  //В классе QFile нельзя открыть файл на дозапись => файл еще копируется, записывается и т.п.
          break;
        }
        else
        {  //Класс QFile может открыть файл на дозапись => файл уже закончил копироваться, записываться и т.п.
          F.close();
        }
      }
      //<<< Цикл по всем файлам в каталоге и во всех подкаталогах <<<

      //Если файлов нет, но есть пустой подкаталог, тогда его отправляю
      if (Files.size() <= 0)
      {
        //Получаем список подкаталогов в каталоге
        QStringList lst = Dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        if (lst.size() > 0)
          res = true;
      }

      if (res == true)
      {
        //Второй раз запрашиваю количество файлов в каталоге абонента
        CountFilesInDir(Catalog, &Q2);

        if (Q1 == Q2)
        if (SendFolders[i]->GetStatus() != TBasicThread::TStatus::WORK)
        {
          //Переименовываю подкаталог абонента
          Dir.rename(Catalog, Catalog + "_TEMP_");

          //Создаю подкаталог абонента
          Dir.mkdir(Catalog);

          #ifdef __unix__
          F.setFileName(Catalog);
          F.setPermissions(QFileDevice::ReadOwner | QFileDevice::ReadGroup | QFileDevice::ReadOther |
                           QFileDevice::WriteOwner | QFileDevice::WriteGroup | QFileDevice::WriteOther |
                           QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
          #endif

          SendFolders[i]->start();
        }
      }
    }

    StartCtrlSimple();  //StartCtrl использовать здесь не надо, чтобы исключить ежесекундное перечитывание каталога абонентов
  }
  else
  {
    QObject::timerEvent(event);
  }
}
