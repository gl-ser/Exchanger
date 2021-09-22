//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SendPartFileThread.h                                                  //
//                                                                            //
//  *** TSENDPARTFILETHREAD ПОТОК ОТПРАВКИ ЧАСТИ ФАЙЛА ПО ЛОКАЛЬНОЙ СЕТИ ***  //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#ifndef SENDPARTFILETHREAD_H
#define SENDPARTFILETHREAD_H

#include <QFile>
#include "NetSendThread.h"


class TSendPartFileThread : public TNetSendThread
{
  Q_OBJECT

  private:
    QString FilePathName;  //Путь и имя файла
    QString FileNameGUID;  //GUID являющийся временным (транспортным) именем файла (38 символов)
    QString MD5;           //Контрольная сумма файла (32 символа)
    long long FileSize;    //Размер файла
    long long StartIndex;  //Позиция в файле, с которой начинается отправляемая область (часть)
    long long BufferSize;  //Размер передаваемой области (части) файла
    int IndexPart;         //Порядковый номер передаваемой области (части) файла
    int CountParts;        //Количество областей (частей) файла

  public:
    TSendPartFileThread(void);
    ~TSendPartFileThread(void);

    //Рабочий метод потока
    void run(void);

    //Передает внутрь потока данные, необходимые для отправки области (части) файла
    // На вход подаются:
    //  - путь и имя файла;
    //  - GUID являющийся временным (транспортным) именем файла (38 символов);
    //  - контрольная сумма файла (32 символа);
    //  - размер файла;
    //  - позиция в файле, с которой начинается отправляемая область (часть);
    //  - размер передаваемой области (части) файла;
    //  - порядковый номер передаваемой области (части) файла;
    //  - количество областей (частей) файла
    void SendPartFile(QString _FilePathName, QString _GUID, QString _MD5, long long _FileSize, long long _StartIndex, long long _BufferSize,
                      int _IndexPart, int _CountParts);
};


#endif
