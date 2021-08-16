@echo off

SETLOCAL

if "%CUPLPATH%" == "" set CUPLPATH=c:\Wincupl\Shared

if not exist "bin" mkdir bin

echo Compiling %1.jed from %1.pld
echo.

%CUPLPATH%\cupl.exe

%CUPLPATH%\cupl.exe -e -x -f - -ju %CUPLPATH%\cupl.dl %1.pld
if %errorlevel% neq 0 exit /b %errorlevel%

REM Removed the location entry - the TL866II Plus programmer seems to have loading file issue

findstr /v /i /L /c:"Location" %1.jed >bin\%1.jed

move %1.doc bin\%1.doc
move %1.mx bin\%1.mx

del %1.jed

echo.
