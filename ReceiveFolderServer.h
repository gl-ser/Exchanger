//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ReceiveFolderServer.h                                                 //
//                                                                            //
//            *** TRECEIVEFOLDERSERVER СЕРВЕР ПРИЕМА КАТАЛОГА, ***            //
//                   *** ОБРАБОТКИ И ОТПРАВКИ КВИТАНЦИИ ***                   //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#ifndef RECEIVEFOLDERSERVER_H
#define RECEIVEFOLDERSERVER_H

#include <QTcpServer>
#include <QBasicTimer>
#include <QTimerEvent>
#include "BasicServerTCP.h"
#include "CheckSum.h"
#include "ZIPThread.h"
#include "SendKvitThread.h"
#include "ReceivePartFileThread.h"


class TZIPThread2 : public TZIPThread
{
  Q_OBJECT
  private:
    QString Address;
    int AnswerPortTCP;
  public:
    TZIPThread2(void);
    ~TZIPThread2(void);
    void SetAddress(QString _Address);
    QString GetAddress(void);
    void SetAnswerPortTCP(int _AnswerPortTCP);
    int GetAnswerPortTCP(void);
};


class TReceiveFolderServer : public TBasicServerTCP
{
  Q_OBJECT

  public:
    //Тип описывает атрибуты расширенного сетевого абонента
    struct AbonentExt
    {
      AbonentExt() : FolderPathName("") {}
      TNetSendThread::TAbonent Base;  //Базовые атрибуты абонента
      QString FolderPathName;         //Путь и имя приемной папки абонента
    };
    typedef struct AbonentExt TAbonentExt;

  signals:
    //Генерируется при получении по сети квитанции об ошибке передачи каталога
    // Возвращает:
    //   - сетевой абонент, от которого пришла квитанция;
    //   - транспортное имя файла;
    //   - код ошибки.
    // Коды ошибок:
    //   - 1 - ошибка распаковки ZIP-архива;
    //   - 2 - тайм-аут приема файла;
    //   - 3 - неверная контрольная сумма;
    //   - 4 - одна или несколько частей файла были получены с ошибкой
    void GotKvitError(TReceiveFolderServer::TAbonentExt Abonent, QString FileName, int Err);

    //Генерируется при получении по сети квитанции об успешной передаче каталога
    // Возвращает:
    //   - сетевой абонент, от которого пришла квитанция;
    //   - транспортное имя файла
    void GotKvitSuccess(TReceiveFolderServer::TAbonentExt Abonent, QString FileName);

  protected slots:
    //Обработчик сигнала: "Поток ПРИЕМА ДАННЫХ ОТ TCP-КЛИЕНТА завершен с ошибкой"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void OnError(int Number, int Duration, QString Str);

    //Обработчик сигнала: "Поток ПРИЕМА ДАННЫХ ОТ TCP-КЛИЕНТА успешно завершен"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void OnSuccess(int Number, int Duration);

    //Обработчик сигнала: "Поток РАЗАРХИВИРОВАНИЯ завершен с ошибкой"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void OnZIPError(int Number, int Duration, QString Str);

    //Обработчик сигнала: "Поток РАЗАРХИВИРОВАНИЯ успешно завершен"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void OnZIPSuccess(int Number, int Duration);

    //Обработчик сигнала: "Поток ОТПРАВКИ КВИТАНЦИИ завершен с ошибкой"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void OnKvitError(int Number, int Duration, QString Str);

    //Обработчик сигнала: "Поток ОТПРАВКИ КВИТАНЦИИ успешно завершен"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void OnKvitSuccess(int Number, int Duration);

  private:
    //Тип описывает часть принятого транспортного файла, ожидающего распаковку
    struct HeaderFile
    {
      HeaderFile() : IsOk(false), DateTime(0.0), Address("") {}
      bool IsOk;        //true, если получение части транспортного файла было успешным
      double DateTime;  //Дата и время, когда были получены данные
      QString Address;  //IPv4-адрес, с которого были получены данные
      TReceivePartFileThread::THeaderFile Header;
    };
    typedef struct HeaderFile THeaderFile;

