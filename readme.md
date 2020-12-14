# Как скомпилировать

Для работы нужны библиотеки:

* [WiringOP](https://github.com/zhaolei/WiringOP)
* [MongoDB C++ Driver](http://mongocxx.org/)

# Установка библиотек

sudo apt-get install cmake libssl-dev libsasl2-dev

Установка WiringOP.

```bash
    git clone https://github.com/zhaolei/WiringOP.git -b h3
    cd WiringOP
    chmod +x ./build
    sudo ./build
```

Установка mongo-c-driver.

```bash
    wget https://github.com/mongodb/mongo-c-driver/releases/download/1.17.3/mongo-c-driver-1.17.3.tar.gz
    tar -zxvf mongo-c-driver-1.17.2.tar.gz mongo-c-driver-1.17.2/
    cd mongo-c-driver-1.17.3
    mkdir cmake-build
    cd cmake-build
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
    cmake --build .
    sudo cmake --build . --target install
```

Установка mongo-cxx-driver.

```bash
    curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.2/mongo-cxx-driver-r3.6.2.tar.gz
    tar -zxvf mongo-cxx-driver-r3.6.2.tar.gz mongo-cxx-driver-r3.6.2/
    cd mongo-cxx-driver-r3.6.2/build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_PREFIX_PATH=/usr/local -DBSONCXX_POLY_USE_MNMLSTC=1 ..
    sudo cmake --build . --target EP_mnmlstc_core
    cmake --build .
    sudo cmake --build . --target install
```


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

# Файл конфигурации

Вставить свои значения в файл adas.config

# Автозапуск

```bash
    sudo cp ADAS /root
    sudo cp ../../adas.config /root
    sudo +x /root/ADAS
    sudo cp ../ADAS.service /etc/systemd/system/
    sudo systemctl daemon-reload
    sudo systemctl enable ADAS.service
    sudo systemctl start ADAS.service
```

# Примечание

Библиотека для работы с экранон работает только по I2C с микросхемой PCF8574.

Используеться перевод из UTF-8 в cp1251 взятый от [сюда](https://code.google.com/archive/p/convert-utf8-to-cp1251/).
    