//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinAddAbonent.h                                                       //
//                                                                            //
//           *** TWINADDABONENT ОКНО ДОБАВЛЕНИЯ НОВОГО АБОНЕНТА ***           //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef WINADDABONENT_H
#define WINADDABONENT_H

#include <QKeyEvent>
#include <QBasicTimer>
#include <QDesktopWidget>
#include <QSqlRecord>

#ifdef __unix__
#include <QProcess>
#endif

#include "../ClassLibrary/Placement.h"
#include "ui_WinAddAbonent.h"


class TWinAddAbonent : public QDialog, public Ui::TWinAddAbonent
{
  Q_OBJECT

  private slots:
    void on_OKButton_clicked();      //ОК
    void on_CancelButton_clicked();  //Отмена

  private:
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна
    TPlacement* Placement;     //Положение и размеры окна сохраняет и восстанавливает
    bool IsOK;                 //True, если пользователь захотел сохранить изменения в окне
    bool IsChange;             //True, если надо при визуализации окна установить значения редактируемых полей

    //По событию этого таймера снова открывается это окно, если введенные для сохранения пользователем параметры неверные
    QBasicTimer* TimerCloseAddAbonent;

    //Размещение окна по центру экрана
    void MoveToCenter(void);

  protected:
    //Обработчик нажатия на клавиши
    void keyPressEvent(QKeyEvent *event);

    //Обработчик OnShow окна
    void showEvent(QShowEvent *event);

    //Обработчик OnClose окна
    void hideEvent(QHideEvent *event);

    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

  public:
    explicit TWinAddAbonent(QWidget *parent = 0);
    ~TWinAddAbonent(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);

    //Владелец экземпляра класса
    void* OWNER;
};


#endif
