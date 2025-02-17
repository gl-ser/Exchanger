//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл MainWin.h                                                             //
//                                                                            //
//                 *** TMAINWIN ГЛАВНОЕ ОКНО АБОНЕНТА ИСИО ***                //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QLabel>
#include <QDateTime>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QBasicTimer>
#include <QProcess>

#include "Base.h"
#include "../ClassLibrary/Placement.h"
#include "ui_MainWin.h"


class TMainWin : public QMainWindow, public Ui::TMainWin
{
  Q_OBJECT

  private slots:
    void on_action_triggered();     //Параметры приложения
    void on_action_3_triggered();   //Перезагрузить приложение
    void on_action_4_triggered();   //Выход из программы
    void on_action_2_triggered();   //Открыть журнал
    void on_action_14_triggered();  //О программе
    void on_action_5_triggered();   //Управление списком абонентов

  private:
    QLabel* CurTimeText;           //Место на главном окне, в которое выводятся показания текущего времени
    QLabel* Port;                  //Место на главном окне, в которое выводится номер порта
    QBasicTimer* CurTimeTimer;     //Таймер текущего времени
    QBasicTimer* TimerQuit;        //Таймер выхода из приложения
    QBasicTimer* TimerFirstStart;  //Таймер срабатывает при первом старте приложения
    TPlacement* Placement;         //Положение и размеры окна сохраняет и восстанавливает

    //Путь и имя INI-файла, в котором хранятся настройки окна
    std::string FIniFileName;

    //Размещение окна по центру экрана
    void MoveToCenter(void);

  protected:
    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

  public:
    explicit TMainWin(QWidget *parent = 0);
    ~TMainWin(void);

    //Объект главного модуля данных приложения
    TBase *Base;

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);

    //Запуск таймера TimerFirstStart
    void FirstStart(void);

    //Вывести номер порта на главном окне
    void Set_Port(QString Value);

    std::string ReNAME;
};


#endif
