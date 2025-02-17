//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinLogBook.cpp                                                        //
//                                                                            //
//                      *** TWINLOGBOOK ОКНО ЖУРНАЛА ***                      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "WinLogBook.h"
#include "Base.h"


TWinLogBook::TWinLogBook(QWidget *parent) : QMainWindow(parent)
{
  setupUi(this);

  OWNER = NULL;

  FIniFileName = "";
  Placement = new TPlacement(this);

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();

  CurTimerLog = new QBasicTimer();
  TimeStartLog = QDateTimeToTDateTime(QDateTime::currentDateTime());

  TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Протокол создан");

QFile F;
QString FN;
  FN = qApp->applicationDirPath() + "/" + "AbonentMKSOD.txt";
  F.setFileName(FN);
  QFileInfo FileInfo(F);

  if (FileInfo.size() > 1000000)
  {
    if (F.open(QIODevice::WriteOnly | QIODevice::Text) == true)
      F.close();

    TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Очищен главный LOG-файл приложения");
  }
  CurTimerLog->start(10*1000, this);  //Срабатывание таймера каждые 10 секунд
}


TWinLogBook::~TWinLogBook()
{
  Placement->Save();
  delete Placement;
  FIniFileName = "";

  OWNER = NULL;

  CurTimerLog->stop();
  delete CurTimerLog;
}


void TWinLogBook::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_LOG_BOOK");
}


void TWinLogBook::SaveToFile(QString FilePathName, bool ReWrite)
{
bool IsOpen;
QFile File;        //Файл журнала
QTextStream Data;  //Содержимое файла журнала
QString Codec;     //Кодировка текстового файла

  if (FilePathName != "")
  {
    Codec = "utf-8";
    Data.setDevice(&File);  //Связать содержимое файла с самим файлом
    Data.setCodec(Codec.toStdString().c_str());  //Кодировка текстового файла

    //Задать имя файла
    File.setFileName(FilePathName);

    if (ReWrite == true)
    {
      //Создать в файловой системе новый (или очистить существующий) текстовый файл и открыть его за запись
      IsOpen = File.open(QIODevice::WriteOnly | QIODevice::Text);
    }
    else
    {
      //Дописать в конец файла
      IsOpen = File.open(QIODevice::Append | QIODevice::Text);
    }

    if (IsOpen == true)
    {
      if (ReWrite == true)
        TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Протокол сохранен в файле пользователем");

      Data << TextProtocol->toPlainText() << endl << endl;  //Передача содержимого QTextEdit в QTextStream (в содержимое файла)

      File.close();  //Завершить запись в файл

      File.setPermissions(QFile::ReadOwner | QFile::WriteOwner |
                          QFile::ReadGroup | QFile::WriteGroup |
                          QFile::ReadOther | QFile::WriteOther);
    }
  }
}


void TWinLogBook::Search(QString &str)
{
QList<QTextEdit::ExtraSelection> extraSelections;

  if(!TextProtocol->isReadOnly())
  {
    TextProtocol->moveCursor(QTextCursor::Start);
    QColor color = QColor(Qt::green).lighter(130);

    while(TextProtocol->find(str))
    {
      QTextEdit::ExtraSelection extra;
      extra.format.setBackground(color);

      extra.cursor = TextProtocol->textCursor();
      extraSelections.append(extra);
    }
  }
  TextProtocol->setExtraSelections(extraSelections);
}


void TWinLogBook::MoveToCenter()
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


