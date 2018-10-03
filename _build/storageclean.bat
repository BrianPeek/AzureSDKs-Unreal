@echo off

echo Deleting directories...

rmdir /s /q ..\Storage\AzureStorage
rmdir /s /q ..\Storage\StorageDemo\Binaries
rmdir /s /q ..\Storage\StorageDemo\Intermediate
rmdir /s /q ..\Storage\StorageDemo\Plugins\AzureStorage\Intermediate
rmdir /s /q ..\Storage\StorageDemo\Plugins\AzureStorage\Binaries\Win32
rmdir /s /q ..\Storage\StorageDemo\Plugins\AzureStorage\Binaries\Win64

echo Complete
