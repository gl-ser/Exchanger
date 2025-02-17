//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Main.cpp                                                              //
//                                                                            //
//                      *** ТОЧКА ВХОДА В ПРИЛОЖЕНИЕ ***                      //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#include "MainWin.h"
#include <QApplication>


int main(int argc, char *argv[])
{
QApplication Application(argc, argv);
TMainWin* AbonentMKSOD;
int res;

#ifndef __unix__
QStringList paths;
  paths = QCoreApplication::libraryPaths();
  paths.append(".");
  //paths.append("imageformats");
  paths.append("platforms");
  paths.append("sqldrivers");
  QCoreApplication::setLibraryPaths(paths);
#endif

  AbonentMKSOD = new TMainWin();
  AbonentMKSOD->show();
  res = Application.exec();
  delete AbonentMKSOD;
  return res;
}

