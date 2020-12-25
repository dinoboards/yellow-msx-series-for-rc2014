@echo off

SETLOCAL

if "%CUPLPATH%" == "" set CUPLPATH=c:\Wincupl\Shared

mkdir bin
%CUPLPATH%\cupl.exe -ju %CUPLPATH%\cupl.dl %1.pld
if %errorlevel% neq 0 exit /b %errorlevel%

move %1.jed bin\
