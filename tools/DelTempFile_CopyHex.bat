@echo off
set "year=%date:~0,4%"
set "month=%date:~5,2%"
set "day=%date:~8,2%"
set "hour_ten=%time:~0,1%"
set "hour_one=%time:~1,1%"
set "minute=%time:~3,2%"
set "second=%time:~6,2%"

if "%hour_ten%" == " " (
    set "dt=%year%%month%%day%-0%hour_one%%minute%%second%"
) else (
    set "dt=%year%%month%%day%-%hour_ten%%hour_one%%minute%%second%"
)

echo %dt%
pause

@echo on
del /f /s /q ..\release_smart_pjst_*.hex
copy Objects\smart_pjst.hex ..\release_smart_pjst_%dt%.hex
dir ..\release_smart_pjst_%dt%.hex