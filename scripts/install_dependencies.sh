#!/bin/bash

# Qt
apt install qt5-default libqt5sql5-mysql
if [ $? -ne 0 ]; then
	echo "You need root privileges"
	exit 1
fi

# cryptopp
wget https://www.cryptopp.com/cryptopp820.zip
mkdir -p cryptopp
mv cryptopp*.zip cryptopp
cd cryptopp
unzip cryptopp*.zip
make libcryptopp.a libcryptopp.so cryptest.exe
make test
make install
