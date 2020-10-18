root_dir=$(pwd)
cd ~/Documents/ || exit
echo [Desktop Entry] > "Collaborative Doc.desktop"
echo Version=1.0 >> "Collaborative Doc.desktop"
echo Type=Application >> "Collaborative Doc.desktop"
echo Terminal=false >> "Collaborative Doc.desktop"
echo Exec=$root_dir/bin/client localhost 8081 >> "Collaborative Doc.desktop"
echo Name=Collaborative Text Editor >> "Collaborative Doc.desktop"
echo Icon=$root_dir/src/widget/ui/images/app/logo.ico >> "Collaborative Doc.desktop"
chmod u+x Collaborative\ Doc.desktop
