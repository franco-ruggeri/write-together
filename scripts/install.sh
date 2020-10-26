#!/bin/bash

# compile and test
cd ..
mkdir -p build
cd build
cmake ..
make
ctest .

# install
make install

# create desktop entry
cd ..
project_dir=$(pwd)
cd ~/.local/share/applications
filename="collaborative-text-editor.desktop"
echo "[Desktop Entry]" > $filename
echo "Version=1.0" >> $filename
echo "Type=Application" >> $filename
echo "Terminal=false" >> $filename
echo "Exec=$project_dir/bin/client localhost 5000" >> $filename
echo "Name=Real-Time Collaborative Text Editor" >> $filename
echo "Icon=$project_dir/src/client/ui/images/app/logo_black.png" >> $filename
