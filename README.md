## Харитонов Кирилл Александрович БПИ 214
### Вариант 16
### Условие задачи
Задача о клумбе – 2. На клумбе растет 40 цветов, за ними непрерывно следят два процесс–садовника и поливают увядшие цветы,
при этом оба садовника очень боятся полить один и тот же цветок, который еще не начал вянуть. Создать приложение, моделирующее состояния цветков на клумбе и действия садовников. Сервер используется для обмена информацией между
клиентами садовниками и клиентом — клумбой. Клумба — клиент, отслеживающий состояния всех цветов. Каждый садовник
— отдельный клиент.

### Расположение файлов
В папке grade_4 расположена программа, которая отвечает требованиям на оценку 4-5.
В папке grade_6&8 программа, которая одновременно отвечает требованиям на оценку 6-7 и 8.

### Сценарий решаемой задачи

Клиент в зависимости от заданных аргументов ведет себя 2 способами: как садовник или как клумба. В случае садовника ему дополнительно задается номер.
Садовник постоянно отправляет запросы о состоянии каждого цветка и выводит полученный ответ. Клумба же сначала генерирует поле из 40 цветков, где каждому цветку сопоставлено некоторое значение
от 1 до 256, каждый раз когда садовник запрашивает состояние цветка это число уменьшается на 1. Если число равно 0, то цветок в данный момент требует полива, после этого ему задается новое значение от 1 до 256.
Так как клумба обрабатывает только один запрос одновременно, то ситуация, где несколько садовников поливают одну и ту же клумбу невозможна.

Сервер получает все сообщения от клиентов и отправляет их всем клиентам. Клиенты же в последствии по виду сообщений понимают, какие из них предназначаются для них и отвечают соответственно.

## Требования на 4-5

### IP и порты

IP адрес задается первым аргументом командной строки у клиента, порт задается вторым аргументом у клиента и первым аргументом у сервера

### Пример выводы программы
#### Садовник 1
    flower number 19 does not need watering
    flower number 3 does not need watering
    flower number 7 does not need watering
    flower number 9 does not need watering
    flower number 13 does not need watering
    flower number 14 does not need watering
    flower number 16 does not need watering
    flower number 17 does not need watering
    flower number 28 does not need watering
    flower number 18 does not need watering
    flower number 30 does not need watering
    flower number 32 does not need watering
    flower number 21 does not need watering
    flower number 22 does not need watering

#### Садовник 2
    flower number 38 does not need watering
    flower number 39 does not need watering
    flower number 0 does not need watering
    flower number 1 does not need watering
    flower number 2 does not need watering
    flower number 3 does not need watering
    flower number 4 does not need watering
    flower number 5 does not need watering
    flower number 6 does not need watering
    flower number 7 does not need watering
    flower number 8 does not need watering
    flower number 9 does not need watering
    flower number 10 does not need watering
    flower number 11 does not need watering
    flower number 12 does not need watering
    flower number 13 does not need watering
    flower number 14 does not need watering
    flower number 15 does not need watering
    flower number 16 does not need watering

#### Клумба
    client number 2 asks for status of flower number 23
    client number 2 asks for status of flower number 24
    client number 2 asks for status of flower number 25
    client number 2 asks for status of flower number 26
    client number 2 asks for status of flower number 27
    client number 2 asks for status of flower number 28
    client number 2 asks for status of flower number 29
    client number 2 asks for status of flower number 30
    client number 2 asks for status of flower number 31
    client number 2 asks for status of flower number 32
    client number 2 asks for status of flower number 33
    client number 2 asks for status of flower number 34
    client number 2 asks for status of flower number 35
    client number 2 asks for status of flower number 36
    client number 2 asks for status of flower number 37
    client number 2 asks for status of flower number 38
    client number 2 asks for status of flower number 39
    client number 2 asks for status of flower number 0
    client number 2 asks for status of flower number 1
    client number 2 asks for status of flower number 2
    client number 2 asks for status of flower number 3
    client number 2 asks for status of flower number 4
    client number 2 asks for status of flower number 5
    client number 2 asks for status of flower number 6
    client number 2 asks for status of flower number 7
    client number 2 asks for status of flower number 8
    client number 2 asks for status of flower number 9
    client number 2 asks for status of flower number 10
    client number 2 asks for status of flower number 11
    client number 2 asks for status of flower number 12

#### Сервер
    Server listening on port 8080...
    New client connected.
    New client connected.
    New client connected.

### Завершение работы
Завершение работы происходит отправкой сигнала прерывания в каждый терминал, в котором запущена программа.

### Требования на 6-7 и 8

В клиент был добавлен еще один вариант поведения, который просто выводит все сообщения поступающие ему от сервера.
Так как сервер изначально поддерживал возможность подключения нескольких клиентов, то модифицировать программу дополнительно для оценки 8 не требовалось.
Подключение и отключение клиентов не влияет на другие клиенты и сервер, поэтому их можно подключать и отключать в динамическом режиме без нарушения работы всего приложения.

### Пример вывода наблюдателя

    1 24 1 25 2 5 1 26 2 6 2 7 2 8 1 27 1 28 1 29 flower number 24 does not need watering
    2 9 2 10 2 11 flower number 5 does not need watering
    flower number 26 does not need watering
    flower number 6 does not need watering
    flower number 8 does not need watering
    flower number 27 does not need watering
    flower number 28 does not need watering
    flower number 9 does not need watering
    flower number 10 was watered by client 2


### Команды для запуска

Каждая команда запускается в отдельном терминале, из директории с программой.
Первым должен быть запущен сервер, за ним flowerbed, далее не важно.

    ./server 8080
    ./client 127.0.0.1 8080 flowerbed
    ./client 127.0.0.1 8080 gardener 1
    ./client 127.0.0.1 8080 gardener 2
    ./client 127.0.0.1 8080 observer
    ./client 127.0.0.1 8080 observer