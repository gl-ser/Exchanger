//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ReceivePartFileThread.h                                               //
//                                                                            //
//    *** TRECEIVEPARTFILETHREAD ПОТОК ПРИЕМА КВИТАНЦИИ ИЛИ ЧАСТИ ФАЙЛА ***   //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#ifndef RECEIVEPARTFILETHREAD_H
#define RECEIVEPARTFILETHREAD_H

#include <QMutex>
#include "NetReceiveThread.h"
#include "File.h"


class TReceivePartFileThread : public TNetReceiveThread
{
  Q_OBJECT

  protected slots:
    //Обработчик события: "TCP-клиент передает информацию"
    void OnRead(void);

    //Обработчик события: "TCP-клиент отключился"
    void OnDisconnect(void);

  public:
    //Тип описывает заголовок получаемого файла
    struct HeaderFile
    {
      HeaderFile() : IsEmpty(true), KeyWord("<MKSOD_FILE>"), GUID(""), MD5(""), FileSize(0LL),
                     StartIndex(0LL), IndexPart(0), CountParts(0), AnswerPortTCP(0), FilePathName("") {}
      bool IsEmpty;          //Если пустой, тогда true
      QString KeyWord;       //Ключевое слово (12 байт)
      QString GUID;          //Транспортное имя файла (38 байт)
      QString MD5;           //Контрольная сумма файла (32 байта)
      long long FileSize;    //Длина файла (8 байт)
      long long StartIndex;  //Индекс начала части файла (8 байт)
      int IndexPart;         //Порядковый номер части файла (4 байта)
      int CountParts;        //Количество частей файла (4 байта)
      int AnswerPortTCP;     //Номер порта для ответа (4 байта)
      QString FilePathName;  //Путь и имя файла

      //Инициализирующий метод
      void Empty(void);
    };
    typedef struct HeaderFile THeaderFile;

    //Тип описывает заголовок получаемой квитанции
    struct HeaderKvit
    {
      HeaderKvit() : IsEmpty(true), KeyWord("<MKSOD_KVIT>"), GUID(""), Type(0), Err(0), AnswerPortTCP(0) {}
      bool IsEmpty;       //Если пустой, тогда true
      QString KeyWord;    //Ключевое слово (12 байт)
      QString GUID;       //Транспортное имя файла (38 байт)
      int Type;           //Тип квитанции (4 байта, 1 - успешно, 0 - неуспешно)
      int Err;            //Код ошибки (4 байта)
      int AnswerPortTCP;  //Номер TCP-порта для ответа (4 байта)

      //Инициализирующий метод
      void Empty(void);
    };
    typedef struct HeaderKvit THeaderKvit;

  private:
    THeaderFile FileHeader;  //Заголовок получаемого файла
    THeaderKvit KvitHeader;  //Заголовок получаемой квитанции

  public:
    TReceivePartFileThread(void);
    ~TReceivePartFileThread(void);

    //Рабочий метод потока
    void run(void);

    //Возвращает заголовок получаемого файла
    THeaderFile GetFileHeader(void);

    //Возвращает заголовок получаемой квитанции
    THeaderKvit GetKvitHeader(void);
};


#endif
