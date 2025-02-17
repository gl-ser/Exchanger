//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinListAbonent.h                                                      //
//                                                                            //
//          *** TWINLISTABONENT ОКНО УПРАВЛЕНИЯ СПИСКОМ АБОНЕНТОВ ***         //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef WINLISTABONENT_H
#define WINLISTABONENT_H

#include <QBasicTimer>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QSqlRecord>
#include <stdio.h>

#include "../ClassLibrary/Placement.h"
#include "../EngineMKSOD/NetSendThread.h"
#include "ui_WinListAbonent.h"


class TWinListAbonent : public QMainWindow, public Ui::TWinListAbonent
{
  Q_OBJECT

  private slots:
    void on_Grid_itemSelectionChanged();  //Перемещение курсора по Grid
    void on_action_triggered();           //Добавить нового абонента
    void on_action_2_triggered();         //Редактировать параметры выбранного абонента
    void on_action_3_triggered();         //Удалить выбранного абонента
    void OnError(int Number, int Duration, QString Str);
    void OnSuccess(int Number, int Duration);

  private:
    TPlacement* Placement;              //Положение и размеры окна сохраняет и восстанавливает
    QBasicTimer* TimerCloseListServer;  //Таймер закрытия окна
    std::string FIniFileName;           //Путь и имя INI-файла, в котором хранятся настройки окна
    TNetSendThread Ping;
    bool ClearGrid;  //Признак того, что Grid очищается (перерисовывается) и на изменение currentRow() реагировать не надо
    QString CurrentAbonentName;

    //Размещение окна по центру экрана
    void MoveToCenter(void);

    //Результат посылки PING'а абоненту
    // true - отвечает; false - не отвечает
    void PingResult(bool Res);

  protected:
    //Обработчик OnShow окна
    void showEvent(QShowEvent *event);

    //Обработчик OnClose окна
    void hideEvent(QHideEvent *event);

    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

    //Метод вызывается в том случае, если надо обработать событие выбора (прокрутки) списка
    void GridScroll(void);

  public:
    //Тип описывает одного абонента
    struct Abonent
    {
      Abonent() : ID(0), name(""), address(""), port(0) {}
      int ID;
      QString name;
      QString address;
      int port;
    };
    typedef struct Abonent TAbonent;

    //Тип описывает массив абонентов
    typedef std::vector<TAbonent> TAbonents;

    explicit TWinListAbonent(QWidget *parent = 0);
    ~TWinListAbonent(void);

    void* OWNER;

    QBasicTimer* TimerIteration;  //Таймер опроса абонентов
    int Iteration;                //Итерация (её номер) опроса абонентов

    //Задает путь и имя INI-файла, в котором хранятся настройки окна
    void IniFileName(std::string Value);

    //Служебная процедура - наполнитель обработчика события showEvent
    void InShowEvent(void);

    //Восстанавливает на диске структуру каталогов пользователей
    void RestoreCatalogOfAbonent(void);

    //Возвращает список абонентов
    TAbonents GetAbonents(void);
};


#endif
