//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinTuner.cpp                                                          //
//                                                                            //
//                       *** TWINTUNER ОКНО НАСТРОЕК ***                      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "WinTuner.h"
#include "Base.h"
#include "MainWin.h"


TWinTuner::TWinTuner(QWidget *parent) : QDialog(parent)
{
  setupUi(this);

  #ifndef __unix__
  //Устанавливаю значки заголовка окна: "закрытие"
  setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
  #endif

  //Запрет изменения размеров окна пользователем
  //setFixedSize(size());
  setFixedHeight(size().rheight());

  OWNER = NULL;
  FIniFileName = "";
  IsOK = false;
  IsChange = true;
  Placement = new TPlacement(this);

  TimerCloseWinTuner = new QBasicTimer();

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();
}


TWinTuner::~TWinTuner(void)
{
  TimerCloseWinTuner->stop();
  Placement->Save();
  OWNER = NULL;
  IsOK = false;
  IsChange = false;
  delete TimerCloseWinTuner;
  delete Placement;
}


void TWinTuner::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_TUNER");
}


void TWinTuner::keyPressEvent(QKeyEvent *event)
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


void TWinTuner::showEvent(QShowEvent *event)
{
  if (event->type() == QEvent::Show)
  {
    if (IsChange == true)
    {
      EditInCatalog->setText(static_cast<TBase*>(OWNER)->InCatalog);
      EditOutCatalog->setText(static_cast<TBase*>(OWNER)->OutCatalog);
      EditTempCatalog->setText(static_cast<TBase*>(OWNER)->TempCatalog);
      EditTCPPort->setText(QString::number(static_cast<TBase*>(OWNER)->TCP_port));
      EditIntervalIteration->setText(QString::number(static_cast<TBase*>(OWNER)->IntervalIteration));
      CheckWin->setChecked(static_cast<TBase*>(OWNER)->CheckWin);
    }
    IsOK = false;
    CancelButton->setFocus();
  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TWinTuner::hideEvent(QHideEvent *event)
{
bool pr1, pr2, pr3, pr4, pr5;
QSettings *ConfigFile;  //INI-файл настроек приложения
QString str, name;

  pr1 = false;
  pr2 = false;
  pr3 = false;
  pr4 = false;
  pr5 = false;

  if(event->type() == QEvent::Hide)
  {
    if (IsOK == true)
    {  //Сохранить изменения
      if ((QFile(EditInCatalog->text()).exists() == true) || (EditInCatalog->text() == ""))
      {
        pr1 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан каталог входящих файлов");
        TimerCloseWinTuner->start(300, this);
      }

      if ((QFile(EditOutCatalog->text()).exists() == true) || (EditOutCatalog->text() == ""))
      {
        pr2 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан каталог исходящих файлов");
        TimerCloseWinTuner->start(300, this);
      }

      if (EditTCPPort->text().toInt() > 0)
      {
        pr3 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан номер порта TCP");
        TimerCloseWinTuner->start(300, this);
      }

      if (EditIntervalIteration->text().toInt() >= 1)
      {
        pr4 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан интервал опроса абонентов");
        TimerCloseWinTuner->start(300, this);
      }

      if ((QFile(EditTempCatalog->text()).exists() == true) || (EditTempCatalog->text() == ""))
      {
        pr5 = true;
      }
      else
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Неверно задан каталог временных файлов");
        TimerCloseWinTuner->start(300, this);
      }

    }
    else
    {  //Не сохранять изменения
      //Пользователь отменил всю свою работу по настройкам => надо все элементы управления вернуть в
      // первоначальное состояние
      EditInCatalog->setText(static_cast<TBase*>(OWNER)->InCatalog);
      EditOutCatalog->setText(static_cast<TBase*>(OWNER)->OutCatalog);
      EditTempCatalog->setText(static_cast<TBase*>(OWNER)->TempCatalog);
      EditTCPPort->setText(QString::number(static_cast<TBase*>(OWNER)->TCP_port));
      EditIntervalIteration->setText(QString::number(static_cast<TBase*>(OWNER)->IntervalIteration));
      CheckWin->setChecked(static_cast<TBase*>(OWNER)->CheckWin);

      IsChange = true;
    }

    if ((pr1 == true) && (pr2 == true) && (pr3 == true) && (pr4 == true) && (pr5 == true))
    {
      static_cast<TBase*>(OWNER)->StopServer();
      static_cast<TBase*>(OWNER)->CtrlOUTCat->StopCtrl();

      static_cast<TBase*>(OWNER)->InCatalog = EditInCatalog->text();
      static_cast<TBase*>(OWNER)->OutCatalog = EditOutCatalog->text();
      static_cast<TBase*>(OWNER)->TempCatalog = EditTempCatalog->text();
      static_cast<TBase*>(OWNER)->TCP_port = EditTCPPort->text().toInt();
      static_cast<TBase*>(OWNER)->IntervalIteration = EditIntervalIteration->text().toInt();
      static_cast<TBase*>(OWNER)->CheckWin = CheckWin->isChecked();

      //>>> Работа с INI-файлом >>>
      ConfigFile = new QSettings(QString::fromStdString(FIniFileName), QSettings::IniFormat);

      ConfigFile->setValue("USER/InCatalog", EditInCatalog->text());
      ConfigFile->setValue("USER/OutCatalog", EditOutCatalog->text());
      ConfigFile->setValue("USER/TempCatalog", EditTempCatalog->text());
      ConfigFile->setValue("USER/TCP_Port", EditTCPPort->text());
      ConfigFile->setValue("USER/IntervalIteration", EditIntervalIteration->text());

      if (CheckWin->isChecked() == true)
        ConfigFile->setValue("USER/CheckWin", "1");
      else
        ConfigFile->setValue("USER/CheckWin", "0");

      delete ConfigFile;
      //<<< Работа с INI-файлом <<<

      static_cast<TBase*>(OWNER)->AddToLogBook("Изменены параметры приложения");

      IsChange = true;

      static_cast<TBase*>(OWNER)->ListAbonent->RestoreCatalogOfAbonent();

      static_cast<TBase*>(OWNER)->StartServer();

      static_cast<TBase*>(OWNER)->ListAbonent->TimerIteration->start((static_cast<TBase*>(OWNER))->IntervalIteration*1000,
                                                                     (static_cast<TBase*>(OWNER))->ListAbonent);

      (static_cast<TMainWin*>(static_cast<TBase*>(OWNER)->OWNER))->setWindowTitle("Абонент МКСОД");
      static_cast<TBase*>(OWNER)->Log->setWindowTitle("Протокол Абонента МКСОД");
      static_cast<TBase*>(OWNER)->ListAbonent->setWindowTitle("Управление списком абонентов");

      if (CheckWin->isChecked() == true)
      {
        static_cast<TBase*>(OWNER)->Log->show();
        static_cast<TBase*>(OWNER)->ListAbonent->show();
      }

      static_cast<TBase*>(OWNER)->CtrlOUTCat->StartCtrl();
    }
  }
  else
  {
    QWidget::hideEvent(event);
  }
}


void TWinTuner::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == TimerCloseWinTuner->timerId())
  {
    TimerCloseWinTuner->stop();
    IsChange = false;
    this->exec();
  }
  else
  {
    QWidget::timerEvent(event);
  }
}


void TWinTuner::MoveToCenter()
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


void TWinTuner::on_OKButton_clicked()
{
  IsOK = true;
  close();
}


void TWinTuner::on_CancelButton_clicked()
{
  IsOK = false;
  close();
}


void TWinTuner::on_FileButton_clicked()
{
QString str;

  str = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Открыть каталог входящих файлов"), qApp->applicationDirPath());

  if (str != "")
  {
    EditInCatalog->setText(str);
  }
}


void TWinTuner::on_FileButton_2_clicked()
{
QString str;

  str = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Открыть каталог исходящих файлов"), qApp->applicationDirPath());

  if (str != "")
  {
    EditOutCatalog->setText(str);
  }
}


void TWinTuner::on_FileButton_3_clicked()
{
QString str;

  str = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Открыть каталог временных файлов"), qApp->applicationDirPath());

  if (str != "")
  {
    EditTempCatalog->setText(str);
  }
}
