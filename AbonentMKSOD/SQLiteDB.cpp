//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SQLiteDB.cpp                                                          //
//                                                                            //
//                   *** TSQLITEDB КЛАСС РАБОТЫ С SQLITE ***                  //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "SQLiteDB.h"
#include "Base.h"


TSQLiteDB::TSQLiteDB(void* Owner)
{
  OWNER = Owner;
  DBName = qApp->applicationDirPath()+"/AbonentMKSOD.db";
  IsConnect = false;
  ConnectionName = "";

  if (QFile(DBName).exists() == true)
  {  //Если файл с локальной БД существует
    ConnectDB();
  }
  else
  {  //Если нет файла с локальной БД
    CreateDB();
  }
}


TSQLiteDB::~TSQLiteDB(void)
{
  if (DataBase.isOpen() == true)
    DataBase.close();

  OWNER = NULL;
  DBName = "";
  IsConnect = false;
  ConnectionName = "";
}


void TSQLiteDB::CreateDB()
{
bool res;
QFile File;
QString str;
  //Задать имя файла локальной БД
  File.setFileName(DBName);

  //Создать файл локальной БД
  if (File.open(QIODevice::WriteOnly | QIODevice::Text) == true)
  {
    File.close();
    File.setPermissions(QFile::ReadOwner | QFile::WriteOwner |
                        QFile::ReadGroup | QFile::WriteGroup |
                        QFile::ReadOther | QFile::WriteOther);
    (static_cast<TBase*>(OWNER))->Error->MessageGreen("Создана новая локальная БД");
  }

  //Подключиться к БД
  ConnectDB();

  if (IsConnect == true)
  {
    QSqlDatabase db = QSqlDatabase::database(ConnectionName);
    if ((db.isValid() == true) && (db.isOpen() == true))
    {
      QSqlQuery Query1(db);

      //Поле status содержит состояние абонента.
      // Может принимать следующие значения:
      // 0 - неизвестно
      // 1 - доступен
      // 2 - недоступен

      str = "create table abonent (id integer primary key autoincrement, name text, "
            "address text, tcp_port integer, status integer)";
      res = Query1.exec(str);
      if (res == false)
      {
        (static_cast<TBase*>(OWNER))->Error->MessageRed("Ошибка создания таблицы абонентов. Причина: " +
                                                        Query1.lastError().text().toStdString());
      }
    }
  }
}


void TSQLiteDB::ConnectDB()
{
QString str;
  //Задать имя подключения к БД
  ConnectionName = (static_cast<TBase*>(OWNER))->_GUID_+"SQLITEDB";

  DataBase = QSqlDatabase::addDatabase("QSQLITE", ConnectionName);
  DataBase.setHostName("SQLITEDB");
  DataBase.setDatabaseName(DBName);

  DataBase.close();

  if (DataBase.isValid() == true)
  {
    if (DataBase.open() == true)
    {
      IsConnect = true;

      QSqlDatabase db = QSqlDatabase::database(ConnectionName);
      if ((db.isValid() == true) && (db.isOpen() == true))
      {
        QSqlQuery Query1(db);

        str = "update abonent set status = 0";
        Query1.exec(str);
      }
    }
    else
    {
      IsConnect = false;
      (static_cast<TBase*>(OWNER))->Error->MessageRed("Подключение к БД не произведено "+DataBase.lastError().text().toStdString());
    }
  }
  else
  {
    IsConnect = false;
    (static_cast<TBase*>(OWNER))->Error->MessageRed("Не загружен драйвер SQLite. Подключение к БД не произведено");
  }
}


bool TSQLiteDB::GiveStatusConnect()
{
  return IsConnect;
}


QString TSQLiteDB::GiveConnectionName()
{
  return ConnectionName;
}
