//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл MainWin.cpp                                                           //
//                                                                            //
//            *** ГЛАВНОЕ ОКНО СРЕДСТВА ПЕРЕЗАГРУЗКИ ПРИЛОЖЕНИЯ ***           //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "MainWin.h"


TMainWin::TMainWin(QWidget *parent) : QMainWindow(parent)
{
std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна
QStringList ParamStr;      //Командная строка приложения
int i;

  setupUi(this);

  //Устанавливаю значки заголовка окна: "минимизация" и "закрытие"
  setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

  FIniFileName = "";
  NAME = "AbonentMKSOD";

  //>>> Разбор командной строки >>>
  ParamStr = qApp->arguments();

  Key = "";

  for(i=1; i<=ParamStr.size()-1; i++)
  {
    if (ParamStr[i].size() > 1)
    {
      if (ParamStr[i].mid(0,3).toStdString() == "rb=")
        Key = ParamStr[i].mid(3).toStdString();
    }
  }
  //<<< Разбор командной строки <<<

  Placement = new TPlacement(this);

  if (Key == "reboot")
  {
    //Главный INI-файл приложения. Запоминаю его имя и путь к нему
    FIniFileName = qApp->applicationDirPath().toStdString() + "/" + NAME + ".ini";

    Placement->Restore(FIniFileName, "WIN_MAIN");
  }

  TimerQuit = new QBasicTimer();
  TimerQuit->start(3000, this);
}


TMainWin::~TMainWin(void)
{
  Placement->Save();
  delete Placement;
  delete TimerQuit;
  Key = "";
  NAME = "";
}


void TMainWin::timerEvent(QTimerEvent *event)
{
std::string str;
QProcess* Prog;
QStringList* Arg;
  if (event->timerId() == TimerQuit->timerId())
  {
    TimerQuit->stop();
    Prog = new QProcess(this);
    Arg = new QStringList();

    str = "";

    if (Key == "reboot")
      str = qApp->applicationDirPath().toStdString() + "/" + NAME;

    #ifndef __unix__
    str = str + ".exe";
    #endif

    if (Key == "reboot")
    {
      Prog->startDetached(QString::fromStdString(str));
    }

    delete Prog;
    delete Arg;
    close();
  }
  else
  {
    QWidget::timerEvent(event);
  }
}
