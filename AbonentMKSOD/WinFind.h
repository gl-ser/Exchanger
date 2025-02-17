//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinFind.h                                                             //
//                                                                            //
//                        *** TWINFIND ОКНО ПОИСКА ***                        //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef WINFIND_H
#define WINFIND_H

#include <QShowEvent>
#include <QDesktopWidget>

#include "../ClassLibrary/Placement.h"
#include "ui_WinFind.h"


class TWinFind : public QDialog, public Ui::TWinFind
{
  Q_OBJECT

  private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

  private:
    TPlacement* Placement;     //Положение и размеры окна сохраняет и восстанавливает
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
    explicit TWinFind(QWidget *parent = 0);
    ~TWinFind(void);

    bool WhatCanIDo(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);
};


#endif
