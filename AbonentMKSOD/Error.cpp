//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл Error.h                                                               //
//                                                                            //
//                  *** ERROR ДИАГНОСТИЧЕСКИЕ СООБЩЕНИЯ ***                   //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2021 год //
//----------------------------------------------------------------------------//


#include "Error.h"


TError::TError(QWidget *parent) : QDialog(parent)
{
  setupUi(this);

  #ifndef __unix__
  //Устанавливаю значки заголовка окна: "закрытие"
  setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
  #endif

  FIniFileName = "";
  Placement = new TPlacement(this);

  //До момента чтения положения окна из INI-файла окно размещается по центру экрана
  MoveToCenter();
}


TError::~TError()
{
  Placement->Save();
  delete Placement;
  FIniFileName = "";
}


void TError::ShowMessage(std::string str)
{
  setWindowTitle("Абонент СОДА");
  Label->setText(QString::fromStdString(str));
  pushButton->setIcon(QIcon(":/1/Picture/OKYellow.png"));
  exec();
}


void TError::MessageGreen(std::string str)
{
  setWindowTitle("Сообщение");
  Label->setText(QString::fromStdString(str));
  pushButton->setIcon(QIcon(":/1/Picture/OKGreen.png"));
  exec();
}


void TError::MessageRed(std::string str)
{
  setWindowTitle("Запрет");
  Label->setText(QString::fromStdString(str));
  pushButton->setIcon(QIcon(":/1/Picture/OKRed.png"));
  exec();
}


void TError::IniFileName(std::string Value)
{
  FIniFileName = Value;
  Placement->Restore(FIniFileName, "WIN_ERROR");
}


void TError::on_pushButton_clicked()
{  //OK
  close();
}


void TError::MoveToCenter()
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


void TError::ReSize()
{
QFontMetrics fm(Label->font());
QRect Rect;

  //Определение размеров выводимого текста
  Rect = fm.boundingRect(Label->text());

  //Изменение размеров метки, чтобы на ней помещался выводимый текст
  Label->resize(Rect.width() + 10, Rect.height() + 4);

  //Изменение размеров окна, чтобы на нем помещалась метка
  this->resize(Rect.width() + 50, 100);
}


void TError::showEvent(QShowEvent *event)
{
  if( event->type() == QEvent::Show)
  {
    ReSize();
    pushButton->setFocus();
  }
  else
  {
    QWidget::showEvent(event);
  }
}


void TError::hideEvent(QHideEvent *event)
{
  if( event->type() == QEvent::Hide)
  {

  }
  else
  {
    QWidget::hideEvent(event);
  }
}
