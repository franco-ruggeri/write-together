@echo off
echo Set oWS = WScript.CreateObject("WScript.Shell") > ClientShortcut.vbs
echo If not oWS Is Nothing Then >> ClientShortcut.vbs
echo DesktopPath = oWS.SpecialFolders("Desktop") >> ClientShortcut.vbs
echo Set oFS = WScript.CreateObject("Scripting.FileSystemObject") >> ClientShortcut.vbs
echo rootPath = oFS.GetParentFolderName(oFS.GetParentFolderName(WScript.ScriptFullName)) >> ClientShortcut.vbs
echo Set oLink = oWS.CreateShortcut(DesktopPath ^& "\Write TOgether.lnk") >> ClientShortcut.vbs
echo oLink.Arguments = "localhost 5000" >> ClientShortcut.vbs
echo oLink.Description = "Write TOgether" >> ClientShortcut.vbs
echo oLink.IconLocation = rootPath ^& "\src\client\ui\images\app\logo_black.ico" >> ClientShortcut.vbs
echo oLink.TargetPath = rootPath ^& "\cmake-build-debug\src\client\client.exe" >> ClientShortcut.vbs & :: to be updated
echo oLink.WindowStyle = 1 >> ClientShortcut.vbs & :: normal, not maximized
echo oLink.WorkingDirectory = "%CD%\..\" >> ClientShortcut.vbs
echo oLink.Save >> ClientShortcut.vbs
echo end if >> ClientShortcut.vbs
cscript ClientShortcut.vbs /NoLogo /T:10 /D
del ClientShortcut.vbs