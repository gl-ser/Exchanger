//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл NetReceiveThread.cpp                                                  //
//                                                                            //
//       *** TNETRECEIVETHREAD ПОТОК ПРИЕМА ДАННЫХ ИЗ ЛОКАЛЬНОЙ СЕТИ ***      //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "NetReceiveThread.h"


TNetReceiveThread::TNetReceiveThread()
{
  Socket = NULL;
  TempCatalog = "";
  Address = "";
  Buffer.SetSize(3);
  Buffer.Clear();
}


TNetReceiveThread::~TNetReceiveThread()
{
  TempCatalog = "";
  Address = "";
  Buffer.Clear();
  Socket->deleteLater();
}


void TNetReceiveThread::run()
{
  //Если вызываем цикл обработки сообщений, то метод run лучше ничем другим не заполнять, т.к. под Astra Linux'ом будут глюки
  exec();
}


void TNetReceiveThread::CheckStart()
{
  TBasicThread::CheckStart();
}


void TNetReceiveThread::SetSocket(QTcpSocket *_Socket)
{
  if (GetStatus() != TStatus::WORK)
  {
    Socket = _Socket;

    connect(Socket, SIGNAL(readyRead()), this, SLOT(OnRead()));
    connect(Socket, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));

    SelectAddress();
  }
}


void TNetReceiveThread::SetTempCatalog(QString _TempCatalog)
{
  if (GetStatus() != TStatus::WORK)
    TempCatalog = _TempCatalog;
}


QString TNetReceiveThread::GetAddress()
{
  return Address;
}


void TNetReceiveThread::OnRead()
{
QByteArray Array = ReadData();
  if (Buffer.IsFull() == false)
  {
    Buffer.PushBack(Array);
  }
  //Сохранить данные в файле на диске
}


void TNetReceiveThread::OnDisconnect()
{
  //Так как TCP-клиент отключился и перестал передавать информацию, поэтому
  // закрываю сокет, по которому приходили данные из локальной сети
  Socket->close();

  //Остановка цикла обработки сообщений внутри потока
  quit();

  if (GetStatus() != TStatus::FINISH_ERROR)
  {
    //Генерация сигнала "успешное завершение потока"
    TrySuccess(GetNumber());
  }
}


void TNetReceiveThread::SelectAddress()
{
  QHostAddress adr = Socket->peerAddress();
  bool conversionOK = false;
  QHostAddress ip4Address(adr.toIPv4Address(&conversionOK));
  if (conversionOK)
  {
    Address = ip4Address.toString();
  }
  else
  {
    Address = adr.toString();
  }
}


QByteArray TNetReceiveThread::ReadData()
{
QByteArray Array;

  while(Socket->bytesAvailable() > 0)
  {
    Array = Socket->readAll();
  }

  return Array;
}
