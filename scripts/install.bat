@echo off
echo Set oWS = WScript.CreateObject("WScript.Shell") > ClientShortcut.vbs
echo If not oWS Is Nothing Then >> ClientShortcut.vbs
echo DesktopPath = oWS.SpecialFolders("Desktop") >> ClientShortcut.vbs
echo Set oLink = oWS.CreateShortcut(DesktopPath ^& "\Collaborative Doc.lnk") >> ClientShortcut.vbs
echo oLink.Arguments = "localhost 8081" >> ClientShortcut.vbs
echo oLink.Description = "Collaborative Text Editor" >> ClientShortcut.vbs
echo oLink.IconLocation = "%CD%\src\widget\ui\images\app\logo.ico" >> ClientShortcut.vbs
echo oLink.TargetPath = "%CD%\cmake-build-debug\src\client_new\client.exe" >> ClientShortcut.vbs & :: to be updated
echo oLink.WindowStyle = 1 >> ClientShortcut.vbs & :: normal, not maximized
echo oLink.WorkingDirectory = "%CD%" >> ClientShortcut.vbs
echo oLink.Save >> ClientShortcut.vbs
echo end if >> ClientShortcut.vbs
cscript ClientShortcut.vbs /NoLogo /T:10 /D
del ClientShortcut.vbs