//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл BasicServerTCP.cpp                                                    //
//                                                                            //
//                 *** TBASICSERVERTCP БАЗОВЫЙ СЕРВЕР TCP ***                 //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "BasicServerTCP.h"


TBasicServerTCP::TBasicServerTCP()
{
  PortTCP = 0;
  Server = new QTcpServer(this);
  ClearClients();
  TempCatalog = "";
  connect(Server, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
}


TBasicServerTCP::~TBasicServerTCP()
{
  TempCatalog = "";
  Stop();
  Server->deleteLater();
}


bool TBasicServerTCP::Start(int _PortTCP)
{
bool res;

  Stop();

  if (_PortTCP > 0)
  {
    if (TempCatalog != "")
    {
      PortTCP = _PortTCP;
      res = Server->listen(QHostAddress::AnyIPv4, static_cast<quint16>(_PortTCP));
    }
    else
    {
      res = false;
    }
  }
  else
  {
    res = false;
  }
  return res;
}


void TBasicServerTCP::Stop()
{
  Server->close();  //Чтобы отпустить порт сразу, а не ждать окончания работы потоков
  PortTCP = 0;
  ClearClients();
}


void TBasicServerTCP::SetTempCatalog(QString _TempCatalog)
{
  TempCatalog = _TempCatalog;
}


int TBasicServerTCP::GetPortTCP()
{
  return PortTCP;
}


void TBasicServerTCP::OnNewConnection()
{
  CreateThreadObject();

  Clients[Clients.size()-1]->SetNumber(static_cast<int>(Clients.size()));
  Clients[Clients.size()-1]->SetTempCatalog(TempCatalog);
  Clients[Clients.size()-1]->SetSocket(Server->nextPendingConnection());
  Clients[Clients.size()-1]->CheckStart();

  connect(Clients[Clients.size()-1], SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(Clients[Clients.size()-1], SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(Clients[Clients.size()-1], SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  Clients[Clients.size()-1]->start();
}


void TBasicServerTCP::OnError(int Number, int Duration, QString Str)
{
QString Address;
  Q_UNUSED(Duration);
  Address = Clients[static_cast<unsigned int>(FindClient(Number))]->GetAddress();
  DeleteClient(Number);

  emit ErrorServer(Str);
  emit MessageServer("IP-адрес клиента: " + Address);
}


void TBasicServerTCP::OnSuccess(int Number, int Duration)
{
QString Address;
  Q_UNUSED(Duration);
  Address = Clients[static_cast<unsigned int>(FindClient(Number))]->GetAddress();
  DeleteClient(Number);

  emit MessageServer("BasicServerTCP: Поток " + QString::number(Number) + " успешно завершен. IP-адрес клиента: " + Address);
}


void TBasicServerTCP::OnMessage(int Number, QString Str)
{
  Q_UNUSED(Number);
  emit MessageServer(Str);
}


void TBasicServerTCP::ClearClients()
{
unsigned int i;
  for(i=0; i<Clients.size(); i++)
  if (Clients[i] != NULL)
  {
    Clients[i]->quit();
    Clients[i]->wait();
    delete Clients[i];
    Clients[i] = NULL;
  }
  Clients.clear();
}


void TBasicServerTCP::DeleteClient(int Number)
{
int index1;
unsigned int i, index2;
std::vector<TNetReceiveThread*> Temp;

  index1 = FindClient(Number);
  if (index1 != -1)
  {
    index2 = static_cast<unsigned int>(index1);
    Clients[index2]->quit();
    Clients[index2]->wait();
    delete Clients[index2];

    Temp.clear();

    for(i=0; i<Clients.size(); i++)
    if (i != index2)
      Temp.push_back(Clients[i]);

    Clients.clear();

    for(i=0; i<Temp.size(); i++)
      Clients.push_back(Temp[i]);

    Temp.clear();
  }
}


int TBasicServerTCP::FindClient(int Number)
{
int res;
unsigned int i;
  res = -1;
  for(i=0; i<Clients.size(); i++)
  if (Clients[i]->GetNumber() == Number)
  {
    res = static_cast<int>(i);
    break;
  }
  return res;
}


void TBasicServerTCP::CreateThreadObject()
{
  Clients.push_back(new TNetReceiveThread());
}


QString TBasicServerTCP::GetTempCatalog()
{
  return TempCatalog;
}
