//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл About.h                                                               //
//                                                                            //
//                 *** TABOUT ОКНО ИНФОРМАЦИИ О ПРОГРАММЕ ***                 //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef ABOUT_H
#define ABOUT_H

#include <QDesktopWidget>

#include "../ClassLibrary/Placement.h"
#include "ui_About.h"


class TAbout : public QDialog, private Ui::TAbout
{
  Q_OBJECT

  private slots:
    void on_pushButton_clicked();

  private:
    TPlacement* Placement;  //Положение и размеры окна сохраняет и восстанавливает
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна

    //Размещение окна по центру экрана
    void MoveToCenter(void);

  public:
    explicit TAbout(QWidget *parent = 0);
    ~TAbout(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);
};


#endif
