//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл BasicServerTCP.h                                                      //
//                                                                            //
//                 *** TBASICSERVERTCP БАЗОВЫЙ СЕРВЕР TCP ***                 //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#ifndef BASICSERVERTCP_H
#define BASICSERVERTCP_H

#include <QTcpServer>
#include "NetReceiveThread.h"


class TBasicServerTCP : public QObject
{
  Q_OBJECT

  signals:
    //Генерируется в случае возникновения ошибки
    // Возвращает:
    //   - описание ошибки
    void ErrorServer(QString Str);

    //Генерируется при необходимости передачи словесного сообщения вызывающей программе
    // Возвращает:
    //   - текст сообщения
    void MessageServer(QString Str);

  protected slots:
    //Обработчик сигнала: "Подключился новый TCP-клиент"
    void OnNewConnection(void);

    //Обработчик сигнала: "Поток завершен с ошибкой"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void OnError(int Number, int Duration, QString Str);

    //Обработчик сигнала: "Поток успешно завершен"
    // Параметры сигнала:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void OnSuccess(int Number, int Duration);

    //Обработчик сигнала: "Информационное сообщение из потока"
    // Параметры сигнала:
    //   - номер потока;
    //   - текст сообщения
    void OnMessage(int Number, QString Str);

  private:
    int PortTCP;          //Номер TCP-порта
    QTcpServer* Server;   //TCP-сервер
    QString TempCatalog;  //Каталог временных файлов

  protected:
    //Массив потоков, обрабатывающих подключенных TCP-клиентов
    //Используется при принудительной остановке сервера, когда необходимо закрыть открытые сокеты
    std::vector<TNetReceiveThread*> Clients;

    //Очищает массив Clients
    void ClearClients(void);

    //Удаляет элемент массива Clients по номеру потока
    void DeleteClient(int Number);

    //Возвращает индекс в массиве Clients по номеру потока
    // Если не нашел, возвращается (-1)
    int FindClient(int Number);

    //Создает экземпляр потока
    // Этот метод следует переопределять в потомках !!!
    virtual void CreateThreadObject(void);

    //Возвращает временный каталог
    QString GetTempCatalog(void);

  public:
    TBasicServerTCP(void);
    virtual ~TBasicServerTCP(void);

    //Запускает TCP-сервер
    // На вход подаются:
    //  - номер TCP-порта.
    // На выходе можно получить:
    //  - true в случае успешного запуска
    bool Start(int _PortTCP);

    //Останавливает TCP-сервер
    void Stop(void);

    //Устанавливает каталог временных файлов
    void SetTempCatalog(QString _TempCatalog);

    //Возвращает TCP-порт
    int GetPortTCP(void);
};


#endif
