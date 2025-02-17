//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinAddAbonent.cpp                                                     //
//                                                                            //
//           *** TWINADDABONENT ОКНО ДОБАВЛЕНИЯ НОВОГО АБОНЕНТА ***           //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "WinAddAbonent.h"
#include "Base.h"


TWinAddAbonent::TWinAddAbonent(QWidget *parent) : QDialog(parent)
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
  Placement = new TPlacement(this);

  TimerCloseAddAbonent = new QBasicTimer();

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();
}


TWinAddAbonent::~TWinAddAbonent(void)
{
  TimerCloseAddAbonent->stop();
  Placement->Save();
  OWNER = NULL;
  IsOK = false;
  IsChange = false;
  delete TimerCloseAddAbonent;
  delete Placement;
}


void TWinAddAbonent::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_ADD_ABONENT");
}


void TWinAddAbonent::keyPressEvent(QKeyEvent *event)
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


void TWinAddAbonent::showEvent(QShowEvent *event)
{
  if (event->type() == QEvent::Show)
  {
    if (IsChange == true)
    {
      EditName->setText("abonent_mksod");
      EditAddress->setText("0.0.0.0");
      EditPort->setText("5902");
    }
    IsOK = false;
    CancelButton->setFocus();
  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TWinAddAbonent::hideEvent(QHideEvent *event)
{
bool pr1, pr2, pr3, pr4, res1, res2;
QString str, name, address;
QDir Dir;  //Класс управления каталогами

  pr1 = false;
  pr2 = false;
  pr3 = false;
  pr4 = false;

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
        TimerCloseAddAbonent->start(300, this);
      }

      if (EditName->text().trimmed().length() > 0)
      {
        res1 = false;
        res2 = false;
        if ((static_cast<TBase*>(OWNER))->SQLiteDB->GiveStatusConnect() == true)
        {
          QSqlDatabase db = QSqlDatabase::database((static_cast<TBase*>(OWNER))->SQLiteDB->GiveConnectionName());
          if ((db.isValid() == true) && (db.isOpen() == true))
          {
            QSqlQuery Query1(db);
            str = "select name, address from abonent";
            Query1.exec(str);

            if (Query1.first() == true)
            {
              do
              {
                name = Query1.value(Query1.record().indexOf("name")).toString();
                address = Query1.value(Query1.record().indexOf("address")).toString();

                if (name == EditName->text().toLower())
                  res1 = true;

                if (address == EditAddress->text())
                  res2 = true;
              }
              while(Query1.next());
            }
          }
        }

        if (res1 == false)
        {
          pr2 = true;
        }
        else
        {
          (static_cast<TBase*>(OWNER))->Error->MessageRed("Абонент с таким именем уже существует");
          TimerCloseAddAbonent->start(300, this);
        }

        if (res2 == false)
        {
          pr4 = true;
        }
        else
        {
          (static_cast<TBase*>(OWNER))->Error->MessageRed("Абонент с таким IP-адресом уже существует");
          TimerCloseAddAbonent->start(300, this);
        }
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задано имя абонента");
        TimerCloseAddAbonent->start(300, this);
      }

      if (EditPort->text().toInt() > 0)
      {
        pr3 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан номер TCP-порта");
        TimerCloseAddAbonent->start(300, this);
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
          str = "insert into abonent (name, address, tcp_port, status) values(\"" + EditName->text().toLower().trimmed() +
                "\", \"" + EditAddress->text().trimmed() + "\", " + EditPort->text().trimmed() + ", 0)";
          res1 = Query1.exec(str);

          if (res1 == true)
          {
            //Создаю подкаталоги абонента
            res1 = Dir.mkdir((static_cast<TBase*>(OWNER))->OutCatalog + "/" + EditName->text().toLower());
            res2 = Dir.mkdir((static_cast<TBase*>(OWNER))->InCatalog + "/" + EditName->text().toLower());

            if ((res1 == false) || (res2 == false))
              (static_cast<TBase*>(OWNER))->AddToLogBook("Ошибка создания подкаталогов абонента. Возможно неверное имя абонента");

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
            (static_cast<TBase*>(OWNER))->AddToLogBook("Добавлены сведения о новом абоненте \"" + EditName->text().toLower() + "\"");

            (static_cast<TBase*>(OWNER))->Error->MessageGreen("Сведения о новом абоненте успешно добавлены");
          }
          else
          {
            (static_cast<TBase*>(OWNER))->Error->MessageRed("Ошибка добавления нового абонента. Причина: " +
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


void TWinAddAbonent::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == TimerCloseAddAbonent->timerId())
  {
    TimerCloseAddAbonent->stop();
    IsChange = false;
    this->exec();
  }
  else
  {
    QWidget::timerEvent(event);
  }
}


void TWinAddAbonent::MoveToCenter()
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


void TWinAddAbonent::on_OKButton_clicked()
{
  IsOK = true;
  close();
}


void TWinAddAbonent::on_CancelButton_clicked()
{
  IsOK = false;
  close();
}
