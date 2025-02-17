//----------------------------------------------------------------------------//
//       *** МНОГОПОТОЧНАЯ КРОССПЛАТФОРМЕННАЯ СИСТЕМА ОБМЕНА ДАННЫМИ ***      //
//                                                                            //
// Файл CtrlOUTCat.h                                                          //
//                                                                            //
//                  *** TCTRLOUTCAT КОНТРОЛЬ КАТАЛОГА OUT ***                 //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2025 год //
//----------------------------------------------------------------------------//


#ifndef CTRLOUTCAT_H
#define CTRLOUTCAT_H

#include <QObject>
#include <QBasicTimer>

#include "WinListAbonent.h"
#include "../EngineMKSOD/SendFolderThread.h"


class TCtrlOUTCat : public QObject
{
  Q_OBJECT

  private slots:
    //Обработчик сигнала, который генерируется в случае возникновения ошибки в потоке отправки каталога
    // Возвращает:
    //   - номер потока;
    //   - длительность работы потока [сек];
    //   - описание ошибки
    void OnSendFolderError(int Number, int Duration, QString Str);

    //Обработчик сигнала, который генерируется в случае успешного завершения потока отправки каталога
    // Возвращает:
    //   - номер потока;
    //   - длительность работы потока [сек]
    void OnSendFolderSuccess(int Number, int Duration);

    //Обработчик сигнала, который генерируется при необходимости передачи словесного сообщения вызывающей программе
    // Возвращает:
    //   - номер потока;
    //   - текст сообщения
    void OnSendFolderMessage(int Number, QString Str);

  private:
    void* OWNER;

    QBasicTimer* TimerOUT;  //Таймер контроля каталога OUT (1 сек)
    TWinListAbonent::TAbonents Abon;  //Перечень абонентов МКСОД
    std::vector<TSendFolderThread*> SendFolders;  //Массив потоков по отправке каталогов

    //Удаляет файлы из указанного каталога
    void DelFilesFromDir(QString PathDir);

    //Удаляет подкаталоги (со всем их содержимым) из указанного каталога
    void DelSubDirs(QString PathDir);

    //Удаляет указанный каталог, если он пустой
    void DelDir(QString PathDir);

    //Возвращает количество файлов в указанном каталоге и его подкаталогах
    // Подаваемая на вход переменная Quantity должна быть инициализирована нулем
    void CountFilesInDir(QString PathDir, unsigned int *Quantity);

    //Возвращает список файлов (с полным путем) в каталоге и во всех подкаталогах
    // Подаваемый на вход список FilesList должен быть пустым
    void ListFilesInDir(QString PathDir, QStringList *FilesList);

  protected:
    //Обработчик событий таймеров
    void timerEvent(QTimerEvent *event);

  public:
    TCtrlOUTCat(void* Owner);
    ~TCtrlOUTCat(void);

    //Запустить процесс мониторинга каталога OUT
    void StartCtrl(void);

    //Остановить процесс мониторинга каталога OUT
    void StopCtrl(void);

    //Запустить таймер процесса мониторинга каталога OUT
    void StartCtrlSimple(void);

    //Остановить таймер процесса мониторинга каталога OUT
    void StopCtrlSimple(void);
};


#endif
