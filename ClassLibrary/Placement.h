//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Placement.h                                                           //
//                                                                            //
//      *** TPLACEMENT КЛАСС ДЛЯ СОХРАНЕНИЯ ПОЛОЖЕНИЯ И РАЗМЕРОВ ОКОН ***     //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <QSettings>
#include <QWidget>
#include <string>


class TPlacement
{
  private:
    void* OWNER;               //Владелец экземпляра этого класса
    std::string FIniFileName;  //Путь и имя INI-файла, в котором сохраняются настройки окна
    std::string FIniSection;   //Название секции INI-файла, в которой сохраняются настройки окна

  public:
    TPlacement(void* Owner);
    virtual ~TPlacement(void);

    //Сохранить в INI-файле настройки окна
    void Save(void);

    //Прочитать из INI-файла настройки окна и настроить само окно
    void Restore(std::string IniFileName, std::string IniSection);
};


#endif
