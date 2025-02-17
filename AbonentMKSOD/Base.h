//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Base.h                                                                //
//                                                                            //
//               *** TBASE ГЛАВНЫЙ МОДУЛЬ ДАННЫХ ПРИЛОЖЕНИЯ ***               //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef BASE_H
#define BASE_H

#include <random>
#include <time.h>
#include <string>
#include <QObject>
#include <QApplication>
#include <QUuid>
#include <QDir>
#include <QHostInfo>
#include <QSettings>
#include <QTimer>
#include <QFileDialog>
#include <QSqlRecord>
#include <QDateTime>

#include "About.h"
#include "Wait.h"
#include "Error.h"
#include "CancelOK.h"
#include "WinTuner.h"
#include "WinLogBook.h"
#include "WinListAbonent.h"
#include "WinAddAbonent.h"
#include "WinEditAbonent.h"
#include "WinFind.h"
#include "SQLiteDB.h"
#include "CtrlOUTCat.h"
#include "../EngineMKSOD/ReceiveFolderServer.h"


class TBase : public QObject
{
  Q_OBJECT

  private slots:
    void OnErrorServer(QString Str);
    void OnMessageServer(QString Str);
    void OnGotKvitError(TReceiveFolderServer::TAbonentExt Abonent, QString FileName, int Err);
    void OnGotKvitSuccess(TReceiveFolderServer::TAbonentExt Abonent, QString FileName);

  private:
    std::string IniFileName;  //Путь и имя INI-файла, в котором хранятся настройки

  public:
    TBase(void* Owner);
    ~TBase(void);

    //Отрисовка ProgressBar
    void HowCalc(qint64 Index, qint64 Quantity);

    //Возвращает сгенерированную случайную строку длиной PLen. Строка состоит из символов алфавита, который "прошит"
    // в самой функции. Может использоваться как генератор паролей или генератор случайного имени подкаталога для
    // размещения временных файлов текущего экземпляра приложения
    std::string RandomString(int PLen);

    //Добавляет запись в журнал
    void AddToLogBook(QString Value);

    //Запуск TCP-сервера
    void StartServer(void);

    //Останов TCP-сервера
    void StopServer(void);

    void* OWNER;

    std::string NAME, ReNAME;

    bool Result;  //Результат загрузки (true - успешно, false - закончить работу приложения)

    //Идентификатор приложения после его инсталяции. Он сохраняется в INI-файле и читается при запуске приложения
    // Если INI-файл утерян или если приложение только устанавливается (первый запуск), то этот GUID создается новый
    // То есть по сути это идентификатор установленной копии приложения
    QString _GUID_;

    bool IsReboot;    //Признак перезагрузки (true - перезагрузка, false - выключение)

    QString InCatalog;      //Каталог входящих файлов
    QString OutCatalog;     //Каталог исходящих файлов
    QString TempCatalog;    //Каталог временных файлов
    int TCP_port;           //Номер TCP-порта
    int IntervalIteration;  //Интервал автоматического опроса абонентов [сек]
    bool CheckWin;          //Автоматически открывать все окна

    TError* Error;
    TAbout* About;
    TCancelOK* CancelOK;
    TWait* Wait;
    TWinTuner* Tuner;
    TWinLogBook* Log;
    TWinListAbonent* ListAbonent;
    TWinAddAbonent* AddAbonent;
    TWinEditAbonent* EditAbonent;
    TWinFind* WinFind;
    TSQLiteDB* SQLiteDB;
    TCtrlOUTCat* CtrlOUTCat;

    TReceiveFolderServer* Server;
};


#endif
