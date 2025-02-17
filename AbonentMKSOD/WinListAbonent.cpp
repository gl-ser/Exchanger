//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinListAbonent.cpp                                                    //
//                                                                            //
//          *** TWINLISTABONENT ОКНО УПРАВЛЕНИЯ СПИСКОМ АБОНЕНТОВ ***         //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "Base.h"
#include "WinListAbonent.h"


TWinListAbonent::TWinListAbonent(QWidget *parent) : QMainWindow(parent)
{
  setupUi(this);

  FIniFileName = "";

  ClearGrid = false;

  Placement = new TPlacement(this);
  TimerCloseListServer = new QBasicTimer();

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();

  Iteration = -1;
  CurrentAbonentName = "";

  TimerIteration = new QBasicTimer();
  TimerIteration->stop();

  connect(&Ping, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&Ping, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
}


TWinListAbonent::~TWinListAbonent()
{
  setWindowTitle("ПОДОЖДИТЕ ЗАВЕРШЕНИЯ ПРИЛОЖЕНИЯ");
  Ping.quit();
  Ping.wait();

  Placement->Save();
  delete Placement;
  delete TimerCloseListServer;
  delete TimerIteration;

  FIniFileName = "";

  ClearGrid = false;

  Iteration = -1;
  CurrentAbonentName = "";
}


void TWinListAbonent::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_LIST_ABONENT");
}


void TWinListAbonent::MoveToCenter()
{
QRect Rect;
QPoint Center;

  //Прямоугольник с размерами экрана
  Rect = qApp->desktop()->availableGeometry(qApp->desktop()->primaryScreen());

  //Координаты центра экрана
  Center = Rect.center();

  //Учитывается половина ширины окна
  Center.setX(Center.x() - (width()/2));

  //Учитывается половина высоты окна
  Center.setY(Center.y() - (height()/2));

  //Смещение окна по указанным координатам
  move(Center);
}


void TWinListAbonent::PingResult(bool Res)
{
  if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
  {
    QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
    if ((db.isValid() == true) && (db.isOpen() == true))
    if (CurrentAbonentName != "")
    {
      QSqlQuery Query1(db);
      QString str;

      if (Res == true)
        str = "update abonent set status = 1 where (name = \"" + CurrentAbonentName + "\")";
      else
        str = "update abonent set status = 2 where (name = \"" + CurrentAbonentName + "\")";

      Query1.exec(str);

      InShowEvent();
      qApp->processEvents();
    }
  }
}


void TWinListAbonent::showEvent(QShowEvent *event)
{
  if(event->type() == QEvent::Show)
  {
    InShowEvent();
  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TWinListAbonent::hideEvent(QHideEvent *event)
{
int i, q;
  if (event->type() == QEvent::Hide)
  {
    TimerCloseListServer->stop();

    //Удаление объектов из "грида". Кол-во строк при этом не меняется
    Grid->clearContents();

    //Удаление старых строк из "грида"
    ClearGrid = true;
    q = Grid->rowCount();
    for(i=0; i<q; i++)
      Grid->removeRow(0);
  }
  else
  {
    QWidget::hideEvent(event);
  }
}


void TWinListAbonent::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == TimerCloseListServer->timerId())
  {
    TimerCloseListServer->stop();
    close();
  }
  else
  {
    if (event->timerId() == TimerIteration->timerId())
    {  //Итерационный опрос абонентов
      if (Grid->rowCount() >= 1)
      {
        TimerIteration->stop();
        Iteration++;

        if (Iteration < 0)
          Iteration = 0;

        if (Iteration >= Grid->rowCount())
          Iteration = 0;

        try
        {
          //Идентификатор абонента из локальной БД (данные из текущей строки "грида")
          int id = Grid->item(Iteration, 0)->text().toInt();

          if (id > 0)
          {
            //(static_cast<TBase*>(OWNER))->TCP->SendPing(Grid->item(Iteration, 1)->text());
            if (Ping.GetStatus() != TBasicThread::TStatus::WORK)
            {
              CurrentAbonentName = Grid->item(Iteration, 1)->text();
              Ping.SetNumber(111);
              Ping.SetAbonent(CurrentAbonentName, Grid->item(Iteration, 2)->text(), Grid->item(Iteration, 3)->text().toInt());
              Ping.SetAnswerPortTCP((static_cast<TBase*>(OWNER))->TCP_port);
              Ping.start();
            }
            else
            {
              Iteration--;
            }
          }
        }
        catch (...)
        {
          if (TimerIteration->isActive() == false)
          {
            TimerIteration->start((static_cast<TBase*>(OWNER))->IntervalIteration*1000, this);
          }
        }
      }
    }
    else
    {
      QWidget::timerEvent(event);
    }
  }
}


