#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUuid>
#include <QDebug>
#include "ui_MainWindow.h"
#include "../ZIPThread.h"
#include "../File.h"
#include "../CheckSum.h"
#include "../SendPartFileThread.h"
#include "../SendFolderThread.h"
#include "../BasicServerTCP.h"
#include "../ReceiveFolderServer.h"
#include "../SendKvitThread.h"


class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

  private slots:
    //Генерируется в случае ошибки
    // Возвращает:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void OnError(int Number, int Duration, QString Str);

    //Генерируется в случае успешного завершения потока
    // Возвращает:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void OnSuccess(int Number, int Duration);

    //Генерируется при необходимости передачи словесного сообщения вызывающей программе
    // Возвращает:
    //   - номер потока;
    //   - текст сообщения
    void OnMessage(int Number, QString Str);

    void OnErrorServer(QString Str);
    void OnMessageServer(QString Str);
    void OnGotKvitError(TReceiveFolderServer::TAbonentExt Abonent, QString FileName, int Err);
    void OnGotKvitSuccess(TReceiveFolderServer::TAbonentExt Abonent, QString FileName);

    void OnSendFolderError(int Number, int Duration, QString Str);
    void OnSendFolderSuccess(int Number, int Duration);
    void OnSendFolderMessage(int Number, QString Str);

    void on_pushButton_clicked();    //Сжатие
    void on_pushButton_2_clicked();  //Распаковка
    void on_pushButton_6_clicked();  //Файл
    void on_pushButton_7_clicked();  //Отправить каталог
    void on_pushButton_8_clicked();  //Отправка
    void on_pushButton_9_clicked();  //Старт TCP-сервера
    void on_pushButton_10_clicked(); //Останов TCP-сервера
    void on_pushButton_11_clicked(); //Отправить квитанцию

private:
    TZIPThread Thread1, Thread2;
    TSendPartFileThread SFT1, SFT2, SFT3, SFT4;
    TFile File;
    TSendFolderThread FolderSend;
    TReceiveFolderServer ServerTCP;
    TSendKvitThread SK;

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(void);
};


#endif
