# Как скомпилировать

Для работы нужны библиотеки:

* [WiringOP](https://github.com/zhaolei/WiringOP)
* [MongoDB C++ Driver](http://mongocxx.org/)

Скомпилированные для OrangePi PC библиотеки [тут](https://drive.google.com/drive/folders/1vEyA6w8nP6B9eOlr8gU5eNacM4JNIHJV?usp=sharing).

# Настройка системы

```bash
    sudo armbian-config
```

System 🠊 Hardware 🠊 [Включить порты I2C и UART]

# Компиляция программы

Дописать в файле ```/src/lib/prog/prog.h``` данные для БД.

```bash
    git clone https://github.com/al14rever/ADAS.git
    mkdir ADAS/build && cd ADAS/build
    cmake .. && make
```

# Автозапуск

```bash
    sudo cp ADAS /root
    sudo +x /root/ADAS
    sudo cp ../ADAS.service /etc/systemd/system/
    sudo systemctl daemon-reload
    sudo systemctl enable ADAS.service
    sudo systemctl start ADAS.service
```

# Примечание

Библиотека для работы с экранон работает только по I2C с микросхемой PCF8574.

Используеться перевод из UTF-8 в cp1251 взятый от [сюда](https://code.google.com/archive/p/convert-utf8-to-cp1251/).


    
    