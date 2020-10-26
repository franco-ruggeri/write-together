#!/bin/bash

# install requirements 
# make sure you have installed cryptopp... see docs/cryptopp_setup.md
apt install qt5-default libboost-all-dev libqt5sql5-mysql
if [ $? -ne 0 ]; then
	echo "You need root privileges"
	exit 1
fi
