//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл NetReceiveThread.h                                                    //
//                                                                            //
//       *** TNETRECEIVETHREAD ПОТОК ПРИЕМА ДАННЫХ ИЗ ЛОКАЛЬНОЙ СЕТИ ***      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef NETRECEIVETHREAD_H
#define NETRECEIVETHREAD_H

#include <QTcpSocket>
#include <QHostAddress>
#include "BasicThread.h"
#include "ByteBuffer.h"


class TNetReceiveThread : public TBasicThread
{
  Q_OBJECT

  protected slots:
    //Обработчик события: "TCP-клиент передает информацию"
    void OnRead(void);

    //Обработчик события: "TCP-клиент отключился"
    void OnDisconnect(void);

  private:
    QTcpSocket* Socket;

  protected:
    QString TempCatalog;  //Каталог временных файлов
    QString Address;      //IPv4-адрес TCP-клиента
    TByteBuffer Buffer;   //Буфер заголовка данных

    //Выуживает IPv4-адрес, с которого пришли данные
    void SelectAddress(void);

    //Читает все данные из сокета, которые пришли в "пакете"
    QByteArray ReadData(void);

  public:
    TNetReceiveThread(void);
    ~TNetReceiveThread(void);

    //Рабочий метод потока
    void run(void);

    //Засекает время начала работы потока, очищает атрибут ErrorNote
    void CheckStart(void);

    //Устанавливает сокет, по которому будут приходить данные из локальной сети
    void SetSocket(QTcpSocket* _Socket);

    //Устанавливает каталог временных файлов
    void SetTempCatalog(QString _TempCatalog);

    //Возвращает IPv4-адрес TCP-клиента
    QString GetAddress(void);
};


#endif
