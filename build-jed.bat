@echo off

SETLOCAL

if "%CUPLPATH%" == "" set CUPLPATH=c:\Wincupl\Shared

mkdir bin
%CUPLPATH%\cupl.exe -ju %CUPLPATH%\cupl.dl memory.pld
if %errorlevel% neq 0 exit /b %errorlevel%

move memory.jed bin\
