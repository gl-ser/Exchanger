//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Placement.cpp                                                         //
//                                                                            //
//      *** TPLACEMENT КЛАСС ДЛЯ СОХРАНЕНИЯ ПОЛОЖЕНИЯ И РАЗМЕРОВ ОКОН ***     //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "Placement.h"


TPlacement::TPlacement(void *Owner)
{
  OWNER = Owner;
  FIniFileName = "";
  FIniSection = "";
}


TPlacement::~TPlacement()
{
  OWNER = NULL;
  FIniFileName = "";
  FIniSection = "";
}


void TPlacement::Save(void)
{
QSettings *ConfigFile;  //INI-файл настроек приложения

  if ((OWNER != NULL) && (FIniFileName != "") && (FIniSection != ""))
  {
    ConfigFile = new QSettings(QString::fromStdString(FIniFileName), QSettings::IniFormat);

    ConfigFile->setValue(QString::fromStdString(FIniSection+"/Place"), static_cast<QWidget*>(OWNER)->saveGeometry());

    delete ConfigFile;
  }
}


void TPlacement::Restore(std::string IniFileName, std::string IniSection)
{
QSettings *ConfigFile;  //INI-файл настроек приложения

  FIniFileName = IniFileName;
  FIniSection = IniSection;
  ConfigFile = new QSettings(QString::fromStdString(FIniFileName), QSettings::IniFormat);

  if (ConfigFile->value(QString::fromStdString(FIniSection+"/Place"), "").toString().toStdString() != "")
  {
    static_cast<QWidget*>(OWNER)->restoreGeometry(ConfigFile->value(QString::fromStdString(FIniSection+"/Place"), "").toByteArray());
  }

  delete ConfigFile;
}
