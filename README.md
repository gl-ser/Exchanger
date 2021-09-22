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

---

### 2. TFile
Работа с файлом

---

### 3. TCheckSum
Подсчет контрольной суммы

---

### 4. TBasicThread
Базовый поток. Использует функционал класса статических методов конвертации времени Calendar

---

### 5. TZIPThread
Поток ZIP-архиватор. Использует функционал сторонней библиотеки quazip

---

### 6. TNetSendThread
Поток отправки данных по локальной сети

---

### 7. TNetReceiveThread
Поток приема данных из локальной сети. Использует функционал TByteBuffer

---

### 8. TReceivePartFileThread
Поток приема квитанции или части файла. Использует функционал TFile

---

### 9. TSendPartFileThread
Поток отправки части файла по локальной сети

---

### 10. TSendKvitThread
Поток отправки квитанции

---

### 11. TBasicServerTCP
Базовый сервер TCP. Использует функционал TNetReceiveThread

---

### 12. TSendFolderThread
Поток отправки каталога по локальной сети. Использует функционал TSendPartFileThread, TCheckSum, TFile

---

### 13. TReceiveFilderServer
Сервер приема каталога, обработки и отправки квитанции. Использует функционал TCheckSum, TFile, TZIPThread, TSendKvitThread, TReceivePartFileThread
