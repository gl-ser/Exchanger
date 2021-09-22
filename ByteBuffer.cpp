//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ByteBuffer.cpp                                                        //
//                                                                            //
//                     *** TBYTEBUFFER БАЙТОВЫЙ БУФЕР ***                     //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "ByteBuffer.h"


TByteBuffer::TByteBuffer()
{
  Clear();
  Size = 0;
}


TByteBuffer::~TByteBuffer()
{
  Clear();
  Size = 0;
}


void TByteBuffer::Clear()
{
  Buf.clear();
}


void TByteBuffer::SetSize(int Value)
{
  if (Value >= Buf.size())
  {
    Size = Value;
  }
  else
  {
    Size = Value;
    Buf.resize(Size);
  }
}


bool TByteBuffer::IsFull()
{
  if (Buf.size() >= Size)
    return true;
  else
    return false;
}


QByteArray TByteBuffer::Data()
{
  return Buf;
}


int TByteBuffer::Count()
{
  return Buf.size();
}


int TByteBuffer::PushBack(QByteArray Value)
{
int res;

  if ((Size - Buf.size()) >= Value.size())
    res = Value.size();
  else
    res = Size - Buf.size();

  Buf.push_back(Value);

  if (Buf.size() > Size)
    Buf.resize(Size);

  return res;
}
