//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinLogBook.h                                                          //
//                                                                            //
//                      *** TWINLOGBOOK ОКНО ЖУРНАЛА ***                      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef WINLOGBOOK_H
#define WINLOGBOOK_H

#include <QLabel>
#include <QFileDialog>
#include <QBasicTimer>
#include "../ClassLibrary/Placement.h"
#include "ui_WinLogBook.h"


class TWinLogBook : public QMainWindow, public Ui::TWinLogBook
{
  Q_OBJECT

  private slots:
    void on_action_4_triggered();  //Сохранить в файле
    void on_action_5_triggered();  //Очистить журнал
    void on_action_triggered();    //Поиск текста

private:
    TPlacement* Placement;     //Положение и размеры окна сохраняет и восстанавливает
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна

    QBasicTimer* CurTimerLog;  //Таймер текущего времени
    double TimeStartLog;       //Время начала записи файла лога

    //Переводит дату и время из формата QDateTime в формат TDateTime
    // TDateTime содержит количество суток, прошедших от 00:00 30.12.1899 г
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    double QDateTimeToTDateTime(QDateTime Value);

    //Переводит строку типа Qstring в формат QDateTime, разбирая вышеупомянутую строку по заданной пользователем маске
    // Подаваемое на вход время должно быть больше 01.01.1970 г
    // Примеры масок:
    // - "dd.MM.yyyy  HH:mm:ss.zzz";
    // - "dd.MM.yyyy  HH:mm:ss";
    // - "dd.MM.yyyy"
    QDateTime QStringToQDateTime(QString Value, QString Mask);

    //Размещение окна по центру экрана
    void MoveToCenter(void);

  protected:
    //Обработчик OnShow окна
    void showEvent(QShowEvent *event);

    //Обработчик OnClose окна
    void hideEvent(QHideEvent *event);

    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

  public:
    explicit TWinLogBook(QWidget *parent = 0);
    ~TWinLogBook(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);

    //Сохраняет содержимое журнала в новом файле
    // Если ReWrite==true, тогда файл создается заново
    // Если ReWrite==false, тогда файл дописывается
    void SaveToFile(QString FilePathName, bool ReWrite);

    //Поиск текста с подсветкой
    void Search(QString& str);

    void* OWNER;
};


#endif
