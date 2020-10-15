# Crypto++ Library Setup

## Windows
1. Download the official source library from [here](https://cryptopp.com/cryptopp820.zip)  
2. Decompress it anywhere in an empty folder, hereinafter referred to as _%CRYTPO_ROOT%_.
3. Open the _cryptest.sln_ project in Visual Studio.  
4. From the **Build -> Configuration Manager...** wizard mark only the _cryptlib_ project to be compiled.  
5. From the wizard **Project -> Properties** modify the following settings:  
6. On the general tab, check the _Windows SDK version_, and set the _Configuration Type_ to _Dynamic Library_.  
7. On the _C/C++ | Code generation_ tab, set the _Runtime Library_ to _Multi-threaded Debug DLL (/MDd)_.  
8. On the _Linker | Debugging_ tab, set the _General Debug Info_ to _/DEBUG_.  
9. Build the cryptlib project.

### IDE configuration
Be sure that the following CMAKE variables contain the reported paths.  
- CMAKE_PREFIX_PATH :
   - %CRYPTO_ROOT%
   - %CRYPTO_ROOT%\Win32\Output\Debug
- CMAKE_LIBRARY_PATH :
   - %CRYPTO_ROOT%\Win32\Output\Debug
   
## Linux
1. Download the official source library from [here](https://www.cryptopp.com/#download)
2. Run the following commands on a shell:
    ```bash
    mkdir -p cryptopp
    mv cryptopp*.zip cryptopp
    cd cryptopp
    unzip cryptopp*.zip
    make libcryptopp.a libcryptopp.so cryptest.exe
    make test
    sudo make install
    ```