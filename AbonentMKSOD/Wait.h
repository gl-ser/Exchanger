//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Wait.h                                                                //
//                                                                            //
//                    *** TWAIT ОКНО ЗАГРУЗКИ ПРОГРАММЫ ***                   //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef WAIT_H
#define WAIT_H

#include <QDesktopWidget>

#include "../ClassLibrary/Placement.h"
#include "ui_Wait.h"


class TWait : public QDialog, private Ui::TWait
{
  Q_OBJECT

  private:
    TPlacement* Placement;  //Положение и размеры окна сохраняет и восстанавливает
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна

    //Размещение окна по центру экрана
    void MoveToCenter(void);

  public:
    explicit TWait(QWidget *parent = 0);
    ~TWait(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);
};


#endif
