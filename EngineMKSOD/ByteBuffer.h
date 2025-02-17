//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ByteBuffer.h                                                          //
//                                                                            //
//                     *** TBYTEBUFFER БАЙТОВЫЙ БУФЕР ***                     //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <QByteArray>


class TByteBuffer
{
  private:
    QByteArray Buf;  //Содержимое буфера
    int Size;        //Размер буфера

  public:
    TByteBuffer(void);
    virtual ~TByteBuffer(void);

    //Очищает буфер
    void Clear(void);

    //Устанавливает размер буфера
    void SetSize(int Value);

    //Возвращает true, если буфер полный
    bool IsFull(void);

    //Возвращает содержимое буфера
    QByteArray Data(void);

    //Возвращает количество байт в буфере
    int Count(void);

    //Вставляет в конец буфера данные. Что влезло - то влезло
    // Возвращает количество добавленных байт
    int PushBack(QByteArray Value);
};


#endif
