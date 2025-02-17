//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл ZIPThread.h                                                           //
//                                                                            //
//              *** TZIPTHREAD ПОТОК РАБОТЫ С АРХИВАТОРОМ ZIP ***             //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef ZIPTHREAD_H
#define ZIPTHREAD_H

#include "BasicThread.h"
#include "../HeaderDLL/JlCompress.h"


class TZIPThread : public TBasicThread
{
  Q_OBJECT

  private:
    //Тип описывает виды задач на архивацию
    enum TTask
    {
      NONE = 0,          //Задача НЕ установлена
      COMPRESS_DIR = 1,  //Сжатие каталога в ZIP-архив
      EXTRACT_DIR = 2    //Распаковка ZIP-архива в каталог
    };

    QString DirPathName;      //Путь и имя каталога для сжатия\распаковки
    QString ZIPFilePathName;  //Путь и имя ZIP-файла для сжатия\распаковки
    TTask Task;               //Задача архиватору

  public:
    TZIPThread(void);
    ~TZIPThread(void);

    //Рабочий метод потока
    void run(void);

    //Устанавливает для сжатия\распаковки следующие атрибуты:
    //  - путь и имя каталога;
    //  - путь и имя ZIP-файла
    void SetPaths(QString _DirPathName, QString _ZIPFilePathName);

    //Ставит задачу сжатия каталога в ZIP-архив
    void CompressDir(void);

    //Ставит задачу распаковки ZIP-архива в каталог
    void ExtractDir(void);

    //Возвращает путь и имя каталога
    QString GetDirPathName(void);

    //Возвращает путь и имя ZIP-файла
    QString GetZIPFilePathName(void);
};


#endif
