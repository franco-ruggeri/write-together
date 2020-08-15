# Crypto++ Library Setup

### Windows
Download the official source library from [here](https://cryptopp.com/cryptopp820.zip)  
Decompress it anywhere in an empty folder, hereinafter referred to as _%CRYTPO_ROOT%_.

Open the _cryptest.sln_ project in Visual Studio.  
From the **Build -> Configuration Manager...** wizard mark only the _cryptlib_ project to be compiled.  
From the wizard **Project -> Properties** modify the following settings:  
On the general tab, check the _Windows SDK version_, and set the _Configuration Type_ to _Dynamic Library_.  
On the _C/C++ | Code generation_ tab, set the _Runtime Library_ to _Multi-threaded Debug DLL (/MDd)_.  
On the _Linker | Debugging_ tab, set the _General Debug Info_ to _/DEBUG_.  
Build the cryptlib project.

#### IDE configuration
Be sure that the following CMAKE variables contain the reported paths.  
1. CMAKE_PREFIX_PATH :
   1. %CRYPTO_ROOT%
   2. %CRYPTO_ROOT%\Win32\Output\Debug
2. CMAKE_LIBRARY_PATH :
   1. %CRYPTO_ROOT%\Win32\Output\Debug