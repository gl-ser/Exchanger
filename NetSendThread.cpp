//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл NetSendThread.cpp                                                     //
//                                                                            //
//       *** TNETSENDTHREAD ПОТОК ОТПРАВКИ ДАННЫХ ПО ЛОКАЛЬНОЙ СЕТИ ***       //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "NetSendThread.h"


TNetSendThread::TNetSendThread()
{
  Abonent.Name = "";
  Abonent.Address = "";
  Abonent.PortTCP = 0;

  Sock = 0;
  AnswerPortTCP = 0;

  #ifndef __unix__
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    //qDebug() << "ERROR";
  }
  #endif
}


TNetSendThread::~TNetSendThread()
{
  #ifndef __unix__
  WSACleanup();
  #endif

  Abonent.Name = "";
  Abonent.Address = "";
  Abonent.PortTCP = 0;

  Sock = 0;
  AnswerPortTCP = 0;
}


void TNetSendThread::run()
{
bool pr;
  CheckStart();

  if (GetNumber() < 0)
  {
    TryError(GetNumber(), "Не задан номер потока");
  }
  else
  {
    if (IsNotEmptyAbonent() == true)
    {
      //Подключиться к удаленному компьютеру
      pr = Connect();

      if (pr == true)
      {
        //Do something...
      }

      //Разорвать соединение с удаленным компьютером и закрыть сокет
      Disconnect();

      if (pr == true)
        TrySuccess(GetNumber());
    }
    else
    {
      TryError(GetNumber(), "Не заданы атрибуты сетевого абонента");
    }
  }
}


bool TNetSendThread::Connect()
{
QString str;
bool err;
struct sockaddr_in server;
#ifdef __unix__
parsec_mac_label_t label;
#endif

  if (IsNotEmptyAbonent() == true)
  {
    err = false;
    str = "";

    //Создание сокета
    Sock = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0));
    if (Sock < 0)
    {
      str = "Ошибка создания сокета.    Имя абонента: " + GetAbonent().Name + "    IP-адрес: " + GetAbonent().Address +
            "    TCP-порт: " + QString::number(GetAbonent().PortTCP);
      err = true;
    }

    #ifdef __unix__
    //Убрать из TCP-пакетов мандатную метку
    label.type = MAC_ATTR_IGNORE;
    label.mac.lev = 0;
    label.mac.cat = 0;

    if (err == false)
    if (parsec_fchmac(Sock, &label) != 0)
    {
      str = "Ошибка управления мандатной меткой.    Имя абонента: " + GetAbonent().Name + "    IP-адрес: " + GetAbonent().Address +
            "    TCP-порт: " + QString::number(GetAbonent().PortTCP);
      err = true;
    }
    #endif

    server.sin_family = AF_INET;
    server.sin_port = htons(static_cast<quint16>(GetAbonent().PortTCP));

    //Преобразование адреса IPv4 из текста в двоичную форму
    #ifdef __unix__
    if (err == false)
    if(inet_pton(AF_INET, GetAbonent().Address.toStdString().c_str(), &server.sin_addr) <= 0)
    {
      str = "Ошибка преобразования IP-адреса.    Имя абонента: " + GetAbonent().Name + "    IP-адрес: " + GetAbonent().Address +
            "    TCP-порт: " + QString::number(GetAbonent().PortTCP);
      err = true;
    }
    #else
    if (err == false)
    {
      server.sin_addr.S_un.S_addr = inet_addr(GetAbonent().Address.toStdString().c_str());
    }
    #endif

    //Подключиться к удаленному компьютеру
    if (err == false)
    if (::connect(static_cast<unsigned int>(Sock), (struct sockaddr *)&server, sizeof(server)) < 0)
    {
      str = "Ошибка TCP-подключения.    Имя абонента: " + GetAbonent().Name + "    IP-адрес: " + GetAbonent().Address +
            "    TCP-порт: " + QString::number(GetAbonent().PortTCP);
      err = true;
    }

    if (err == true)
    {  //Соединение неуспешно
      TryError(GetNumber(), str);
      return false;
    }
    else
    {  //Соединение успешно
      return true;
    }
  }
  else
  {
    str = "Не заданы атрибуты сетевого абонента";
    TryError(GetNumber(), str);
    return false;
  }
}


void TNetSendThread::Disconnect()
{
  //Разорвать соединение с удаленным компьютером и закрыть сокет
  shutdown(static_cast<unsigned int>(Sock), 2);
  #ifdef __unix__
  close(Sock);
  #else
  closesocket(static_cast<unsigned int>(Sock));
#endif
}


bool TNetSendThread::SendSmallData(const char *Buf, int Len)
{
int total, n;
bool res;
QString str;

  total = 0;
  res = true;
  while (total < Len)
  {
    n = send(static_cast<unsigned int>(Sock), Buf+total, Len-total, 0);

    if (n == -1)
    {
      res = false;
      str = "Ошибка передачи данных. Имя абонента: " + GetAbonent().Name;
      TryError(GetNumber(), str);
      break;
    }

    total = total + n;
  }
  return res;
}


bool TNetSendThread::SendBigData(const char* Buffer, long long BufferSize)
{
bool pr;
  pr = true;

  while (BufferSize > 10000000LL)
  {
    pr = SendSmallData(Buffer, 10000000);
    if (pr == true)
    {
      Buffer = Buffer + 10000000;
      BufferSize = BufferSize - 10000000LL;
    }
    else
    {
      break;
    }
  }

  if (pr == true)
    pr = SendSmallData(Buffer, static_cast<int>(BufferSize));

  return pr;
}


void TNetSendThread::SetAbonent(QString _Name, QString _Address, int _PortTCP)
{
  if (GetStatus() != TStatus::WORK)
  {
    Abonent.Name = _Name;
    Abonent.Address = _Address;
    Abonent.PortTCP = _PortTCP;
  }
}


TNetSendThread::TAbonent TNetSendThread::GetAbonent()
{
  return Abonent;
}


bool TNetSendThread::IsNotEmptyAbonent()
{
bool res;

  if ((Abonent.Name != "") && (Abonent.Address != "") && (Abonent.PortTCP > 0))
    res = true;
  else
    res = false;

  return res;
}


void TNetSendThread::SetAnswerPortTCP(int _AnswerPortTCP)
{
  if (GetStatus() != TStatus::WORK)
    AnswerPortTCP = _AnswerPortTCP;
}


int TNetSendThread::GetAnswerPortTCP()
{
  return AnswerPortTCP;
}
