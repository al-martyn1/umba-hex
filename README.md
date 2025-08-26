# Утилита Umba HEX

- [Краткое описание возможностей утилиты Umba HEX](#краткое-описание-возможностей-утилиты-umba-hex)
- [Синтаксис описания диаграмм](#синтаксис-описания-диаграмм)
- [Пример описания протокола обмена](#пример-описания-протокола-обмена)
  - [Кадр (Frame)](#кадр-frame)
  - [Заколовок (Header)](#заколовок-header)
  - [Рабочая нагрузка (Payload)](#рабочая-нагрузка-payload)
- [Пример описания раскладки HEX-файла](#пример-описания-раскладки-hex-файла)
  - [Описание раскладки](#описание-раскладки)
  - [Диаграмма, 16 байт шириной](#диаграмма-16-байт-шириной)
  - [Диаграмма, 32 байта шириной](#диаграмма-32-байта-шириной)
  - [Диаграмма, 64 байт шириной](#диаграмма-64-байт-шириной)
  - [Сгенерированный C/C++ код](#сгенерированный-cc-код)
- [Генерация HEX-файла и пример пайплайна для производства и разработки](#генерация-hex-файла-и-пример-пайплайна-для-производства-и-разработки)
  - [Первоначальная прошивка нового устройства на производстве](#первоначальная-прошивка-нового-устройства-на-производстве)
  - [Сборка прошивки](#сборка-прошивки)
  - [Пример скрипта для формирования HEX-файла по описанию диаграммы](#пример-скрипта-для-формирования-hex-файла-по-описанию-диаграммы)


# Краткое описание возможностей утилиты Umba HEX

**Важно:** Утилита **Umba HEX** в текущий момент не реализована в виде самостоятельного инструмента,
весь функционал содержится в нескольких взаимосвязанных библиотеках. Краткое описание возможностей утилиты
основано на моём представлении о её функционале и возможностях, реализованных в библиотеках. Все примеры
взяты из различных тестов этих библиотек.


Утилита **Umba HEX** предназначена для:

- описания каких-либо двоичных структур или пакетов;
- описания фиксированных частей раскладок HEX-файлов прошивок микроконтроллеров.


Утилита **Umba HEX** умеет:

- генерировать код на C/C++ для использования в коде для обращения к полям пакета/фиксированной части раскладки HEX-файла;
- генерировать графические диаграммы для использования в документации;
- заполнять HEX-файлы заданными значениями.
- при заполнении HEX-файлов утилита **Umba HEX** умеет самостоятельно расчитывать CRC 8/16/32 предыдущей части пакета
  согласно заданным `poly` и `seed` в опциях поля.


# Синтаксис описания диаграмм

Синтаксис описания диаграмм основан на синтаксисе диаграмм [mermaid packet diagram](https://mermaid.js.org/syntax/packet.html),
с некоторыми расширениями:

- в `mermaid packet diagram`-ах последовательность символов `%%` начинает однострочный коментарий.
  В расширении `Umba HEX` коментарий начинается так же, но последовательность `%%#!` начинает
  директивы расширения;

- `mermaid packet diagram` оперирует битами, `Umba HEX` позволяет оперировать байтами.
  Для этого после указания типа `mermaid` диаграммы (для `packet diagram`) идентификатором `packet-beta`
  добавляем расширение: `packet-beta %%#! byte-diagram`. Для битовых диаграмм ничего не добавляем или указываем
  `packet-beta %%#! bit-diagram`;

- позволяет использовать не только диапазоны, но и привычные программистам типы `int/uint8/16/32` и `char`, при этом
  поля будут иметь соответствующий размер в байтах;

- позволяет использовать массивы: `char[16] : "Name" %%#! ascii-z`;

- глобальные расширенные директивы начинаются со специального коментария в начале строки:
    - `%%#! native le data 32 bits segment 16 bits segment-shift 4 bits offset 16 bits` - описание 'нативной' платформы;
    - `%%#! display-options byte-numbers no-single-byte-numbers` - флаги, управляющие отображением диаграммы;
    - `%%#! display-width 16` - количество байт, отображаемых на одной строке диаграммы, 8/16/32/64.

- опции отдельных полей задаются после их описания в той же строке  после расширенного коментария `%%#!`;

- опциями отдельных полей могут быть следующие опции:
  `ascii-z` - массив/строка, заканчивающаяся нулем - если поле заполнено целиком, нули остутствуют, иначе
  неинициализированные байты заполняются нулями (**примечание:** тут надо вспомнить - весьма вероятно, что
  завершающий ноль гарантируется, тогда значимых байт/символов хранится на один меньше); `block` - не помню,
  вроде отображение на диаграмме блоком (массивом);
  `le`, `be`, `me-le`, `me-be` - little|big-endian и middle-endian-little|big-endian (обратите внимание
  на индексы байт многобайтных значений на диаграммах);
  `crc`, `poly`, `seed` - CRC-поле и его параметры.

**Важно:** синтаксис `+1: "Block name"` для задания количества бит/байт в текущий момент не поддерживается
(что-то я профакапил, или, что тоже вероятно, эта фича в mermaid добавлена уже после того, как я делал свою реализацию).


# Пример описания протокола обмена

**Примечание 1:** в сгенерированном C/C++ коде для частей пакета есть баг -
когда у нас в описании пакета только один фрагмент с единственной директивой
`ORG` или вообще без неё, то должна генерироваться только одна C/C++ структура,
сейчас генерируется две.
Это баг, надо разбираться.

**Примечание 2:** на диаграммах также отображаются артефакты, связанные с несколькими
`ORG`-директивами. Это связано с тем, что основное тестирование производилось с прицелом
на формирование HEX-файлов с несколькими секциями.

**Примечание 3:** пакеты с полями переменной длины пока не поддерживаются, вместо
них используем какую-либо фиксированную длину. Сгенерированные C/C++ не будут точно
соответствовать структуре пакета.


## Кадр (Frame)

**Описание кадра**

```
%%#! native le data 32 bits
%%#! display-width 16

packet-beta %%#! byte-diagram
title Frame

0-5: "Header"
uint8[1]: "Paylod"
uint32: "CRC" %%#! CRC 0-6 poly 0x1234 be
```

**Графическая иллюстрация раскладки данных**
![](doc.drafts/samples/example-01-1-frame.svg)

**Сгенерированный C/C++ код**

```cpp
/* #pragma pack(push,1) */
struct FrameOrg0
{
    std::uint8_t    header[6];                // 0 - 5
    std::uint8_t    paylod[1];                // 6
    std::uint32_t   crc;                      // 7 - 10

}; // struct FrameOrg0
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct Frame
{
    FrameOrg0       frameOrg0;                // ORG 00000000

}; // struct Frame
/* #pragma pack(pop) */
```


## Заколовок (Header)

**Описание заголовка**

```
%%#! native le data 32 bits
%%#! display-width 16

packet-beta %%#! byte-diagram
title Header

uint8[2]: "Preamble"
uint16  : "Remaining bytes count"
uint16  : "CRC" %%#! CRC 0-3 poly 0x1234 be
```

**Графическая иллюстрация раскладки данных**
![](doc.drafts/samples/example-01-2-header.svg)

**Сгенерированный C/C++ код**

```cpp
/* #pragma pack(push,1) */
struct HeaderOrg0
{
    std::uint8_t    preamble[2];              // 0 - 1
    std::uint16_t   remainingBytesCount;      // 2 - 3
    std::uint16_t   crc;                      // 4 - 5

}; // struct HeaderOrg0
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct Header
{
    HeaderOrg0      headerOrg0;               // ORG 00000000

}; // struct Header
/* #pragma pack(pop) */
```


## Рабочая нагрузка (Payload)

**Описание рабочей нагрузки**

```
%%#! native le data 32 bits
%%#! display-width 16

packet-beta %%#! byte-diagram
title Payload

uint8   : "FunctionId"
uint8[1]: "Data"
```

**Графическая иллюстрация раскладки данных**
![](doc.drafts/samples/example-01-3-payload.svg)

**Сгенерированный C/C++ код**

```cpp
/* #pragma pack(push,1) */
struct PayloadOrg0
{
    std::uint8_t    functionId;               // 0
    std::uint8_t    data[1];                  // 1

}; // struct PayloadOrg0
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct Payload
{
    PayloadOrg0     payloadOrg0;              // ORG 00000000

}; // struct Payload
/* #pragma pack(pop) */
```


# Пример описания раскладки HEX-файла

В раскладке описывается сегментированный HEX-файл, раскладка описывает
два раздела/фрагмента: "Harware Info" и "Software Info"


## Описание раскладки

```
---
Title: Test
 example
Options: a b c ascii-z, little-endian
---

%% https://mermaid.js.org/syntax/packet.html#examples

%% linear 16 bits? Linear address size?

%%#! native le data 32 bits segment 16 bits segment-shift 4 bits offset 16 bits
%%#! display-options none byte-numbers no-single-byte-numbers single-byte-block-numbers
%%#! display-options split-words-to-bytes show-single-byte-field-index no-hex-field-index hex-prefix-field-index
%%#! display-options show-field-labels show-title no-title-on-top hex-prefix-section

%%#! display-width 16
%% display-width 32
%% display-width 64


%% single-byte-numbers
%% single-byte-numbers no-single-byte-numbers single-byte-block-numbers no-single-byte-block-numbers byte-numbers no-byte-numbers split-words-to-bytes no-split-words-to-bytes


packet-beta %%#! byte-diagram

%% #! org 0x1000:0xFFAC : "Harware Info"
%% #! org 0x1000:0xFF94 : "Harware Info"
%%#! org 0x1000:0xFF40  : "Harware Info"

%% char[16]  : "Device Name"                  %% 0-15
0-13      : "Device Name"                  %%#! ascii-z block %% 0-13
uint64    : "uint64"
uint8     : "Hardware Revision Major"
uint8     : "Hardware Revision Minor"
uint16    : "Manufactured Year"
uint8     : "Manufactured Month"
uint8     : "Manufactured Day"
uint16    : "Batch"
%% 24-31     : "Serial Number"                %%#! block
30-37     : "Serial Number"                %%#! block
%% 26-33     : "Serial Number"                %%#! block
uint16    : "SignatureLE"                  %%#! le
uint16    : "SignatureBE"                  %%#! be


%%#! org +48: "Software Info"
%% #! org auto : "Software Info"

uint16    : "Firmware Version Major"
uint16    : "Firmware Version Minor"
uint16    : "Firmware Version Build"
uint16    : "Build Year"
uint8     : "Build Month"
uint8     : "Build Day"
uint8     : "Build Hour"
uint8     : "Build Minute"
%% uint8     : "Build Second"
char[16]  : "Version Info String"    %%#! ascii-z block %% Must contain version (Major.Minor.Build) the same as in uint16 fields above
char[16]  : "Built Date Time String" %%#! ascii-z block %% Must contain date and time the same as in integer fields above
char      : "signleChar1"
uint64    : "LeTest" %%#! le
char      : "signleChar2"
uint64    : "BeTest" %%#! be
char      : "signleChar3"
uint64    : "MeTest" %%#! le-me
char      : "signleChar4"
uint64[4] : "Test Array[4] of uint64"
char      : "signleChar5"
uint32[8] : "Test Array[8] of uint32"
char[7]   : "fill"                       %%#! %% block
```

## Диаграмма, 16 байт шириной

![](doc.drafts/samples/example-02-w16.svg)

## Диаграмма, 32 байта шириной

![](doc.drafts/samples/example-02-w32.svg)

## Диаграмма, 64 байт шириной

![](doc.drafts/samples/example-02-w64.svg)

## Сгенерированный C/C++ код

```cpp
/* #pragma pack(push,1) */
struct HarwareInfo
{
    std::char       deviceName[14];           // 0 - 13
    std::uint64_t   uint64;                   // 14 - 21
    std::uint8_t    hardwareRevisionMajor;    // 22
    std::uint8_t    hardwareRevisionMinor;    // 23
    std::uint16_t   manufacturedYear;         // 24 - 25
    std::uint8_t    manufacturedMonth;        // 26
    std::uint8_t    manufacturedDay;          // 27
    std::uint16_t   batch;                    // 28 - 29
    std::uint8_t    serialNumber[8];          // 30 - 37
    std::uint16_t   signatureLe;              // 38 - 39
    std::uint16_t   signatureBe;              // 40 - 41

}; // struct HarwareInfo
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct SoftwareInfo
{
    std::uint16_t   firmwareVersionMajor;     // 0 - 1
    std::uint16_t   firmwareVersionMinor;     // 2 - 3
    std::uint16_t   firmwareVersionBuild;     // 4 - 5
    std::uint16_t   buildYear;                // 6 - 7
    std::uint8_t    buildMonth;               // 8
    std::uint8_t    buildDay;                 // 9
    std::uint8_t    buildHour;                // 10
    std::uint8_t    buildMinute;              // 11
    char            versionInfoString[16];    // 12 - 27
    char            builtDateTimeString[16];  // 28 - 43
    char            signleChar1;              // 44
    std::uint64_t   leTest;                   // 45 - 52
    char            signleChar2;              // 53
    std::uint64_t   beTest;                   // 54 - 61
    char            signleChar3;              // 62
    std::uint64_t   meTest;                   // 63 - 70
    char            signleChar4;              // 71
    std::uint64_t   testArray4OfUint64[4];    // 72 - 103
    char            signleChar5;              // 104
    std::uint32_t   testArray8OfUint32[8];    // 105 - 136
    char            fill[7];                  // 137 - 143

}; // struct SoftwareInfo
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct TestExample
{
    Harware Info    harwareInfo;              // ORG 1000:FF40
    std::uint8_t    fill0[6];                 // 42 - 47
    Software Info   softwareInfo;             // ORG 1000:FF70

}; // struct TestExample
/* #pragma pack(pop) */
```


# Генерация HEX-файла и пример пайплайна для производства и разработки

Прошивка в устройстве может состоять из нескольких частей: метаинформация
и собственно, сама исполняемая прошивка. Метаинформация, к примеру, может состоять
из описания аппаратной части и описания программной части.


## Первоначальная прошивка нового устройства на производстве

Метаинформация об аппаратной части зашивается один раз при первоначальной прошивке
устройства после сборки, туда зашивается актуальная информация о ревизии платы, дате сборки,
номере партии, серийном номере устройства. Для генерации такой информации можно использовать
утилиту **Umba HEX**.

Также в при первоначальной прошивке может зашиваться базовый загрузчик (bootloader), который
проверяет при  прошивке только метаинформацию об аппаратной части в устройстве и в новой прошивке,
и выдаёт отказ при их несовпадении.

Метаинформацию об аппаратной части можно генерировать в отдельный HEX-файл с последующим объединением
HEX-файлов загрузчика и метаинформации об аппаратуре, или можно генерировать
в существующий файл (загрузчика).


## Сборка прошивки

При сборке прошивки при помощи утилиты **Umba HEX** генерируется метаинформация по аппаратуре, для которой
собирается прошивка, а также о версии собираемой прошивки.

В прошивке также имеется загрузчик, который проверяет аппаратную версию платы, а также версию прошивки.

Для генерации метаинформации о программной части также может использоваться утилита **Umba HEX**.
При этом эта информация генерируется в готовый файл прошивки, или в отдельные файлы для последующего объединения
сторонними утилитами.

Адреса блоков метаинформации утилитой **Umba HEX** могут выводится в C/C++ файл, описания структур
метаинформации также выводятся в C/C++ файл, и эти исходные файлы используются при сборке прошивки.
Таким образом загрузчик в прошивке будет знать, где метаинформация расположена во флэш-памяти,
знает её раскладку, и может произвести нужные проверки: не позволить зашить прошивку от платы другой ревизии,
не позволить прошить прошивку с более старой версией firmware или с более старой датой сборки.


## Пример скрипта для формирования HEX-файла по описанию диаграммы

Для генерации HEX-файла утилита **Umba HEX** принимает:
1. описание раскладки (диаграмму);
2. "скрипт" для задания значений полей.

Имена полей сквозные для всей диаграммы, т.е. они должны быть уникальными для всего файла диаграммы.

В HEX-файл записываются только те поля, которые явно задаются, таким образом, можно в одной диаграмме
описать как метаинформацию об аппаратуре, так и метаинформацию о firmware, но заполнять/генерировать
HEX-файлы по ним по отдельности.

**Пример "скрипта" для формирования HEX-файла**

```
# 42 4D
"SignatureLE" : 'BM'
"SignatureBE" : 'BM'

# 41 53 34 30 30 37 00 00 00 00 00 00 00 00 00 00
Device Name:"AS4007"

# 45 53 34 30 30 37
'Device Name[0]':'E'

Hardware Revision Major : 1
Hardware Revision Minor : 3
Manufactured Year : 2025
Manufactured Month: 2
Manufactured Day  : 18
Batch: 1001
"Serial Number" : 81 BE CA FE DE AD BE AF

"Firmware Version Major" : 3
"Firmware Version Minor" : 15
"Firmware Version Build" : 1365


# uint16    : "Build Year"
# uint8     : "Build Month"
# uint8     : "Build Day"
# uint8     : "Build Hour"
# uint8     : "Build Minute"
# uint8     : "Build Second"

"Version Info String"    : "debug"
"Built Date Time String" : "Feb 12, 2025"
```


