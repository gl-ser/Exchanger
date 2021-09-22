//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл CheckSum.cpp                                                          //
//                                                                            //
//                 *** TCHECKSUM ПОДСЧЕТ КОНТРОЛЬНОЙ СУММЫ ***                //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "CheckSum.h"


TCheckSum::TCheckSum()
{
  //Пустой конструктор
}


TCheckSum::~TCheckSum()
{
  //Пустой деструктор
}


QString TCheckSum::ByteArrayMD5(QByteArray ByteArray)
{
QString result;
QCryptographicHash cryp(QCryptographicHash::Md5);

  cryp.addData(ByteArray);
  result = cryp.result().toHex().data();

  return result;
}


QString TCheckSum::StrMD5(QString Str)
{
  return ByteArrayMD5(Str.toUtf8());
}


QString TCheckSum::FileMD5(QString FilePathName)
{
QString result;
QFile file(FilePathName);
QCryptographicHash cryp(QCryptographicHash::Md5);

  if (file.open(QIODevice::ReadOnly))
  {
    file.close();
    cryp.addData(&file);
    result = cryp.result().toHex().data();
  }
  else
  {
    result = StrMD5("");
  }
  return result;
}
