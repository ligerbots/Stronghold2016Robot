@echo off
rem Set up variables
set NAVX_LOCATION=%userprofile%/navx-mxp/cpp/include
set WPILIB_LOCATION=%userprofile%/wpilib/cpp/current/include
set PATH=%PATH%;C:\mingw\msys\1.0\bin

rem Clean up
IF EXIST "..\src\Stronghold2016Robot.h.gch" (
del ..\src\Stronghold2016Robot.h.gch
)

rem Precompile Stronghold2016Robot.h
IF EXIST "C:\mingw\msys\1.0\bin" (
echo Rebuilding index header
more ..\src\Includes.h > ..\src\Stronghold2016Robot.h
C:\mingw\msys\1.0\bin\find "../src/" -name "*.h" | sed -n "/Stronghold2016Robot\.h/!p" | sed -n "s|^\.\./src/|#include ""|p" | sed -n "s|$|""|p" >> ..\src\Stronghold2016Robot.h
arm-frc-linux-gnueabi-g++ -std=c++1y "-I..\\src" "-I%NAVX_LOCATION%" "-I%WPILIB_LOCATION%" -O0 -g3 -Wall -c -fmessage-length=0 -x c++-header -o "..\\src\\Stronghold2016Robot.h.gch" "..\\src\\Stronghold2016Robot.h" 
) ELSE (
echo No mingw, skipping precompiled headers
echo Add new headers to Stronghold2016Robot.h manually
)

IF EXIST "C:\Program Files\Cppcheck\cppcheck.exe" (
"C:\Program Files\Cppcheck\cppcheck.exe" --enable=warning,style,performance,portability,unusedFunction -I ..\src --inconclusive -rp=..\ -q -v --inline-suppr --template={file}:{line},{severity},{id},{message} ..\src
) ELSE (
echo No cppcheck, skipping code analysis
)