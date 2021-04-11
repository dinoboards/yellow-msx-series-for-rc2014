@echo off

SETLOCAL

if "%CUPLPATH%" == "" set CUPLPATH=c:\Wincupl\Shared

if not exist "bin" mkdir bin

echo Compiling %1.jed from %1.pld
echo.

%CUPLPATH%\cupl.exe -ju %CUPLPATH%\cupl.dl %1.pld
if %errorlevel% neq 0 exit /b %errorlevel%

move /Y %1.jed bin\ >NUL

echo.