    //Тип описывает заголовок полученной квитанции
    struct HeaderKvit
    {
      HeaderKvit() : Address("") {}
      QString Address;  //IPv4-адрес, с которого были получены данные
      TReceivePartFileThread::THeaderKvit Header;
    };
    typedef struct HeaderKvit THeaderKvit;

    //Отправляет квитанцию
    // На вход подаются:
    //  - имя абонента;
    //  - IPv4-адрес абонента, которому адресуется квитанция;
    //  - TCP-порт;
    //  - транспортное имя файла;
    //  - тип квитанции (1 - успешно, 0 - неуспешно);
    //  - код ошибки.
    // Коды ошибок:
    //  - 1 - ошибка распаковки ZIP-архива;
    //  - 2 - тайм-аут приема файла;
    //  - 3 - неверная контрольная сумма;
    //  - 4 - одна или несколько частей файла были получены с ошибкой
    void SendKvit(QString Name, QString Address, int PortTCP, QString FileName, int Type, int Err);

    //Считывает заголовок транспортного файла и сохраняет его в массиве Files
    // На вход подаются:
    //  - номер потока;
    //  - true, если получение части транспортного файла было успешным
    void ReadHeader(int Number, bool IsOk);

    //Анализирует массив Files
    void AnalyseFiles(void);

    //Удаление из массива Files элементов по GUID
    void DeleteFromFiles(QString GUID);

    //Возвращает количество успешно принятых частей файла по GUID
    int CountParts(QString GUID);

    //Возвращает количество всех принятых частей файла по GUID (успешных и неуспешных)
    int CountPartsAll(QString GUID);

    //Очищает массив ZIPs
    void ClearZIPs(void);

    //Удаляет элемент массива ZIPs по номеру потока
    void DeleteZIPs(int Number);

    //Возвращает индекс в массиве ZIPs по номеру потока
    // Если не нашел, возвращается (-1)
    int FindZIPs(int Number);

    //Очищает массив Kvits
    void ClearKvits(void);

    //Удаляет элемент массива Kvits по номеру потока
    void DeleteKvits(int Number);

    //Возвращает индекс в массиве Kvits по номеру потока
    // Если не нашел, возвращается (-1)
    int FindKvits(int Number);

    //Возвращает индекс в массиве Abonents по IPv4-адресу
    // На вход подаются:
    //  - IPv4-адрес абонента;
    //  - номер TCP-порта для ответа (квитирования)
    int FindAbonent(QString _Address, int _AnswerPortTCP);

    std::vector<THeaderFile> Files;       //Принятые транспортные файлы, ожидающие распаковку
    std::vector<TAbonentExt> Abonents;    //Массив сетевых абонентов
    QBasicTimer* CurTimer;                //Таймер текущего времени
    std::vector<TZIPThread2*> ZIPs;       //Потоки для разархивирования принятых файлов
    std::vector<TSendKvitThread*> Kvits;  //Потоки для отправки квитанций
    double TimeOut;                       //Тайм-аут приема файла [мин]

  protected:
    //Создает экземпляр потока
    // Этот метод следует переопределять в потомках !!!
    void CreateThreadObject(void);

    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

  public:
    TReceiveFolderServer(void);
    virtual ~TReceiveFolderServer(void);

    //Запускает TCP-сервер
    // На вход подаются:
    //  - номер TCP-порта.
    // На выходе можно получить:
    //  - true в случае успешного запуска
    bool Start(int _PortTCP);

    //Останавливает TCP-сервер
    void Stop(void);

    //Добавляет нового сетевого абонента в массив
    // На вход подаются:
    //  - имя абонента;
    //  - IPv4-адрес абонента;
    //  - TCP-порт;
    //  - путь и имя приемной папки абонента
    void AddAbonent(QString _Name, QString _Address, int _PortTCP, QString _FolderPathName);

    //Задает тайм-аут приема файла
    // На вход подаются:
    //  - временная задержка [мин];
    void SetTimeOut(double Value);
};


#endif
