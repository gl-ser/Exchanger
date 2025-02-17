//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл MainWin.cpp                                                           //
//                                                                            //
//                 *** TMAINWIN ГЛАВНОЕ ОКНО АБОНЕНТА ИСИО ***                //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "MainWin.h"


TMainWin::TMainWin(QWidget *parent) : QMainWindow(parent)
{
  setupUi(this);

  QApplication::setStyle(QStyleFactory::create("windows"));

  //Устанавливаю значки заголовка окна: "минимизация" и "закрытие"
  setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

  //Запрет изменения размеров окна пользователем
  //setFixedSize(size());

  ReNAME = "ReStartMKSOD";

  CurTimeText = new QLabel(this);
  this->statusBarMain->addWidget(CurTimeText);

  CurTimeText->setText("XXXXXXXXXXXXXXXXXXXXXXXXXX");

  Port = new QLabel(this);
  this->statusBarMain->addWidget(Port);

  Port->setText(" Порт: нет подключения ");

  CurTimeTimer = new QBasicTimer();

  TimerQuit = new QBasicTimer();  //Таймер выхода из приложения (по умолчанию он остановлен)

  TimerFirstStart = new QBasicTimer();

  FIniFileName = "";
  Placement = new TPlacement(this);

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();

  //Создание главного модуля данных приложения
  Base = new TBase(this);

  if (Base->Result == false)
  {
    Base->Error->MessageGreen("Корректная работа приложения невозможна");
    TimerQuit->start(300, this);
  }
  CurTimeTimer->start(1000, this);

  //progressBar->setMinimum(0);
  //progressBar->setMaximum(100);
  //progressBar->setValue(0);
}


TMainWin::~TMainWin(void)
{
  Placement->Save();
  CurTimeTimer->stop();
  delete Base;
  delete CurTimeTimer;
  delete CurTimeText;
  delete Port;
  delete TimerQuit;
  delete TimerFirstStart;
  delete Placement;
  ReNAME = "";
}


void TMainWin::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_MAIN");
}


void TMainWin::FirstStart()
{
    TimerFirstStart->start(2000, this);
}


void TMainWin::Set_Port(QString Value)
{
  Port->setText(Value);
}


void TMainWin::on_action_triggered()
{  //Параметры приложения
  Base->Tuner->close();
  Base->Tuner->show();
}


void TMainWin::on_action_3_triggered()
{  //Перезагрузить приложение
std::string str;
QProcess* Prog;
QStringList* Arg;

  if (Base->CancelOK->WhatCanIDo() == true)
  {
    Base->CtrlOUTCat->StopCtrl();

    Base->Log->TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Приложение перезагружено");

    Base->IsReboot = true;

    Prog = new QProcess(this);
    Arg = new QStringList();

    str = qApp->applicationDirPath().toStdString() + "/" + ReNAME;

    #ifndef __unix__
    str = str + ".exe";
    #endif

    Arg->push_back(QString::fromStdString("rb=reboot"));
    Prog->startDetached(QString::fromStdString(str), *Arg);

    delete Prog;
    delete Arg;
    close();
  }
}


void TMainWin::on_action_4_triggered()
{  //Выход
  close();
}


void TMainWin::on_action_14_triggered()
{  //О программе
  Base->About->close();
  Base->About->show();
}


void TMainWin::MoveToCenter()
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


void TMainWin::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == CurTimeTimer->timerId())
  {
    CurTimeText->setText(" тек. время " + QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate)+" ");
  }
  else
  {
    if (event->timerId() == TimerQuit->timerId())
    {
      TimerQuit->stop();
      close();
    }
    else
    {
      if (event->timerId() == TimerFirstStart->timerId())
      {
        TimerFirstStart->stop();
        Base->Tuner->exec();
      }
      else
      {
        QWidget::timerEvent(event);
      }
    }
  }
}


void TMainWin::on_action_2_triggered()
{  //Открыть журнал
  Base->Log->close();
  Base->Log->show();
}


void TMainWin::on_action_5_triggered()
{  //Управление списком абонентов
  Base->ListAbonent->close();
  Base->ListAbonent->show();
}
