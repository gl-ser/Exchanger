//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл SendKvitThread.cpp                                                    //
//                                                                            //
//     *** TSENDKVITTHREAD ПОТОК ОТПРАВКИ КВИТАНЦИИ ПО ЛОКАЛЬНОЙ СЕТИ ***     //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "SendKvitThread.h"


TSendKvitThread::TSendKvitThread()
{
  GUID = "";
  Type = -1;
  Err = -1;
}


TSendKvitThread::~TSendKvitThread()
{
  GUID = "";
  Type = -1;
  Err = -1;
}


void TSendKvitThread::run()
{
int p;
bool pr;

  CheckStart();

  if (GetNumber() < 0)
  {
    TryError(GetNumber(), "Не задан номер потока");
  }
  else
  {
    if (IsNotEmptyAbonent() == true)
    {
      if ((GUID != "") && (Type > -1) && (Err > -1))
      {
        //Подключиться к удаленному компьютеру
        pr = Connect();

        if (pr == true)
        {
          pr = SendSmallData(QString("<MKSOD_KVIT>").toStdString().c_str(), 12);
          //В сокет передал ключевое слово TCP-сообщения
          if (pr == true)
          {
            pr = SendSmallData(GUID.toStdString().c_str(), 38);
            //В сокет передал GUID (транспортное имя файла)
            if (pr == true)
            {
              pr = SendSmallData((char*)&Type, sizeof(int));
              //В сокет передал тип квитанции
              if (pr == true)
              {
                pr = SendSmallData((char*)&Err, sizeof(int));
                //В сокет передал код ошибки
                if (pr == true)
                {
                  p = GetAnswerPortTCP();
                  pr = SendSmallData((char*)&p, sizeof(int));
                  //В сокет передал номер порта для ответа (для получения квитанции)
                  if (pr == true)
                  {
                    pr = SendSmallData(QString("000000000000000000000000000000000000000000000000").toStdString().c_str(), 48);
                    //В сокет передал "хвост" заголовка
                  }
                }
              }
            }
          }
        }

        //Разорвать соединение с удаленным компьютером и закрыть сокет
        Disconnect();

        if (pr == true)
          TrySuccess(GetNumber());
      }
      else
      {
        TryError(GetNumber(), "Не заданы данные для отправки квитанции");
      }
    }
    else
    {
      TryError(GetNumber(), "Не заданы атрибуты сетевого абонента");
    }
  }
}


void TSendKvitThread::SendKvit(QString _GUID, int _Type, int _Err)
{
  if (GetStatus() != TStatus::WORK)
  {
    GUID = _GUID;
    Type = _Type;
    Err = _Err;
  }
}
