//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SQLiteDB.h                                                            //
//                                                                            //
//                   *** TSQLITEDB КЛАСС РАБОТЫ С SQLITE ***                  //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <QApplication>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


class TSQLiteDB : public QObject
{
  Q_OBJECT
  public:
    struct Abonent
    {
      Abonent() : id(0), name(""), address(""), tcp_port(0) {}
      int id;           //Идентификатор абонента
      QString name;     //Имя абонента
      QString address;  //IP-адрес абонента
      int tcp_port;     //Номер TCP-порта
    };
    typedef struct Abonent TAbonent;

  private:
    void* OWNER;
    QString DBName;          //Путь и имя файла с локальной БД
    QString ConnectionName;  //Имя подключения к БД
    QSqlDatabase DataBase;   //Объект БД
    bool IsConnect;          //Признак того, что подключение к БД состоялось

  public:
    TSQLiteDB(void* Owner);
    ~TSQLiteDB(void);

    //Создать заново локальную БД и подключиться к ней
    void CreateDB(void);

    //Подключиться к БД
    void ConnectDB(void);

    //Возвращает признак подключения к БД
    bool GiveStatusConnect(void);

    //Возвращает имя подключения к БД
    QString GiveConnectionName(void);
};


#endif
