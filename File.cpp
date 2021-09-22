//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл File.cpp                                                              //
//                                                                            //
//                 *** TFILE КЛАСС МЕТОДОВ РАБОТЫ С ФАЙЛОМ ***                //
//                                                                            //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                                     Москва //
//----------------------------------------------------------------------------//


#include "File.h"


TFile::TFile()
{
  Memory = NULL;
  MMFileIndexes.clear();
}


TFile::~TFile()
{
  CloseMMFile();
}


bool TFile::CreateEmptyFile(QString FilePathName, long long Size)
{
QFile File;
bool IsOpen;

  //Задать имя файла
  File.setFileName(FilePathName);

  //Создать в файловой системе новый (или очистить существующий) файл и открыть его за запись
  IsOpen = File.open(QIODevice::WriteOnly);

  if (IsOpen == true)
  {
    File.close();  //Завершить запись в файл

    File.setPermissions(QFile::ReadOwner | QFile::WriteOwner |
                        QFile::ReadGroup | QFile::WriteGroup |
                        QFile::ReadOther | QFile::WriteOther);

    IsOpen = File.resize(Size);  //Задать размер пустого файла
  }
  return IsOpen;
}


bool TFile::OpenMMFile(QString FilePathName, bool ReadOnly)
{
bool IsOpen, res;
long long s;
  res = false;
  CloseMMFile();
  MMFile.setFileName(FilePathName);

  MMFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner |
                        QFile::ReadGroup | QFile::WriteGroup |
                        QFile::ReadOther | QFile::WriteOther);

  if (ReadOnly == true)
    IsOpen = MMFile.open(QIODevice::ReadOnly);
  else
    IsOpen = MMFile.open(QIODevice::ReadWrite);

  if (IsOpen == true)
  {
    s = MMFile.size();
    Memory = MMFile.map(0, s);

    if (Memory != NULL)
      res = true;
  }
  return res;
}


void TFile::CloseMMFile()
{
  if (Memory != NULL)
    MMFile.unmap(Memory);

  MMFile.close();
  Memory = NULL;
  MMFileIndexes.clear();
}


bool TFile::SplitMMFile(int Quantity)
{
bool res;
long long q, qq, i;
double a;
TMMFileIndex Index;
  res = false;
  q = static_cast<long long>(Quantity);  //Количество областей
  MMFileIndexes.clear();

  if (q >= 1LL)
  if (Memory != NULL)
  {
    if (q >= MMFile.size())
    {
      Index.start = 0LL;
      Index.stop = MMFile.size() - 1LL;
      Index.buf_unsigned_char = &Memory[Index.start];
      Index.buf_const_char = static_cast<const char*>(static_cast<void*>(&Memory[Index.start]));
      Index.size = Index.stop - Index.start + 1LL;
      MMFileIndexes.push_back(Index);
    }
    else
    {
      a = MMFile.size() / q;
      qq = static_cast<long long>(floor(a + 0.5));  //Количество байт в одной области (по правилу математического округления)
      Index.start = -qq;
      for(i=1LL; i<q; i++)
      {
        Index.start = Index.start + qq;
        Index.stop = Index.start + qq - 1LL;
        Index.buf_unsigned_char = &Memory[Index.start];
        Index.buf_const_char = static_cast<const char*>(static_cast<void*>(&Memory[Index.start]));
        Index.size = Index.stop - Index.start + 1LL;
        MMFileIndexes.push_back(Index);
      }
      Index.start = Index.start + qq;
      Index.stop = MMFile.size() - 1LL;
      Index.buf_unsigned_char = &Memory[Index.start];
      Index.buf_const_char = static_cast<const char*>(static_cast<void*>(&Memory[Index.start]));
      Index.size = Index.stop - Index.start + 1LL;
      MMFileIndexes.push_back(Index);
    }
    res = true;
  }

  return res;
}


long long TFile::GetFileSize(QString FilePathName)
{
QFile f;
long long FileSize;
  f.setFileName(FilePathName);
  FileSize = f.size();
  return FileSize;
}


QString TFile::GetFileName(QString FilePathName)
{
QFileInfo fileInfo(FilePathName);
  return fileInfo.fileName();
}


bool TFile::FileExist(QString FilePathName)
{
QFile f;
  f.setFileName(FilePathName);
  return f.exists();
}


bool TFile::DelFile(QString FilePathName)
{
bool res;
  res = QFile::remove(FilePathName);
  return res;
}


void TFile::ClearDir(QString DirPathName)
{
QDir *Dir;  //Класс управления каталогами

  Dir = new QDir(DirPathName);

  //Получаем список файлов в каталоге
  QStringList lstFiles = Dir->entryList(QDir::Files);

  //Удаляем файлы из каталога
  foreach (QString entry, lstFiles)
  {
    QString entryAbsPath = Dir->absolutePath() + "/" + entry;
    QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
    QFile::remove(entryAbsPath);
  }

  delete Dir;
}


TFile::TFileIndexes TFile::SplitFile(long long FileSize, int Quantity)
{
long long q, qq, i;
double a;
TFileIndex Index;
TFileIndexes FileIndexes;

  q = static_cast<long long>(Quantity);  //Количество областей
  FileIndexes.clear();

  if (FileSize >= 1LL)
  {
    if (q >= FileSize)
    {
      Index.start = 0LL;
      Index.stop = FileSize - 1LL;
      Index.size = Index.stop - Index.start + 1LL;
      FileIndexes.push_back(Index);
    }
    else
    {
      a = FileSize / q;
      qq = static_cast<long long>(floor(a + 0.5));  //Количество байт в одной области (по правилу математического округления)
      Index.start = -qq;
      for(i=1LL; i<q; i++)
      {
        Index.start = Index.start + qq;
        Index.stop = Index.start + qq - 1LL;
        Index.size = Index.stop - Index.start + 1LL;
        FileIndexes.push_back(Index);
      }
      Index.start = Index.start + qq;
      Index.stop = FileSize - 1LL;
      Index.size = Index.stop - Index.start + 1LL;
      FileIndexes.push_back(Index);
    }
  }

  return FileIndexes;
}


bool TFile::SavePartFile(QString FilePathName, QByteArray Array, long long StartIndex)
{
bool res;
QFile f;
  f.setFileName(FilePathName);
  res = f.open(QIODevice::ReadWrite);
  if (res == true)
  {
    res = f.seek(StartIndex);
    if (res == true)
    {
      f.write(Array);
    }
    f.close();
  }
  return res;
}
