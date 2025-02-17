//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinEditAbonent.cpp                                                    //
//                                                                            //
//       *** TWINEDITABONENT ОКНО РЕДАКТИРОВАНИЯ ПАРАМЕТРОВ АБОНЕНТА ***      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "WinEditAbonent.h"
#include "Base.h"


TWinEditAbonent::TWinEditAbonent(QWidget *parent) : QDialog(parent)
{
  setupUi(this);

  #ifndef __unix__
  //Устанавливаю значки заголовка окна: "закрытие"
  setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
  #endif

  //Запрет изменения размеров окна пользователем
  setFixedSize(size());

  OWNER = NULL;
  FIniFileName = "";
  IsOK = false;
  IsChange = true;
  ID = 0;
  Placement = new TPlacement(this);

  TimerCloseEditServer = new QBasicTimer();

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();
}


TWinEditAbonent::~TWinEditAbonent(void)
{
  TimerCloseEditServer->stop();
  Placement->Save();
  OWNER = NULL;
  IsOK = false;
  IsChange = false;
  ID = 0;
  delete TimerCloseEditServer;
  delete Placement;
}


void TWinEditAbonent::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_EDIT_ABONENT");
}


void TWinEditAbonent::Edit(int id)
{
QString str;
  if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
  {
    QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
    if ((db.isValid() == true) && (db.isOpen() == true))
    {
      QSqlQuery Query1(db);
      str = "select id, name, address, tcp_port from abonent where (id = " + QString::number(id) + ")";
      Query1.exec(str);

      if (Query1.first() == true)
      {
        ID = id;

        EditAddress->setText(Query1.value(Query1.record().indexOf("address")).toString());
        EditName->setText(Query1.value(Query1.record().indexOf("name")).toString());
        EditPort->setText(Query1.value(Query1.record().indexOf("tcp_port")).toString());

        exec();
      }
    }
    else
    {
      (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
      TimerCloseEditServer->start(300, this);
    }
  }
  else
  {
    (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
    TimerCloseEditServer->start(300, this);
  }

}


void TWinEditAbonent::keyPressEvent(QKeyEvent *event)
{
  //Обработка нажатия на клавишу Esc
  if(event->key() == Qt::Key_Escape)
  {
    IsOK = false;
    close();
  }
  else
  {
    QWidget::keyPressEvent(event);
  }
}


void TWinEditAbonent::showEvent(QShowEvent *event)
{
  if (event->type() == QEvent::Show)
  {
    if (IsChange == true)
    {

    }
    IsOK = false;
    CancelButton->setFocus();
  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TWinEditAbonent::hideEvent(QHideEvent *event)
{
int id;
bool pr1, pr2, pr3, pr4, res, res1, res2, res3;
QString str, name, address;
QDir Dir;  //Класс управления каталогами
QString OldName;
  id = 0;
  pr1 = false;
  pr2 = false;
  pr3 = false;
  pr4 = false;
  OldName = "";
  if(event->type() == QEvent::Hide)
  {
    if (IsOK == true)
    {  //Сохранить изменения
      if ((EditAddress->text().trimmed().length() >= 7) && (EditAddress->text().trimmed().length() <= 15))
      {
        pr1 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан IP-адрес абонента");
        TimerCloseEditServer->start(300, this);
      }

      if (EditName->text().trimmed().length() > 0)
      {
        res = false;
        res1 = false;
        if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
        {
          QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
          if ((db.isValid() == true) && (db.isOpen() == true))
          {
            QSqlQuery Query1(db);
            str = "select id, name, address from abonent";
            Query1.exec(str);

            if (Query1.first() == true)
            {
              do
              {
                address = Query1.value(Query1.record().indexOf("address")).toString();
                name = Query1.value(Query1.record().indexOf("name")).toString();
                id = Query1.value(Query1.record().indexOf("id")).toString().toInt();

                if (id == ID)
                  OldName = name;

                if ((name == EditName->text().toLower()) && (id != ID))
                  res = true;

                if ((address == EditAddress->text().toLower()) && (id != ID))
                  res1 = true;
              }
              while(Query1.next());
            }
          }
        }

        if (res == false)
        {
          pr2 = true;
        }
        else
        {
          (static_cast<TBase*>(OWNER))->Error->MessageRed("Абонент с таким именем уже существует");
          TimerCloseEditServer->start(300, this);
        }

        if (res1 == false)
        {
          pr4 = true;
        }
        else
        {
          (static_cast<TBase*>(OWNER))->Error->MessageRed("Абонент с таким IP-адресом уже существует");
          TimerCloseEditServer->start(300, this);
        }
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задано имя абонента");
        TimerCloseEditServer->start(300, this);
      }

      if (EditPort->text().trimmed().toInt() > 0)
      {
        pr3 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан номер TCP-порта");
        TimerCloseEditServer->start(300, this);
      }
    }
    else
    {  //Не сохранять изменения
      IsChange = true;
    }

    if ((pr1 == true) && (pr2 == true) && (pr3 == true) && (pr4 == true))
    {


      if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
      {
        QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
        if ((db.isValid() == true) && (db.isOpen() == true))
        {
          QSqlQuery Query1(db);

          str = "update abonent set name = \"" + EditName->text().trimmed().toLower() + "\" where id = " + QString::number(ID);
          res1 = Query1.exec(str);

          str = "update abonent set address = \"" + EditAddress->text().trimmed() + "\" where id = " + QString::number(ID);
          res2 = Query1.exec(str);

          str = "update abonent set tcp_port = " + EditPort->text().trimmed() + " where id = " + QString::number(ID);
          res3 = Query1.exec(str);

          if ((res1 == true) && (res2 == true) && (res3 == true))
          {
            if (OldName != "")
            {
              Dir = QDir((static_cast<TBase*>(OWNER))->OutCatalog + "/" + OldName);

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
              Dir.rmdir((static_cast<TBase*>(OWNER))->OutCatalog + "/" + OldName);

              qApp->processEvents(QEventLoop::AllEvents, 100);

              Dir = QDir((static_cast<TBase*>(OWNER))->InCatalog + "/" + OldName);

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
              Dir.rmdir((static_cast<TBase*>(OWNER))->InCatalog + "/" + OldName);
            }

            qApp->processEvents(QEventLoop::AllEvents, 100);

            //Создаю подкаталоги абонента
            str = (static_cast<TBase*>(OWNER))->OutCatalog + "/" + EditName->text().toLower();
            res1 = Dir.mkdir(str);
            if (res1 == false)
            {
              (static_cast<TBase*>(OWNER))->AddToLogBook("Ошибка создания подкаталогов абонента. Возможно неверное имя абонента");
            }

            qApp->processEvents(QEventLoop::AllEvents, 100);

            str = (static_cast<TBase*>(OWNER))->InCatalog + "/" + EditName->text().toLower();
            res2 = Dir.mkdir(str);
            if (res2 == false)
            {
              (static_cast<TBase*>(OWNER))->AddToLogBook("Ошибка создания подкаталогов абонента. Возможно неверное имя абонента");
            }

            qApp->processEvents(QEventLoop::AllEvents, 100);

            #ifdef __unix__
            QFile f;
            f.setFileName((static_cast<TBase*>(OWNER))->OutCatalog + "/" + EditName->text().toLower());
            f.setPermissions(QFileDevice::ReadOwner | QFileDevice::ReadGroup | QFileDevice::ReadOther |
                             QFileDevice::WriteOwner | QFileDevice::WriteGroup | QFileDevice::WriteOther |
                             QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);

            f.setFileName((static_cast<TBase*>(OWNER))->InCatalog + "/" + EditName->text().toLower());
            f.setPermissions(QFileDevice::ReadOwner | QFileDevice::ReadGroup | QFileDevice::ReadOther |
                             QFileDevice::WriteOwner | QFileDevice::WriteGroup | QFileDevice::WriteOther |
                             QFileDevice::ExeOwner | QFileDevice::ExeGroup | QFileDevice::ExeOther);
            #endif

            (static_cast<TBase*>(OWNER))->ListAbonent->InShowEvent();
            (static_cast<TBase*>(OWNER))->AddToLogBook("Изменены параметры абонента \"" + EditName->text().toLower() + "\"");

            (static_cast<TBase*>(OWNER))->Error->MessageGreen("Изменения параметров абонента успешно произведены");
          }
          else
          {
            (static_cast<TBase*>(OWNER))->Error->MessageRed("Ошибка изменения параметров абонента. Причина: " +
                                                            Query1.lastError().text().toStdString());
          }
        }
        else
        {
          (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
        }
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Нет подключения к БД");
      }

      IsChange = true;
    }
  }
  else
  {
    QWidget::hideEvent(event);
  }
}


void TWinEditAbonent::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == TimerCloseEditServer->timerId())
  {
    TimerCloseEditServer->stop();
    IsChange = false;
    this->exec();
  }
  else
  {
    QWidget::timerEvent(event);
  }
}


void TWinEditAbonent::MoveToCenter()
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


void TWinEditAbonent::on_OKButton_clicked()
{
  IsOK = true;
  close();
}


void TWinEditAbonent::on_CancelButton_clicked()
{
  IsOK = false;
  close();
}
