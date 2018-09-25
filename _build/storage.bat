@echo off

if "%1"=="" goto usage

REM config vars
set ueVersion=4.18
set plugindir=%~dp0\..\Storage\StorageDemo\Plugins\AzureStorage
set pluginwin64dir=%plugindir%\Binaries\ThirdParty\Win64
set pluginwin32dir=%plugindir%\Binaries\ThirdParty\Win32


REM copy new triplets to vcpkg directory
echo Copying triplets to vcpkg
copy /y .\x86-windows-staticlib.cmake %1\triplets
copy /y .\x64-windows-staticlib.cmake %1\triplets

REM build storage sdk for x86 and x64
echo Building Storage SDK
%1\vcpkg install azure-storage-cpp:x86-windows-staticlib
%1\vcpkg install azure-storage-cpp:x64-windows-staticlib

REM make output directories
mkdir %pluginwin64dir% > NUL
mkdir %pluginwin32dir% > NUL

REM copy the resulting libs to the Plugins directory
echo Copying Win64 libs
copy /y "%1\packages\boost-system_x64-windows-staticlib\lib\boost_system-vc140-mt.lib" "%pluginwin64dir%"
copy /y "%1\packages\cpprestsdk_x64-windows-staticlib\lib\cpprest_2_10.lib" "%pluginwin64dir%"
copy /y "%1\packages\openssl-windows_x64-windows-staticlib\lib\libeay32.lib" "%pluginwin64dir%"
copy /y "%1\packages\openssl-windows_x64-windows-staticlib\lib\ssleay32.lib" "%pluginwin64dir%"
copy /y "%1\packages\azure-storage-cpp_x64-windows-staticlib\lib\wastorage.lib" "%pluginwin64dir%"
copy /y "%1\packages\zlib_x64-windows-staticlib\lib\zlib.lib" "%pluginwin64dir%"

echo Copying Win32 libs
copy /y "%1\packages\boost-system_x86-windows-staticlib\lib\boost_system-vc140-mt.lib" "%pluginwin32dir%"
copy /y "%1\packages\cpprestsdk_x86-windows-staticlib\lib\cpprest_2_10.lib" "%pluginwin32dir%"
copy /y "%1\packages\openssl-windows_x86-windows-staticlib\lib\libeay32.lib" "%pluginwin32dir%"
copy /y "%1\packages\openssl-windows_x86-windows-staticlib\lib\ssleay32.lib" "%pluginwin32dir%"
copy /y "%1\packages\azure-storage-cpp_x86-windows-staticlib\lib\wastorage.lib" "%pluginwin32dir%"
copy /y "%1\packages\zlib_x86-windows-staticlib\lib\zlib.lib" "%pluginwin32dir%"

REM build unreal plugin
echo Building Unreal plugin
call "C:\Program Files\Epic Games\UE_%ueVersion%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -plugin="%plugindir%\AzureStorage.uplugin" -package="%~dp0\..\Storage\AzureStorage" -TargetPlatforms=Win64
call "C:\Program Files\Epic Games\UE_%ueVersion%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -plugin="%plugindir%\AzureStorage.uplugin" -package="%~dp0\..\Storage\AzureStorage" -TargetPlatforms=Win32

echo Complete!
goto end

:usage
echo Usage: storage [path to vcpkg dir]

:end
