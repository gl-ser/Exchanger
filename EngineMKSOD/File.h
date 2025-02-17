//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл File.h                                                                //
//                                                                            //
//                 *** TFILE КЛАСС МЕТОДОВ РАБОТЫ С ФАЙЛОМ ***                //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <math.h>


class TFile
{
  private:
    QFile MMFile;   //Memory Mapped File
    uchar *Memory;  //Содержимое файла отображаемого в память (по объему должно влезть в память)

  public:
    //Тип описывает границы одной области файла отображаемого в память
    struct MMFileIndex
    {
      MMFileIndex() : start(0LL), stop(0LL), buf_unsigned_char(NULL), buf_const_char(NULL), size(0LL) {}
      long long start;             //Левая граница области
      long long stop;              //Правая граница области
      uchar* buf_unsigned_char;    //Указатель на начало области
      const char* buf_const_char;  //Указатель на начало области
      long long size;              //Размер области в байтах
    };
    typedef struct MMFileIndex TMMFileIndex;

    //Тип описывает вектор границ областей файла отображаемого в память
    // buf_unsigned_char и buf_const_char нулевого элемента вектора указывают на начало Memory
    // Сумма size всех элементов вектора - это размер всего Memory
    typedef std::vector<TMMFileIndex> TMMFileIndexes;

    //Тип описывает границы одной области файла. Границы устанавливаются методом seek
    struct FileIndex
    {
      FileIndex() : start(0LL), stop(0LL), size(0LL) {}
      long long start;  //Левая граница области
      long long stop;   //Правая граница области
      long long size;   //Размер области в байтах
    };
    typedef struct FileIndex TFileIndex;

    //Тип описывает вектор границ областей файла. Границы устанавливаются методом seek
    // Сумма size всех элементов вектора - это размер всего файла
    typedef std::vector<TFileIndex> TFileIndexes;

    TFile(void);
    virtual ~TFile(void);

    //Создает пустой файл заданного размера
    // На вход подаются:
    //   - путь и имя файла;
    //   - размер файла в байтах.
    // На выходе можно получить:
    //   - true в случае успешного создания файла
    static bool CreateEmptyFile(QString FilePathName, long long Size);

    //Возвращает размер файла
    // На вход подаются:
    //   - путь и имя файла.
    // На выходе можно получить:
    //   - размер файла в байтах
    static long long GetFileSize(QString FilePathName);

    //Возвращает имя файла
    // На вход подаются:
    //   - путь и имя файла.
    // На выходе можно получить:
    //   - имя файла
    static QString GetFileName(QString FilePathName);

    //Проверяет существование файла
    // На вход подаются:
    //   - путь и имя файла.
    // На выходе можно получить:
    //   - true если файл существует
    static bool FileExist(QString FilePathName);

    //Удаляет файл
    // На вход подаются:
    //   - путь и имя файла.
    // На выходе можно получить:
    //   - true в случае успеха
    static bool DelFile(QString FilePathName);

    //Удаляет все файлы из заданного каталога (сам каталог НЕ удаляется)
    // На вход подаются:
    //   - путь и имя каталога
    static void ClearDir(QString DirPathName);

    //Разрезает ранее открытый Memory Mapped File на заданное количество областей.
    // Заполняет свойство Indexes массивом границ областей файла
    // На вход подаются:
    //   - размер файла в байтах;
    //   - количество нарезаемых областей
    // На выходе можно получить:
    //   - вектор границ областей файла
    static TFileIndexes SplitFile(long long FileSize, int Quantity);

    //Сохраняет часть файла
    // На вход подаются:
    //   - путь и имя файла;
    //   - массив байт для записи;
    //   - позиция начала записи.
    // На выходе можно получить:
    //   - true в случае успеха
    static bool SavePartFile(QString FilePathName, QByteArray Array, long long StartIndex);

    //Открывает файл как Memory Mapped File
    // Если физический файл очень большой, он не может быть отображен в память
    // На вход подаются:
    //   - путь и имя файла;
    //   - true в случае только чтения.
    // На выходе можно получить:
    //   - true в случае успешного открытия файла
    bool OpenMMFile(QString FilePathName, bool ReadOnly);

    //Закрывает ранее открытый Memory Mapped File
    void CloseMMFile(void);

    //Разрезает ранее открытый Memory Mapped File на заданное количество областей.
    // Заполняет свойство Indexes массивом границ областей файла
    // На вход подаются:
    //   - количество нарезаемых областей
    // На выходе можно получить:
    //   - true в случае успешного нарезания файла
    bool SplitMMFile(int Quantity);

    //Массив границ областей файла отображаемого в память.
    // Заполняется методом SplitMMFile
    TMMFileIndexes MMFileIndexes;
};


#endif
