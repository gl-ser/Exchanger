//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл BasicThread.h                                                         //
//                                                                            //
//                     *** TBASICTHREAD БАЗОВЫЙ ПОТОК ***                     //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef BASICTHREAD_H
#define BASICTHREAD_H

#include <QThread>
#include "Calendar.h"


class TBasicThread : public QThread
{
  Q_OBJECT

  signals:
    //Генерируется в случае возникновения ошибки в потоке
    // Возвращает:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void Error(int Number, int Duration, QString Str);

    //Генерируется в случае успешного завершения потока
    // Возвращает:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void Success(int Number, int Duration);

    //Генерируется при необходимости передачи словесного сообщения вызывающей программе
    // Возвращает:
    //   - номер потока;
    //   - текст сообщения
    void Message(int Number, QString Str);

  public:
    //Тип описывает состояние потока
    enum TStatus
    {
      NONE = 0,           //Поток НЕ запускался
      WORK = 1,           //Поток работает
      FINISH_ERROR = 2,   //Поток завершился с ошибкой
      FINISH_SUCCESS = 3  //Поток завершился успешно
    };

  private:
    int Number;          //Номер потока
    double Start, Stop;  //Время начала и время окончания работы потока
    TStatus Status;      //Состояние потока
    QString ErrorNote;   //Описание ошибки

    //Засекает время окончания работы потока
    void CheckStop(void);

  protected:
    //Засекает время начала работы потока, очищает атрибут ErrorNote
    void CheckStart(void);

    //Генерирует сигнал Error и сохраняет описание ошибки в атрибуте ErrorNote
    void TryError(int Number, QString Str);

    //Генерирует сигнал Success, очищает атрибут ErrorNote
    void TrySuccess(int Number);

    //Генерирует сигнал Message
    void TryMessage(int Number, QString Str);

  public:
    TBasicThread(void);
    virtual ~TBasicThread(void);

    //Рабочий метод потока
    virtual void run(void);

    //Устанавливает номер потока
    void SetNumber(int _Number);

    //Возвращает номер потока
    int GetNumber(void);

    //Возвращает длительность работы потока [сек]
    int GetDuration(void);

    //Возвращает состояние потока
    TStatus GetStatus(void);

    //Возвращает описание ошибки
    QString GetErrorNote(void);
};


#endif
