//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл CancelOK.cpp                                                          //
//                                                                            //
//                   *** TCANCELOK ОКНО ВЫБОРА РЕШЕНИЯ ***                    //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "CancelOK.h"


TCancelOK::TCancelOK(QWidget *parent) : QDialog(parent)
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


TCancelOK::~TCancelOK()
{
  Placement->Save();
  delete Placement;
  FIniFileName = "";
  what = false;
}


bool TCancelOK::WhatCanIDo()
{
  what = false;
  this->exec();
  return what;
}


void TCancelOK::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_CANCEL_OK");
}


void TCancelOK::on_pushButton_clicked()
{  //OK
  what = true;
  close();
}


void TCancelOK::on_pushButton_2_clicked()
{  //Отмена
  what = false;
  close();
}


void TCancelOK::MoveToCenter()
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


void TCancelOK::showEvent(QShowEvent *event)
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


void TCancelOK::hideEvent(QHideEvent *event)
{
  if (event->type() == QEvent::Hide)
  {

  }
  else
  {
    QWidget::hideEvent(event);
  }
}
