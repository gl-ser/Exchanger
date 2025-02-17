//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл WinFind.cpp                                                           //
//                                                                            //
//                        *** TWINFIND ОКНО ПОИСКА ***                        //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "WinFind.h"


TWinFind::TWinFind(QWidget *parent) : QDialog(parent)
{
  setupUi(this);

  #ifndef __unix__
  //Устанавливаю значки заголовка окна: "закрытие"
  setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
  #endif

  //Запрет изменения размеров окна пользователем
  setFixedSize(size());

  FIniFileName = "";
  Placement = new TPlacement(this);

  what = false;

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();
}


TWinFind::~TWinFind()
{
  Placement->Save();
  delete Placement;
  FIniFileName = "";
  what = false;
}


bool TWinFind::WhatCanIDo()
{
  what = false;
  this->exec();
  return what;
}


void TWinFind::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_FIND");
}


void TWinFind::on_pushButton_clicked()
{  //OK
  what = true;
  close();
}


void TWinFind::on_pushButton_2_clicked()
{  //Отмена
  what = false;
  close();
}


void TWinFind::MoveToCenter()
{
QRect Rect;
QPoint Center;

  //Прямоугольник с размерами экрана
  Rect = qApp->desktop()->availableGeometry(qApp->desktop()->primaryScreen());

  //Координаты центра экрана
  Center = Rect.center();

  //Учитывается половина ширины окна
  Center.setX(Center.x() - (width()/2));

  //Учитывается половина высоты окна
  Center.setY(Center.y() - (height()/2));

  //Смещение окна по указанным координатам
  move(Center);
}


void TWinFind::showEvent(QShowEvent *event)
{
  if (event->type() == QEvent::Show)
  {
    pushButton_2->setFocus();
    what = false;
  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TWinFind::hideEvent(QHideEvent *event)
{
  if (event->type() == QEvent::Hide)
  {

  }
  else
  {
    QWidget::hideEvent(event);
  }
}
