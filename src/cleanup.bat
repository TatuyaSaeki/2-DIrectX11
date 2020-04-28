@echo off
cd /d %~dp0
del /Q /S Application\build\*.sdf
rd /Q /S Application\build\.vs
rd /Q /S Application\build\x64

del /Q /S Engine\build\*.sdf
rd /Q /S Engine\build\Debug
rd /Q /S Engine\build\Release
rd /Q /S Engine\build\ipch
rd /Q /S Engine\build\.vs
rd /Q /S Engine\build\x64

del /Q /S OpenSource\*.sdf
rd /Q /S OpenSource\DirectXTK-master\Bin\Desktop_2017_Win10\x64