void TWinListAbonent::GridScroll()
{

}


void TWinListAbonent::InShowEvent()
{
int i, q, status;
QString str;
QColor Color;
QTableWidgetItem* Item;
  if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
  {
    QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
    if ((db.isValid() == true) && (db.isOpen() == true))
    {
      QSqlQuery Query1(db);
      str = "select id, name, address, tcp_port, status from abonent order by id desc";
      Query1.exec(str);

      //Удаление объектов из "грида". Кол-во строк при этом не меняется
      Grid->clearContents();

      //Удаление старых строк из "грида"
      ClearGrid = true;
      q = Grid->rowCount();
      for(i=0; i<q; i++)
        Grid->removeRow(0);
      ClearGrid = false;

      //Заполнение "грида" данными из SQL-запроса, позиционирование курсора в "гриде", если запрос непустой
      if (Query1.first() == true)
      {
        ClearGrid = true;
        do
        {
          Grid->insertRow(0);

          Item = new QTableWidgetItem(Query1.value(Query1.record().indexOf("id")).toString());
          Item->setTextAlignment(Qt::AlignCenter);
          Grid->setItem(0, 0, Item);

          Item = new QTableWidgetItem(Query1.value(Query1.record().indexOf("name")).toString());
          Item->setTextAlignment(Qt::AlignCenter);
          Grid->setItem(0, 1, Item);

          Item = new QTableWidgetItem(Query1.value(Query1.record().indexOf("address")).toString());
          Item->setTextAlignment(Qt::AlignCenter);
          Grid->setItem(0, 2, Item);

          Item = new QTableWidgetItem(Query1.value(Query1.record().indexOf("tcp_port")).toString());
          Item->setTextAlignment(Qt::AlignCenter);
          Grid->setItem(0, 3, Item);

          status = Query1.value(Query1.record().indexOf("status")).toInt();
          switch (status)
          {
            case 0:
              Item = new QTableWidgetItem("неизвестно");
              Color = QColor(Qt::darkGray);
              Item->setData(Qt::TextColorRole, Color);
            break;

            case 1:
              Item = new QTableWidgetItem("доступен");
              Color = QColor(Qt::darkGreen);
              Item->setData(Qt::TextColorRole, Color);
            break;

            case 2:
              Item = new QTableWidgetItem("недоступен");
              Color = QColor(Qt::red);
              Item->setData(Qt::TextColorRole, Color);
            break;
          };
          Item->setTextAlignment(Qt::AlignCenter);
          Grid->setItem(0, 4, Item);
        }
        while(Query1.next());

        ClearGrid = false;
        Grid->setCurrentCell(0, 0);
        //Позиционировал курсор в списке Grid и тем самым вызвал событие itemSelectionChanged
      }
      //Установить ширину колонок по ширине отображаемых данных
      Grid->resizeColumnsToContents();

      //Убрать вертикальный заголовок
      Grid->verticalHeader()->setVisible(false);

      if (TimerIteration->isActive() == false)
      {
        TimerIteration->start((static_cast<TBase*>(OWNER))->IntervalIteration*1000, this);
      }
    }
    else
    {
      (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
      TimerCloseListServer->start(300, this);
    }
  }
  else
  {
    (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
    TimerCloseListServer->start(300, this);
  }
}


void TWinListAbonent::RestoreCatalogOfAbonent()
{
bool res;
QString str;
QDir Dir;  //Класс управления каталогами
  if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
  if ((static_cast<TBase*>(OWNER))->InCatalog != "")
  if ((static_cast<TBase*>(OWNER))->OutCatalog != "")
  {
    QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
    if ((db.isValid() == true) && (db.isOpen() == true))
    {
      QSqlQuery Query1(db);
      str = "select id, name from abonent order by id desc";
      Query1.exec(str);

      if (Query1.first() == true)
      {
        do
        {
          if (Query1.value(Query1.record().indexOf("name")).toString() != "")
          {
            Dir = QDir((static_cast<TBase*>(OWNER))->OutCatalog + "/" +
                       Query1.value(Query1.record().indexOf("name")).toString().toLower());

            if (Dir.exists() == false)
            {
              //Создаю подкаталог абонента
              res = Dir.mkdir((static_cast<TBase*>(OWNER))->OutCatalog + "/" +
                              Query1.value(Query1.record().indexOf("name")).toString().toLower());
              if (res == false)
              {
                (static_cast<TBase*>(OWNER))->AddToLogBook("Ошибка создания подкаталога абонента. Возможно неверное имя абонента");
                (static_cast<TBase*>(OWNER))->OutCatalog = "";
                break;
              }
            }

            Dir = QDir((static_cast<TBase*>(OWNER))->InCatalog + "/" +
                       Query1.value(Query1.record().indexOf("name")).toString().toLower());

            if (Dir.exists() == false)
            {
              //Создаю подкаталог абонента
              res = Dir.mkdir((static_cast<TBase*>(OWNER))->InCatalog + "/" +
                              Query1.value(Query1.record().indexOf("name")).toString().toLower());
              if (res == false)
              {
                (static_cast<TBase*>(OWNER))->AddToLogBook("Ошибка создания подкаталога абонента. Возможно неверное имя абонента");
                (static_cast<TBase*>(OWNER))->InCatalog = "";
                break;
              }
            }

            #ifdef __unix__
            QFile f;
            f.setFileName((static_cast<TBase*>(OWNER))->OutCatalog + "/" +
                          Query1.value(Query1.record().indexOf("name")).toString().toLower());

            f.setPermissions(QFileDevice::ReadOwner | QFileDevice::ReadGroup | QFileDevice::ReadOther |
                             QFileDevice::WriteOwner | QFileDevice::WriteGroup | QFileDevice::WriteOther |
                             QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);

            f.setFileName((static_cast<TBase*>(OWNER))->InCatalog + "/" +
                          Query1.value(Query1.record().indexOf("name")).toString().toLower());

            f.setPermissions(QFileDevice::ReadOwner | QFileDevice::ReadGroup | QFileDevice::ReadOther |
                             QFileDevice::WriteOwner | QFileDevice::WriteGroup | QFileDevice::WriteOther |
                             QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
            #endif
          }
        }
        while(Query1.next());
      }
    }
  }
}


TWinListAbonent::TAbonents TWinListAbonent::GetAbonents()
{
QString str;
TAbonent A;
TAbonents Abon;
  Abon.clear();
  if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
  {
    QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
    if ((db.isValid() == true) && (db.isOpen() == true))
    {
      QSqlQuery Query1(db);
      str = "select id, name, address, tcp_port from abonent order by id desc";
      Query1.exec(str);

      if (Query1.first() == true)
      {
        do
        {
          if (Query1.value(Query1.record().indexOf("name")).toString() != "")
          {
            A.ID = Query1.value(Query1.record().indexOf("id")).toInt();
            A.name = Query1.value(Query1.record().indexOf("name")).toString();
            A.address = Query1.value(Query1.record().indexOf("address")).toString();
            A.port = Query1.value(Query1.record().indexOf("tcp_port")).toInt();

            Abon.push_back(A);
          }
        }
        while(Query1.next());
      }
    }
  }
  return Abon;
}


void TWinListAbonent::on_Grid_itemSelectionChanged()
{  //Перемещение курсора по Grid
  GridScroll();
}


void TWinListAbonent::on_action_triggered()
{  //Добавить нового абонента
  TimerIteration->stop();

  static_cast<TBase*>(OWNER)->StopServer();
  static_cast<TBase*>(OWNER)->CtrlOUTCat->StopCtrl();

  (static_cast<TBase*>(OWNER))->AddAbonent->exec();

  static_cast<TBase*>(OWNER)->StartServer();
  static_cast<TBase*>(OWNER)->CtrlOUTCat->StartCtrl();

  TimerIteration->start((static_cast<TBase*>(OWNER))->IntervalIteration*1000, this);
}


void TWinListAbonent::on_action_2_triggered()
{  //Редактировать параметры выбранного абонента
int id;
  TimerIteration->stop();

  if (Grid->currentRow() >= 0)
  {
    static_cast<TBase*>(OWNER)->StopServer();
    static_cast<TBase*>(OWNER)->CtrlOUTCat->StopCtrl();

    //Идентификатор сервера из локальной БД (данные из текущей строки "грида")
    id = Grid->item(Grid->currentRow(), 0)->text().toInt();

    if (id > 0)
      (static_cast<TBase*>(OWNER))->EditAbonent->Edit(id);

    static_cast<TBase*>(OWNER)->StartServer();
    static_cast<TBase*>(OWNER)->CtrlOUTCat->StartCtrl();
  }

  TimerIteration->start((static_cast<TBase*>(OWNER))->IntervalIteration*1000, this);
}


void TWinListAbonent::on_action_3_triggered()
{  //Удалить выбранного абонента
QString str, id, name;
bool res;
QDir Dir;  //Класс управления каталогами
  TimerIteration->stop();

  if (Grid->currentRow() >= 0)
  {
    static_cast<TBase*>(OWNER)->StopServer();
    static_cast<TBase*>(OWNER)->CtrlOUTCat->StopCtrl();

    //Идентификатор абонента из локальной БД (данные из текущей строки "грида")
    id = Grid->item(Grid->currentRow(), 0)->text();

    name = Grid->item(Grid->currentRow(), 1)->text();

    if (id.toInt() > 0)
    if ((static_cast<TBase*>(OWNER))->CancelOK->WhatCanIDo() == true)
    {
      if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
      {
        QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
        if ((db.isValid() == true) && (db.isOpen() == true))
        {
          QSqlQuery Query1(db);
          str = "delete from abonent where(id = " + id + ")";
          res = Query1.exec(str);

          if (res == true)
          {
            Dir = QDir((static_cast<TBase*>(OWNER))->OutCatalog + "/" + name);

            //Получаем список файлов в подкаталоге пользователя
            QStringList lstFilesOut = Dir.entryList(QDir::Files);

            //Удаляем файлы из подкаталога пользователя
            foreach (QString entry, lstFilesOut)
            {
              QString entryAbsPath = Dir.absolutePath() + "/" + entry;
              QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
              QFile::remove(entryAbsPath);
            }

            //Удаляем сам подкаталог пользователя
            Dir.rmdir((static_cast<TBase*>(OWNER))->OutCatalog + "/" + name);


            Dir = QDir((static_cast<TBase*>(OWNER))->InCatalog + "/" + name);

            //Получаем список файлов в подкаталоге пользователя
            QStringList lstFilesIn = Dir.entryList(QDir::Files);

            //Удаляем файлы из подкаталога пользователя
            foreach (QString entry, lstFilesIn)
            {
              QString entryAbsPath = Dir.absolutePath() + "/" + entry;
              QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
              QFile::remove(entryAbsPath);
            }

            //Удаляем сам подкаталог пользователя
            Dir.rmdir((static_cast<TBase*>(OWNER))->InCatalog + "/" + name);

            InShowEvent();  //Перерисовать содержимое окна
            (static_cast<TBase*>(OWNER))->Error->MessageGreen("Сведения об абоненте успешно удалены");
            (static_cast<TBase*>(OWNER))->AddToLogBook("Удалены сведения об абоненте \"" + name + "\"");
          }
          else
          {
            (static_cast<TBase*>(OWNER))->Error->MessageRed("Ошибка удаления абонента. Причина: " +
                                                            Query1.lastError().text().toStdString());
          }
        }
        else
        {
          (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
          TimerCloseListServer->start(300, this);
        }
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
        TimerCloseListServer->start(300, this);
      }
    }

    static_cast<TBase*>(OWNER)->StartServer();
    static_cast<TBase*>(OWNER)->CtrlOUTCat->StartCtrl();
  }

  TimerIteration->start((static_cast<TBase*>(OWNER))->IntervalIteration*1000, this);
}


void TWinListAbonent::OnError(int Number, int Duration, QString Str)
{
  Q_UNUSED(Duration);
  Q_UNUSED(Str);

  if (Number == 111)
    PingResult(false);
}


void TWinListAbonent::OnSuccess(int Number, int Duration)
{
  Q_UNUSED(Duration);

  if (Number == 111)
    PingResult(true);
}
