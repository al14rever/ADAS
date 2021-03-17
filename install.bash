#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

apt --assume-yes install cmake libssl-dev libsasl2-dev rsync

mkdir /root/ADAS_SOURCE
cd /root/ADAS_SOURCE

git clone https://github.com/al14rever/ADAS.git
mkdir ADAS/build && cd ADAS/build
cmake .. && make

cp ADAS /root
cp ../adas.config /root
chmod +x /root/ADAS
cp ../ADAS.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable ADAS.service
systemctl start ADAS.service

cd ~