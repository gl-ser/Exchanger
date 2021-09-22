//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл NetSendThread.h                                                       //
//                                                                            //
//       *** TNETSENDTHREAD ПОТОК ОТПРАВКИ ДАННЫХ ПО ЛОКАЛЬНОЙ СЕТИ ***       //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#ifndef NETSENDTHREAD_H
#define NETSENDTHREAD_H

#ifdef __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <parsec/parsec_mac.h>
#else
#include <WinSock2.h>
#endif

#include "BasicThread.h"


class TNetSendThread : public TBasicThread
{
  Q_OBJECT

  public:
    //Тип описывает атрибуты сетевого абонента
    struct Abonent
    {
      Abonent() : Name(""), Address(""), PortTCP(0) {}
      QString Name;     //Имя абонента
      QString Address;  //IPv4-адрес абонента
      int PortTCP;      //TCP-порт абонента
    };
    typedef struct Abonent TAbonent;

  private:
    TAbonent Abonent;   //Сетевой абонент
    int Sock;           //Идентификатор создаваемого сокета
    int AnswerPortTCP;  //Номер TCP-порта для ответа (квитирования)

  protected:
    //Подключает к сетевому абоненту (удаленному компьютеру)
    bool Connect(void);

    //Отключает от сетевого абонента (удаленного компьютера)
    void Disconnect(void);

    //Передает в сокет буфер данных указанной длины (небольшой буфер)
    bool SendSmallData(const char* Buf, int Len);

    //Передает в сокет буфер данных указанной длины (большой буфер)
    bool SendBigData(const char* Buffer, long long BufferSize);

  public:
    TNetSendThread(void);
    ~TNetSendThread(void);

    //Рабочий метод потока
    void run(void);

    //Устанавливает атрибуты сетевого абонента:
    //  - имя;
    //  - IPv4-адрес;
    //  - TCP-порт
    void SetAbonent(QString _Name, QString _Address, int _PortTCP);

    //Возвращает атрибуты сетевого абонента
    TAbonent GetAbonent(void);

    //Возвращает true, если атрибуты абонента заполнены (отличны от значений по умолчанию)
    //Возвращает false, если атрибуты абонента соответствуют значениям по умолчанию
    bool IsNotEmptyAbonent(void);

    //Устанавливает номер TCP-порта для ответа (квитирования)
    void SetAnswerPortTCP(int _AnswerPortTCP);

    //Возвращает номер TCP-порта для ответа (квитирования)
    int GetAnswerPortTCP(void);
};


#endif
