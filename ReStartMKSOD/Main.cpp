//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Main.cpp                                                              //
//                                                                            //
//                   *** УТИЛИТА ПЕРЕЗАГРУЗКИ ПРИЛОЖЕНИЯ ***                  //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include <QApplication>
#include "MainWin.h"


int main(int argc, char *argv[])
{
QApplication App(argc, argv);
TMainWin w;

#ifndef __unix__
QStringList paths;
  paths = QCoreApplication::libraryPaths();
  paths.append(".");
  //paths.append("imageformats");
  paths.append("platforms");
  //paths.append("sqldrivers");
  QCoreApplication::setLibraryPaths(paths);
#endif

  w.show();
  return App.exec();
}
