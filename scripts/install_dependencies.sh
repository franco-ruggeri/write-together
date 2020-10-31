#!/bin/bash

# Qt
sudo apt install qt5-default libqt5sql5-mysql

# cryptopp
wget https://www.cryptopp.com/cryptopp820.zip
mkdir -p cryptopp
mv cryptopp*.zip cryptopp
cd cryptopp
unzip cryptopp*.zip
make libcryptopp.a libcryptopp.so cryptest.exe
make test
sudo make install
cd ..
rm -rf cryptopp
