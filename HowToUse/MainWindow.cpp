#include "MainWindow.h"


void MainWindow::OnError(int Number, int Duration, QString Str)
{
  TextProtocol->append("");
  TextProtocol->append("Поток № " + QString::number(Number) + " завершил работу с ошибкой:");
  TextProtocol->append("<<" + Str + ">>");
  TextProtocol->append("Поток отработал " + QString::number(Duration) + " сек");
}


void MainWindow::OnSuccess(int Number, int Duration)
{
  TextProtocol->append("");
  TextProtocol->append("Поток № " + QString::number(Number) + " успешно завершил работу");
  TextProtocol->append("Поток отработал " + QString::number(Duration) + " сек");
}


void MainWindow::OnMessage(int Number, QString Str)
{
  TextProtocol->append("Поток № " + QString::number(Number) + ": " + Str);
}


void MainWindow::OnErrorServer(QString Str)
{
  TextProtocol->append(Str);
}


void MainWindow::OnMessageServer(QString Str)
{
  TextProtocol->append(Str);
}


void MainWindow::OnGotKvitError(TReceiveFolderServer::TAbonentExt Abonent, QString FileName, int Err)
{
  TextProtocol->append("Получена КВИТАНЦИЯ об ОШИБКЕ <<" + QString::number(Err) + ">> от абонента " +
                       Abonent.Base.Address + " Файл " + FileName + " не был доставлен");
}


void MainWindow::OnGotKvitSuccess(TReceiveFolderServer::TAbonentExt Abonent, QString FileName)
{
  TextProtocol->append("Получена КВИТАНЦИЯ от абонента " + Abonent.Base.Address + " об успешной доставке файла " + FileName);
}


void MainWindow::OnSendFolderError(int Number, int Duration, QString Str)
{
  Q_UNUSED(Number);
  Q_UNUSED(Duration);
  TextProtocol->append(Str);
}


void MainWindow::OnSendFolderSuccess(int Number, int Duration)
{
  Q_UNUSED(Number);
  TextProtocol->append("Общее время отправки каталога " + QString::number(Duration) + " сек");
  TextProtocol->append(" ");
}


