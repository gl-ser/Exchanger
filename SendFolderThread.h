//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SendFolderThread.h                                                    //
//                                                                            //
//     *** TSENDFOLDERTHREAD ПОТОК ОТПРАВКИ КАТАЛОГА ПО ЛОКАЛЬНОЙ СЕТИ ***    //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#ifndef SENDFOLDERTHREAD_H
#define SENDFOLDERTHREAD_H

#include <QUuid>
#include <thread>
#include "CheckSum.h"
#include "NetSendThread.h"
#include "ZIPThread.h"
#include "File.h"
#include "SendPartFileThread.h"


class TSendFolderThread : public TNetSendThread
{
  Q_OBJECT

  private:
    QString TempCatalog;   //Каталог временных файлов
    QString CatPathName;   //Путь и имя каталога, отправляемого по локальной сети

    QString _GUID;         //Генерируется заново в рабочем методе потока, используется для формирования имени временного ZIP-файла
    QString FilePathName;  //Временный ZIP-файл, представляющий собой архив отправляемого каталога
    QString MD5;           //Контрольная сумма временного ZIP-файла, представляющего собой архив отправляемого каталога
    long long FileSize;    //Размер временного ZIP-файла, представляющего собой архив отправляемого каталога

    TZIPThread ZIP;        //Поток-архиватор

    int QProc;             //Количество доступных в ОС процессоров

    std::vector<TSendPartFileThread*> Parts;  //Массив потоков передачи файла по частям

    //Очищает массив Parts
    void ClearParts(void);

  public:
    TSendFolderThread(void);
    ~TSendFolderThread(void);

    //Рабочий метод потока
    void run(void);

    //Устанавливает каталог временных файлов
    void SetTempCatalog(QString _TempCatalog);

    //Передает внутрь потока данные, необходимые для отправки каталога по локальной сети
    // На вход подаются:
    //  - путь и имя каталога, отправляемого по локальной сети
    void SendFolder(QString _CatPathName);
};


#endif
