//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Error.h                                                               //
//                                                                            //
//                  *** ERROR ДИАГНОСТИЧЕСКИЕ СООБЩЕНИЯ ***                   //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef ERROR_H
#define ERROR_H

#include <QShowEvent>
#include <QDesktopWidget>

#include "../ClassLibrary/Placement.h"
#include "ui_Error.h"


class TError : public QDialog, public Ui::TError
{
  Q_OBJECT

  private slots:
    void on_pushButton_clicked();

  private:
    TPlacement* Placement;  //Положение и размеры окна сохраняет и восстанавливает
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна

    //Размещение окна по центру экрана
    void MoveToCenter(void);

    //Изменение размеров метки и окна для того, чтобы помещался выводимый текст
    void ReSize(void);

  protected:
    //Обработчик OnShow окна
    void showEvent(QShowEvent *event);

    //Обработчик OnClose окна
    void hideEvent(QHideEvent *event);

  public:
    explicit TError(QWidget *parent = 0);
    ~TError(void);

    //Простое окно с информационной текстовой строкой
    void ShowMessage(std::string str);

    //Информационное сообщение оператору (не ошибка)
    void MessageGreen(std::string str);

    //Сообщение об ошибке
    void MessageRed(std::string str);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);
};


#endif