void MainWindow::OnSendFolderMessage(int Number, QString Str)
{
  Q_UNUSED(Number);
  TextProtocol->append(Str);
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  setupUi(this);

  connect(&Thread1, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&Thread1, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&Thread1, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  connect(&Thread2, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&Thread2, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&Thread2, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  connect(&SFT1, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&SFT1, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&SFT1, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  connect(&SFT2, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&SFT2, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&SFT2, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  connect(&SFT3, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&SFT3, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&SFT3, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  connect(&SFT4, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&SFT4, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&SFT4, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);

  connect(&FolderSend, SIGNAL(Success(int, int)), this, SLOT(OnSendFolderSuccess(int, int)), Qt::QueuedConnection);
  connect(&FolderSend, SIGNAL(Error(int, int, QString)), this, SLOT(OnSendFolderError(int, int, QString)), Qt::QueuedConnection);
  connect(&FolderSend, SIGNAL(Message(int, QString)), this, SLOT(OnSendFolderMessage(int, QString)), Qt::QueuedConnection);

  connect(&ServerTCP, SIGNAL(ErrorServer(QString)), this, SLOT(OnErrorServer(QString)), Qt::QueuedConnection);
  connect(&ServerTCP, SIGNAL(MessageServer(QString)), this, SLOT(OnMessageServer(QString)), Qt::QueuedConnection);
  connect(&ServerTCP, SIGNAL(GotKvitError(TReceiveFolderServer::TAbonentExt, QString, int)), this, SLOT(OnGotKvitError(TReceiveFolderServer::TAbonentExt, QString, int)), Qt::QueuedConnection);
  connect(&ServerTCP, SIGNAL(GotKvitSuccess(TReceiveFolderServer::TAbonentExt, QString)), this, SLOT(OnGotKvitSuccess(TReceiveFolderServer::TAbonentExt, QString)), Qt::QueuedConnection);

  connect(&SK, SIGNAL(Success(int, int)), this, SLOT(OnSuccess(int, int)), Qt::QueuedConnection);
  connect(&SK, SIGNAL(Error(int, int, QString)), this, SLOT(OnError(int, int, QString)), Qt::QueuedConnection);
  connect(&SK, SIGNAL(Message(int, QString)), this, SLOT(OnMessage(int, QString)), Qt::QueuedConnection);
}


MainWindow::~MainWindow()
{
  //Дождаться окончания работы потоков
  Thread1.quit();
  Thread1.wait();

  Thread2.quit();
  Thread2.wait();

  SFT1.quit();
  SFT1.wait();

  SFT2.quit();
  SFT2.wait();

  SFT3.quit();
  SFT3.wait();

  SFT4.quit();
  SFT4.wait();

  FolderSend.quit();
  FolderSend.wait();

  File.CloseMMFile();
}


void MainWindow::on_pushButton_clicked()
{
  Thread1.SetNumber(1);
#ifdef __unix__
  Thread1.SetPaths("//home//administrator//tmp//TEST_IN_1", "//home//administrator//tmp//TEST_IN_1.zip");
#else
  Thread1.SetPaths("D:\\WORK\\C++\\Exchanger\\TEST_IN_1", "D:\\WORK\\C++\\Exchanger\\TEST_IN_1.zip");
#endif
  Thread1.CompressDir();
  Thread1.start();

  Thread2.SetNumber(2);
#ifdef __unix__
  Thread2.SetPaths("//home//administrator//tmp//TEST_IN_2", "//home//administrator//tmp//TEST_IN_2.zip");
#else
  Thread2.SetPaths("D:\\WORK\\C++\\Exchanger\\TEST_IN_2", "D:\\WORK\\C++\\Exchanger\\TEST_IN_2.zip");
#endif
  Thread2.CompressDir();
  Thread2.start();

  TextProtocol->append("");
  TextProtocol->append("Процесс сжатия запущен");
}


void MainWindow::on_pushButton_2_clicked()
{
  TBasicThread::TStatus r = Thread1.GetStatus();
  TextProtocol->append("");
  TextProtocol->append("Статус " + QString::number(r));
  TextProtocol->append("");


  //Распаковать папку
  Thread1.SetNumber(1);
#ifdef __unix__
  Thread1.SetPaths("//home//administrator//tmp//TEST_OUT_1", "//home//administrator//tmp//TEST_IN_1.zip");
#else
  Thread1.SetPaths("D:/WORK/C++/Exchanger/Application/In",
                   "D:/WORK/C++/Exchanger/Application/TempServer/{a6602290-b895-48b6-bd87-91244d9df914}.zip");
#endif
  Thread1.ExtractDir();
  Thread1.start();

 /* Thread2.SetNumber(2);
#ifdef __unix__
  Thread2.SetPaths("//home//administrator//tmp//TEST_OUT_2", "//home//administrator//tmp//TEST_IN_2.zip");
#else
  Thread2.SetPaths("D:\\WORK\\C++\\Exchanger\\TEST_OUT_2", "D:\\WORK\\C++\\Exchanger\\TEST_IN_2.zip");
#endif
  Thread2.ExtractDir();
  Thread2.start(); */

  TextProtocol->append("");
  TextProtocol->append("Процесс распаковки запущен");
}


void MainWindow::on_pushButton_6_clicked()
{
QString FilePathName;
long long Size;
TFile::TFileIndexes Indexes;
QByteArray Data;
QFile F;

#ifdef __unix__
  FilePathName = "//home//administrator//tmp//2.txt";
#else
  //FilePathName = "D:/WORK/C++/Exchanger/Application/Temp/{d8faae34-07aa-407e-a275-8b0bcd9a47fb}.zip";
  //FilePathName = "C:/Users/Admin/Desktop/2.txt";
  FilePathName = "D:/WORK/C++/Exchanger/Application/TempClient/{c33e1b73-aca7-4bf1-b46a-5924b6fd4ffd}.zip";
#endif

  Size = TFile::GetFileSize(FilePathName);
  Indexes = TFile::SplitFile(Size, 4);

  TextProtocol->append("Файл разделен на " + QString::number(Indexes.size()) + " частей");

  F.setFileName(FilePathName);
  if (F.open(QIODevice::ReadOnly) == true)
  {
    while (!F.atEnd())
    {
      qDebug() << F.pos();
      Data = F.read(1);
      qDebug() << Data;
    }
    F.close();
    Data.clear();
  }
}


void MainWindow::on_pushButton_7_clicked()
{
QString TempCatalog, Folder;

#ifdef __unix__
  TempCatalog = "/home/administrator/WORK/Exchanger/Application/TempClient";
  Folder = "/home/administrator/WORK/Exchanger/Application/Out";
#else
  TempCatalog = "D:/WORK/C++/Exchanger/Application/TempClient";
  Folder = "D:/WORK/C++/Exchanger/Application/Out";
#endif

  FolderSend.SetNumber(10);
  //FolderSend.SetAbonent("win", "10.34.139.30", 5601);
  FolderSend.SetAbonent("linux", "192.168.44.122", 5800);
  FolderSend.SetAnswerPortTCP(5601);
  FolderSend.SetTempCatalog(TempCatalog);
  FolderSend.SendFolder(Folder);
  FolderSend.start();
}


void MainWindow::on_pushButton_8_clicked()
{
unsigned int ind;
long long FileSize;
QString FilePathName, MD5, _GUID;
TFile::TFileIndexes Indexes;

#ifdef __unix__
  FilePathName = "/home/administrator/tmp/1.txt";
#else
  FilePathName = "C:/Users/Admin/Desktop/2.txt";
  //FilePathName = "D:/WORK/C++/Exchanger/Application/Temp/{c33e1b73-aca7-4bf1-b46a-5924b6fd4ffd}.zip";
#endif

  MD5 = TCheckSum::FileMD5(FilePathName);  //Вычисление контрольной суммы файла по алгоритму MD5
  _GUID = QUuid::createUuid().toString();  //Генерация нового GUID'а

  FileSize = TFile::GetFileSize(FilePathName);

  Indexes = TFile::SplitFile(FileSize, 4);

  if (Indexes.size() >= 1)
  {
    TextProtocol->append("Файл успешно разделен на " + QString::number(Indexes.size()) + " частей");

    ind = 0;
    SFT1.SetNumber(1);
    SFT1.SetAbonent("first", "10.34.139.30", 4095);
    SFT1.SendPartFile(FilePathName, _GUID, MD5, FileSize, Indexes[ind].start, Indexes[ind].size, 1, 4);
    SFT1.start();

    ind = 1;
    SFT2.SetNumber(2);
    SFT2.SetAbonent("first", "10.34.139.30", 4095);
    SFT2.SendPartFile(FilePathName, _GUID, MD5, FileSize, Indexes[ind].start, Indexes[ind].size, 2, 4);
    SFT2.start();

    ind = 2;
    SFT3.SetNumber(3);
    SFT3.SetAbonent("first", "10.34.139.30", 4095);
    SFT3.SendPartFile(FilePathName, _GUID, MD5, FileSize, Indexes[ind].start, Indexes[ind].size, 3, 4);
    SFT3.start();

    ind = 3;
    SFT4.SetNumber(4);
    SFT4.SetAbonent("first", "10.34.139.30", 4095);
    SFT4.SendPartFile(FilePathName, _GUID, MD5, FileSize, Indexes[ind].start, Indexes[ind].size, 4, 4);
    SFT4.start();
  }
  else
  {
    TextProtocol->append("Ошибка разделения файла");
  }
  Indexes.clear();
}


void MainWindow::on_pushButton_9_clicked()
{
bool res;
QString TempCatalog, Folder;

#ifdef __unix__
  TempCatalog = "/home/administrator/WORK/Exchanger/Application/TempServer";
  Folder = "/home/administrator/WORK/Exchanger/Application/In";
#else
  TempCatalog = "D:/WORK/C++/Exchanger/Application/TempServer";
  Folder = "D:/WORK/C++/Exchanger/Application/In";
#endif

  ServerTCP.SetTempCatalog(TempCatalog);
  ServerTCP.SetTimeOut(0.2);
  ServerTCP.AddAbonent("windows", "10.34.139.30", 5601, Folder+"/win");
  ServerTCP.AddAbonent("linux", "192.168.44.122", 5800, Folder+"/linux");
  res = ServerTCP.Start(5601);
  if (res == true)
  {
    TextProtocol->append("TCP-сервер успешно запущен");
    TextProtocol->append("Открыт TCP-порт № " + QString::number(ServerTCP.GetPortTCP()));
    TextProtocol->append("");
  }
  else
  {
    TextProtocol->append("Ошибка запуска TCP-сервера");
    TextProtocol->append("");
  }
}


void MainWindow::on_pushButton_10_clicked()
{
  ServerTCP.Stop();
  TextProtocol->append("");
  TextProtocol->append("TCP-сервер остановлен");
}


void MainWindow::on_pushButton_11_clicked()
{
  SK.SetNumber(111);
  SK.SetAbonent("first", "10.34.139.30", 5601);
  SK.SetAnswerPortTCP(5601);
  SK.SendKvit("{c33e1b73-aca7-4bf1-b46a-5924b6fd4ffd}", 0, 4);
  SK.start();
}
