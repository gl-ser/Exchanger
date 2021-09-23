# Exchanger
Многопоточная кроссплатформенная система обмена данными (МКСОД)

МКСОД построена по принципу "точка-точка", предназначена для взаимодействия между пользователями ОС Windows и ОС Astra Linux, работающими под различными мандатными метками.

Для работы под Astra Linux на всех уровнях доступа Exchanger требует, чтобы запускающему её пользователю были выставлены 3 следующие привилегии:

- parsec_cap_priv_sock
- parsec_cap_mac_sock
- parsec_cap_chmac

Использует функционал:

- Qt 5;
- сторонней библиотеки zlib;
- сторонней библиотеки quazip;
- класса статических методов конвертации времени [Calendar](https://github.com/gl-ser/Calendar).

Состоит из 13 классов: TByteBuffer, TFile, TCheckSum, TBasicThread, TZIPThread, TNetSendThread, TNetReceiveThread, TReceivePartFileThread, TSendPartFileThread, TSendKvitThread, TBasicServerTCP, TSendFolderThread, TReceiveFilderServer.

---

В подкаталоге HowToUse находится тестовая программа, демонстрирующая работу с МКСОД

Использует функционал:

- Qt 5;
- сторонней библиотеки zlib;
- сторонней библиотеки quazip;
- класса статических методов конвертации времени [Calendar](https://github.com/gl-ser/Calendar).
- МКСОД [Exchanger](https://github.com/gl-ser/Exchanger).

---

В подкаталоге QuaZIP находится сторонняя объектно-ориентированная библиотека ZIP-архиватор quazip

Использует функционал:

- Qt 5;
- сторонней библиотеки zlib.

---

В подкаталоге ZLib находится сторонняя библиотека ZIP-архиватор zlib

---

### 1. TByteBuffer
Байтовый буфер

#### Публичные методы
1.1. _Clear_ очищает буфер

Синтаксис:
```cpp
void Clear(void)
```

1.2. _SetSize_ устанавливает размер буфера

Синтаксис:
```cpp
void SetSize(int Value)
```

1.3. _IsFull_ возвращает true, если буфер полный

Синтаксис:
```cpp
bool IsFull(void)
```

1.4. _Data_ возвращает содержимое буфера

Синтаксис:
```cpp
QByteArray Data(void)
```

1.5. _Count_ возвращает количество байт в буфере

Синтаксис:
```cpp
int Count(void)
```

1.6. _PushBack_ вставляет в конец буфера данные. Что влезло - то влезло. Возвращает количество добавленных байт

Синтаксис:
```cpp
int PushBack(QByteArray Value)
```

---

### 2. TFile
Работа с файлом

#### Публичные типы
```cpp
//Тип описывает границы одной области файла отображаемого в память
struct MMFileIndex
{
  MMFileIndex() : start(0LL), stop(0LL), buf_unsigned_char(NULL),
                  buf_const_char(NULL), size(0LL) {}
  long long start;             //Левая граница области
  long long stop;              //Правая граница области
  uchar* buf_unsigned_char;    //Указатель на начало области
  const char* buf_const_char;  //Указатель на начало области
  long long size;              //Размер области в байтах
};
typedef struct MMFileIndex TMMFileIndex;


//Тип описывает вектор границ областей файла отображаемого в память
// buf_unsigned_char и buf_const_char нулевого элемента вектора 
// указывают на начало Memory
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
```

#### Публичные методы
2.1. _CreateEmptyFile_ создает пустой файл заданного размера

На вход подаются:

  - путь и имя файла;
  - размер файла в байтах.

На выходе можно получить:

  - true в случае успешного создания файла

Синтаксис:
```cpp
static bool CreateEmptyFile(QString FilePathName, long long Size)
```

2.2. _GetFileSize_ возвращает размер файла

На вход подаются:

   - путь и имя файла.

На выходе можно получить:

   - размер файла в байтах

Синтаксис:
```cpp
static long long GetFileSize(QString FilePathName)
```

2.3. _GetFileName_ возвращает имя файла
    
На вход подаются:

   - путь и имя файла.

На выходе можно получить:

   - имя файла

Синтаксис:
```cpp
static QString GetFileName(QString FilePathName)
```

2.4. _FileExist_ проверяет существование файла

На вход подаются:

   - путь и имя файла.

На выходе можно получить:
   - true если файл существует

Синтаксис:
```cpp
static bool FileExist(QString FilePathName)
```

2.5. _DelFile_ удаляет файл
    
На вход подаются:

   - путь и имя файла.

На выходе можно получить:

   - true в случае успеха

Синтаксис:
```cpp
static bool DelFile(QString FilePathName)
```

2.6. _ClearDir_ удаляет все файлы из заданного каталога (сам каталог НЕ удаляется)
    
На вход подаются:
   - путь и имя каталога

Синтаксис:
```cpp
static void ClearDir(QString DirPathName)
```

2.7. _SplitFile_ разрезает ранее открытый Memory Mapped File на заданное количество областей.
 
Заполняет свойство Indexes массивом границ областей файла

На вход подаются:

   - размер файла в байтах;
   - количество нарезаемых областей

На выходе можно получить:

   - вектор границ областей файла

Синтаксис:
```cpp
static TFileIndexes SplitFile(long long FileSize, int Quantity)
```

2.8. _SavePartFile_ сохраняет часть файла
    
На вход подаются:

   - путь и имя файла;
   - массив байт для записи;
   - позиция начала записи.

На выходе можно получить:

   - true в случае успеха

Синтаксис:
```cpp
static bool SavePartFile(QString FilePathName, QByteArray Array, long long StartIndex)
```

2.9. _OpenMMFile_ открывает файл как Memory Mapped File

Если физический файл очень большой, он не может быть отображен в память

На вход подаются:

   - путь и имя файла;
   - true в случае только чтения.

На выходе можно получить:

   - true в случае успешного открытия файла

Синтаксис:
```cpp
bool OpenMMFile(QString FilePathName, bool ReadOnly)
```

2.10. _CloseMMFile_ закрывает ранее открытый Memory Mapped File

Синтаксис:
```cpp
void CloseMMFile(void)
```

2.11. _SplitMMFile_ разрезает ранее открытый Memory Mapped File на заданное количество областей.
    
Заполняет свойство Indexes массивом границ областей файла

На вход подаются:

   - количество нарезаемых областей

На выходе можно получить:

   - true в случае успешного нарезания файла

Синтаксис:
```cpp
bool SplitMMFile(int Quantity)
```

#### Публичное свойство

2.12. _MMFileIndexes_ массив границ областей файла отображаемого в память.
    
Заполняется методом SplitMMFile

Синтаксис:
```cpp
TMMFileIndexes MMFileIndexes
```

---

### 3. TCheckSum
Подсчет контрольной суммы

---

### 4. TBasicThread
Базовый поток

Использует функционал класса статических методов конвертации времени Calendar

---

### 5. TZIPThread
Поток ZIP-архиватор

Использует функционал сторонней библиотеки quazip

---

### 6. TNetSendThread
Поток отправки данных по локальной сети

---

### 7. TNetReceiveThread
Поток приема данных из локальной сети

Использует функционал класса TByteBuffer

---

### 8. TReceivePartFileThread
Поток приема квитанции или части файла

Использует функционал класса TFile

---

### 9. TSendPartFileThread
Поток отправки части файла по локальной сети

---

### 10. TSendKvitThread
Поток отправки квитанции

---

### 11. TBasicServerTCP
Базовый сервер TCP

Использует функционал класса TNetReceiveThread

---

### 12. TSendFolderThread
Поток отправки каталога по локальной сети

Использует функционал следующих классов: TSendPartFileThread, TCheckSum, TFile

---

### 13. TReceiveFilderServer
Сервер приема каталога, обработки и отправки квитанции

Использует функционал следующих классов: TCheckSum, TFile, TZIPThread, TSendKvitThread, TReceivePartFileThread