void TWinLogBook::showEvent(QShowEvent *event)
{
  if(event->type() == QEvent::Show)
  {

  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TWinLogBook::hideEvent(QHideEvent *event)
{
  if( event->type() == QEvent::Hide)
  {

  }
  else
  {
    QWidget::hideEvent(event);
  }
}


void TWinLogBook::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == CurTimerLog->timerId())
  {
    double STEP;
    QDateTime t;
    QString name, fn, str, mask;

    STEP = 1.0; // /24.0/60.0/60.0*20.0;
    t = QDateTime::currentDateTime();
    name = "AbonentMKSOD";
    fn = qApp->applicationDirPath() + "/" + name;
    mask = "_dd_MM_yyyy_hh_mm_ss";
    str = t.toString(mask);

    //Если пришло время сохранить текущий лог в архивный лог
    if ((QDateTimeToTDateTime(t) - TimeStartLog) >= STEP)
    {
      //Сохранить журнал в главном LOG-файле приложения
      SaveToFile(fn + ".txt", false);

      //>>> Копирование главного LOG-файла приложения в архивный файл >>>
      if (QFile::exists(fn + str + ".txt"))
      {
        QFile::remove(fn + str + ".txt");
      }
      QFile::copy(fn + ".txt", fn + str + ".txt");
      //<<< Копирование главного LOG-файла приложения в архивный файл <<<

      //>>> Очистка главного LOG-файла приложения >>>
      QFile F;

      F.setFileName(fn + ".txt");

      if (F.open(QIODevice::WriteOnly | QIODevice::Text) == true)
        F.close();
      //<<< Очистка главного LOG-файла приложения <<<

      //>>> Очистка журнала >>>
      TextProtocol->document()->clear();

      TextProtocol->append(t.toString("dd.MM.yyyy hh:mm:ss") +
                           " Содержимое протокола автоматически сохранено в главном LOG-файле приложения");

      TextProtocol->append(t.toString("dd.MM.yyyy hh:mm:ss") + " Протокол очищен");
      //<<< Очистка журнала <<<

      // >>> Очистка каталога от старых архивных файлов >>>
      //Каталог мониторинга
      QDir Dir = QDir(qApp->applicationDirPath());

      //Получаем список файлов в каталоге
      QStringList Files = Dir.entryList(QDir::Files);

      //Цикл по всем файлам в каталоге
      for(int i=0; i<Files.size(); i++)
      {
        //Интересуют только файлы с определенной маской
        if (Files[i].contains(name + "_", Qt::CaseInsensitive))
        if (Files[i].length() == name.length() + mask.length() + 4)
        {
          //Дата и время последней модификации файла
          double LM = QDateTimeToTDateTime(QStringToQDateTime(Files[i].mid(name.length(), mask.length()), mask));

          //Если файл "старый", тогда удаляю его (остаются только 60 недавних файлов)
          if (LM > 25569.0)
          if ((TimeStartLog - LM) >= STEP*59.0)
            QFile::remove(qApp->applicationDirPath() + "/" + Files[i]);
        }
      }
      //<<< Очистка каталога от старых архивных файлов <<<

      //Отсчет интервала времени заново
      TimeStartLog = QDateTimeToTDateTime(t);
    }
  }
  else
  {
    QWidget::timerEvent(event);
  }
}


void TWinLogBook::on_action_4_triggered()
{
QString FilePathName;  //Путь и имя к текстовому файлу

  FilePathName = QFileDialog::getSaveFileName(this, QString::fromUtf8("Сохранить протокол в файле"),
                                              qApp->applicationDirPath(), QString::fromUtf8("*.txt"));
  SaveToFile(FilePathName, true);
}


void TWinLogBook::on_action_5_triggered()
{
  TextProtocol->clear();
  TextProtocol->append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + " Протокол очищен пользователем");
}


void TWinLogBook::on_action_triggered()
{  //Поиск текста
  if ((static_cast<TBase*>(OWNER))->WinFind->WhatCanIDo())
  {
    QString str;
    str = (static_cast<TBase*>(OWNER))->WinFind->Edit->text();
    Search(str);
  }
  else
  {
    QString str;
    str = "";
    Search(str);

    //Прокрутка текста вниз
    QTextCursor cursor = TextProtocol->textCursor();
    cursor.movePosition(QTextCursor::End);
    TextProtocol->setTextCursor(cursor);
  }
}


double TWinLogBook::QDateTimeToTDateTime(QDateTime Value)
{
QDateTime dt;
double res;
  dt = (QDateTime::fromString("01.01.1970 00:00:00", "dd.MM.yyyy HH:mm:ss"));
  dt.setTimeSpec(Qt::UTC);
  Value.setTimeSpec(Qt::UTC);

  if (Value.toMSecsSinceEpoch() >= dt.toMSecsSinceEpoch())
  {
    res = (static_cast<double>(Value.toMSecsSinceEpoch()))/1000.0/86400.0+25569.0;
  }
  else
  {
    res = (static_cast<double>(dt.toMSecsSinceEpoch()))/1000.0/86400.0+25569.0;
  }
  return res;
}


QDateTime TWinLogBook::QStringToQDateTime(QString Value, QString Mask)
{
QDateTime dt;
  dt = QDateTime::fromString(Value, Mask);
  dt.setTimeSpec(Qt::UTC);
  return dt;
}
