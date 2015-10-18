@echo off
echo Post build starting.

set outdir=%1
set libdir=%2

REM
REM copy libs
REM 
echo xcopy %libdir%*.dll %outdir% /I /R /Y
xcopy %libdir%*.dll %outdir% /I /R /Y
