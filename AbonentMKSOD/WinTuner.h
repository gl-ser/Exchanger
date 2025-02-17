//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinTuner.h                                                            //
//                                                                            //
//                       *** TWINTUNER ОКНО НАСТРОЕК ***                      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef WINTUNER_H
#define WINTUNER_H

#include <QKeyEvent>
#include <QBasicTimer>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QSqlRecord>

#include "../ClassLibrary/Placement.h"
#include "ui_WinTuner.h"


class TWinTuner : public QDialog, public Ui::TWinTuner
{
  Q_OBJECT

  private slots:
    void on_OKButton_clicked();      //ОК
    void on_CancelButton_clicked();  //Отмена
    void on_FileButton_clicked();    //Открыть каталог входящих файлов
    void on_FileButton_2_clicked();  //Открыть каталог исходящих файлов
    void on_FileButton_3_clicked();  //Открыть каталог временных файлов

  private:
    std::string FIniFileName;  //Путь и имя INI-файла, в котором хранятся настройки окна
    TPlacement* Placement;     //Положение и размеры окна сохраняет и восстанавливает
    bool IsOK;                 //True, если пользователь захотел сохранить изменения в окне
    bool IsChange;             //True, если надо при визуализации окна установить значения редактируемых полей

    //По событию этого таймера снова открывается это окно, если введенные для сохранения пользователем параметры неверные
    QBasicTimer* TimerCloseWinTuner;

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
    explicit TWinTuner(QWidget *parent = 0);
    ~TWinTuner(void);

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);

    //Владелец экземпляра класса
    void* OWNER;
};


#endif
