@echo off
xcopy TestTemplate %1 /I /E /R /Y
ren .\%1\TestTemplate.* %1.*
