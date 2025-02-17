//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл CancelOK.h                                                            //
//                                                                            //
//                   *** TCANCELOK ОКНО ВЫБОРА РЕШЕНИЯ ***                    //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef CANCELOK_H
#define CANCELOK_H

#include <QShowEvent>
#include <QDesktopWidget>

#include "../ClassLibrary/Placement.h"
#include "ui_CancelOK.h"


class TCancelOK : public QDialog, public Ui::TCancelOK
{
  Q_OBJECT

  private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

  private:
    TPlacement* Placement;  //Положение и размеры окна сохраняет и восстанавливает
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна
    bool what;

    //Размещение окна по центру экрана
    void MoveToCenter(void);

  protected:
    //Обработчик OnShow окна
    void showEvent(QShowEvent *event);

    //Обработчик OnClose окна
    void hideEvent(QHideEvent *event);

  public:
    explicit TCancelOK(QWidget *parent = 0);
    ~TCancelOK(void);

    bool WhatCanIDo(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);
};


#endif
