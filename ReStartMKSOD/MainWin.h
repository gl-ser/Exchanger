//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл MainWin.h                                                             //
//                                                                            //
//            *** ГЛАВНОЕ ОКНО СРЕДСТВА ПЕРЕЗАГРУЗКИ ПРИЛОЖЕНИЯ ***           //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef MAINWIN_H
#define MAINWIN_H

#include <QKeyEvent>
#include <QBasicTimer>
#include <QProcess>
#include "../ClassLibrary/Placement.h"
#include "ui_MainWin.h"


class TMainWin : public QMainWindow, private Ui::TMainWin
{
  Q_OBJECT

  private:
    TPlacement* Placement;   //Положение и размеры окна сохраняет и восстанавливает
    QBasicTimer* TimerQuit;  //Таймер выхода из приложения
    std::string Key;
    std::string NAME;        //Название приложения

  protected:
    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

  public:
    explicit TMainWin(QWidget *parent = 0);
    ~TMainWin(void);
};


#endif
